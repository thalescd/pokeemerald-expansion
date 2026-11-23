#include "global.h"
#include "battle.h"
#include "battle_message.h"
#include "bug_catching_contest.h"
#include "data.h"
#include "event_data.h"
#include "item.h"
#include "overworld.h"
#include "party_menu.h"
#include "pokemon.h"
#include "random.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "string_util.h"
#include "strings.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/opponents.h"
#include "constants/vars.h"

// Bug Catching Contest, based on the Bug-Catching Contest of Pokémon Gold/Silver.
// The player keeps only the first Pokémon of the party, fights wild bugs with it and throws Park Balls.
// Only one caught Pokémon is kept, and the contest is judged against some of the rivals below.
// All of the contest state lives in EWRAM and is never saved.

#define BUG_CONTEST_PLAYER      NUM_BUG_CONTEST_RIVALS // index of the player in the standings
#define NUM_BUG_CONTEST_CONTESTANTS (NUM_BUG_CONTEST_COMPETING + 1)
#define NUM_RIVAL_RESULTS       3
#define MAX_PLACES_ANNOUNCED    3

struct BugContestRivalResult
{
    u16 species;
    u16 score;
};

struct BugContestRival
{
    u16 trainerId; // only used for the class and the name
    struct BugContestRivalResult results[NUM_RIVAL_RESULTS];
};

struct BugContestStanding
{
    u8 contestant; // rival index, or BUG_CONTEST_PLAYER
    u16 species;
    u16 score;
    u16 tiebreak;
};

// Scores are balanced for the wild Pokémon of the Moth Garden with average IVs.
static const struct BugContestRival sRivals[NUM_BUG_CONTEST_RIVALS] =
{
    {
        .trainerId = TRAINER_DAVIS,
        .results = {
            { SPECIES_PINSIR,    340 },
            { SPECIES_GRUBBIN,   285 },
            { SPECIES_WURMPLE,   240 },
        },
    },
    {
        .trainerId = TRAINER_LYLE,
        .results = {
            { SPECIES_SKORUPI,   290 },
            { SPECIES_GRUBBIN,   270 },
            { SPECIES_WURMPLE,   255 },
        },
    },
    {
        .trainerId = TRAINER_RICK,
        .results = {
            { SPECIES_SURSKIT,   285 },
            { SPECIES_WURMPLE,   250 },
            { SPECIES_WURMPLE,   215 },
        },
    },
    {
        .trainerId = TRAINER_GREG,
        .results = {
            { SPECIES_SURSKIT,   300 },
            { SPECIES_SURSKIT,   270 },
            { SPECIES_WURMPLE,   230 },
        },
    },
    {
        .trainerId = TRAINER_BRENT,
        .results = {
            { SPECIES_HERACROSS, 360 },
            { SPECIES_PINSIR,    340 },
            { SPECIES_SKORUPI,   305 },
        },
    },
    {
        .trainerId = TRAINER_DEREK,
        .results = {
            { SPECIES_CUTIEFLY,  312 },
            { SPECIES_SURSKIT,   288 },
            { SPECIES_WURMPLE,   262 },
        },
    },
    {
        .trainerId = TRAINER_DONALD,
        .results = {
            { SPECIES_SIZZLIPEDE, 318 },
            { SPECIES_SKORUPI,    312 },
            { SPECIES_GRUBBIN,    285 },
        },
    },
    {
        .trainerId = TRAINER_SHANE,
        .results = {
            { SPECIES_CUTIEFLY,  305 },
            { SPECIES_SURSKIT,   280 },
            { SPECIES_WURMPLE,   245 },
        },
    },
    {
        .trainerId = TRAINER_GEORGIA,
        .results = {
            { SPECIES_CUTIEFLY,  295 },
            { SPECIES_WURMPLE,   278 },
            { SPECIES_SURSKIT,   260 },
        },
    },
    {
        .trainerId = TRAINER_CAROLINA,
        .results = {
            { SPECIES_SCYTHER,   357 },
            { SPECIES_HERACROSS, 343 },
            { SPECIES_DURANT,    330 },
        },
    },
};

extern const u8 BugContest_EventScript_TimesUp[];
extern const u8 BugContest_EventScript_RetirePrompt[];

EWRAM_DATA u8 gNumBugContestBalls = 0;
EWRAM_DATA u16 gBugContestStepCounter = 0;
EWRAM_DATA static bool8 sIsActive = FALSE;
EWRAM_DATA static bool8 sHasContestMon = FALSE;
EWRAM_DATA static struct Pokemon sContestMon = {0};
EWRAM_DATA static struct Pokemon sSavedParty[PARTY_SIZE] = {0};
EWRAM_DATA static u16 sCompetingRivals = 0; // bit per rival
EWRAM_DATA static u16 sRivalSpecies[NUM_BUG_CONTEST_RIVALS] = {0};
EWRAM_DATA static u16 sRivalScore[NUM_BUG_CONTEST_RIVALS] = {0};
EWRAM_DATA static struct BugContestStanding sStandings[NUM_BUG_CONTEST_CONTESTANTS] = {0};
EWRAM_DATA static u8 sPlayerPlace = 0; // 1-3 if the player placed, otherwise 0
EWRAM_DATA static bool8 sHasStandings = FALSE; // a contest has been judged since the game was started

static const u8 sText_BattleMenuStart[] = _("Battle{CLEAR_TO 56}Ball ");
static const u8 sText_BattleMenuEnd[] = _("\nPokémon{CLEAR_TO 56}Run");

bool8 IsBugContestActive(void)
{
    return sIsActive;
}

bool8 BugContestTakeStep(void)
{
    if (!sIsActive)
        return FALSE;

    if (gBugContestStepCounter != 0)
        gBugContestStepCounter--;
    if (gBugContestStepCounter == 0)
    {
        ScriptContext_SetupScript(BugContest_EventScript_TimesUp);
        return TRUE;
    }
    return FALSE;
}

void BugContestRetirePrompt(void)
{
    ScriptContext_SetupScript(BugContest_EventScript_RetirePrompt);
}

// Text of the battle action menu, with the Park Ball in place of the Bag.
void BugContest_BuildBattleMenuText(u8 *dst)
{
    u8 *ptr = StringCopy(dst, sText_BattleMenuStart);
    ptr = ConvertIntToDecimalStringN(ptr, gNumBugContestBalls, STR_CONV_MODE_LEFT_ALIGN, 2);
    StringCopy(ptr, sText_BattleMenuEnd);
}

bool8 BugContest_HasCaughtMon(void)
{
    return sHasContestMon;
}

// Fills the battle text buffers with the Pokémon already kept and the one just caught.
void BugContest_BufferSwitchPrompt(struct Pokemon *newMon)
{
    PREPARE_SPECIES_BUFFER(gBattleTextBuff1, GetMonData(&sContestMon, MON_DATA_SPECIES));
    PREPARE_SPECIES_BUFFER(gBattleTextBuff2, GetMonData(newMon, MON_DATA_SPECIES));
}

void BugContest_StoreCaughtMon(struct Pokemon *mon)
{
    sContestMon = *mon;
    sHasContestMon = TRUE;
}

static u16 CalculatePlayerScore(void)
{
    u32 score;

    if (!sHasContestMon)
        return 0;

    // Max HP * 4 + every other stat + remaining HP / 8 + 1 if holding an item.
    score = GetMonData(&sContestMon, MON_DATA_MAX_HP) * 4;
    score += GetMonData(&sContestMon, MON_DATA_ATK);
    score += GetMonData(&sContestMon, MON_DATA_DEF);
    score += GetMonData(&sContestMon, MON_DATA_SPEED);
    score += GetMonData(&sContestMon, MON_DATA_SPATK);
    score += GetMonData(&sContestMon, MON_DATA_SPDEF);
    score += GetMonData(&sContestMon, MON_DATA_HP) / 8;
    if (GetMonData(&sContestMon, MON_DATA_HELD_ITEM) != ITEM_NONE)
        score++;

    if (score > 0xFFFF)
        score = 0xFFFF;
    return score;
}

static void HideAllRivals(void)
{
    s32 i;

    for (i = 0; i < NUM_BUG_CONTEST_RIVALS; i++)
        FlagSet(FLAG_HIDE_BUG_CONTEST_RIVAL_1 + i);
}

// Picks NUM_BUG_CONTEST_COMPETING different rivals at random.
static void SelectRivals(void)
{
    u8 pool[NUM_BUG_CONTEST_RIVALS];
    u32 count = NUM_BUG_CONTEST_RIVALS;
    u32 i, pick;

    for (i = 0; i < NUM_BUG_CONTEST_RIVALS; i++)
        pool[i] = i;

    sCompetingRivals = 0;
    for (i = 0; i < NUM_BUG_CONTEST_COMPETING; i++)
    {
        pick = Random() % count;
        sCompetingRivals |= 1 << pool[pick];
        pool[pick] = pool[--count];
    }
}

// Higher score first, a random draw settles ties.
static bool8 DidStandingPlaceHigher(const struct BugContestStanding *a, const struct BugContestStanding *b)
{
    if (a->score != b->score)
        return a->score > b->score;
    return a->tiebreak > b->tiebreak;
}

static void JudgeContest(void)
{
    s32 i, j;
    s32 count = 0;
    struct BugContestStanding temp;

    // Every competing rival gets one of its three results, a little changed.
    for (i = 0; i < NUM_BUG_CONTEST_RIVALS; i++)
    {
        if (sCompetingRivals & (1 << i))
        {
            const struct BugContestRivalResult *result = &sRivals[i].results[Random() % NUM_RIVAL_RESULTS];

            sRivalSpecies[i] = result->species;
            sRivalScore[i] = result->score + (Random() & 7);
            sStandings[count].contestant = i;
            sStandings[count].species = sRivalSpecies[i];
            sStandings[count].score = sRivalScore[i];
            sStandings[count].tiebreak = Random();
            count++;
        }
    }

    sStandings[count].contestant = BUG_CONTEST_PLAYER;
    sStandings[count].species = sHasContestMon ? GetMonData(&sContestMon, MON_DATA_SPECIES) : SPECIES_NONE;
    sStandings[count].score = CalculatePlayerScore();
    sStandings[count].tiebreak = Random();
    count++;

    for (i = 0; i < count - 1; i++)
    {
        for (j = count - 1; j > i; j--)
        {
            if (DidStandingPlaceHigher(&sStandings[j], &sStandings[j - 1]))
            {
                temp = sStandings[j - 1];
                sStandings[j - 1] = sStandings[j];
                sStandings[j] = temp;
            }
        }
    }

    sHasStandings = TRUE;

    sPlayerPlace = 0;
    if (sHasContestMon)
    {
        for (i = 0; i < MAX_PLACES_ANNOUNCED; i++)
        {
            if (sStandings[i].contestant == BUG_CONTEST_PLAYER)
            {
                sPlayerPlace = i + 1;
                break;
            }
        }
    }
}

static void BufferContestantName(u8 *dst, u32 contestant)
{
    if (contestant == BUG_CONTEST_PLAYER)
    {
        StringCopy(dst, gSaveBlock2Ptr->playerName);
    }
    else
    {
        u16 trainerId = sRivals[contestant].trainerId;

        dst = StringCopy(dst, GetTrainerClassNameFromId(trainerId));
        dst = StringAppend(dst, gText_Space);
        StringAppend(dst, GetTrainerNameFromId(trainerId));
    }
}

// Back at the gate. Heals nothing, gives nothing: the scripts do that.
static void FinishContest(void)
{
    s32 i;

    // The first slot is already the live Pokémon, the others were set aside.
    for (i = 1; i < PARTY_SIZE; i++)
        gParties[B_TRAINER_PLAYER][i] = sSavedParty[i];
    CalculatePlayerPartyCount();

    sIsActive = FALSE;
    gNumBugContestBalls = 0;
    gBugContestStepCounter = 0;

    JudgeContest();

    // Only the rivals that took part show up at the gate.
    for (i = 0; i < NUM_BUG_CONTEST_RIVALS; i++)
    {
        if (sCompetingRivals & (1 << i))
            FlagClear(FLAG_HIDE_BUG_CONTEST_RIVAL_1 + i);
        else
            FlagSet(FLAG_HIDE_BUG_CONTEST_RIVAL_1 + i);
    }
    VarSet(VAR_BUG_CONTEST_STATE, BUG_CONTEST_STATE_RESULTS);
}

// The Pokémon fainted: the contest ends as it does in Gold, no whiteout.
void BugContest_DoWhiteOut(void)
{
    // Only the Pokémon in the contest is healed, the rest is still set aside.
    HealPlayerParty();
    FinishContest();
    SetWarpDestination(MAP_GROUP(MAP_VERDANTURF_TOWN_MOTH_GARDEN_ENTRANCE), MAP_NUM(MAP_VERDANTURF_TOWN_MOTH_GARDEN_ENTRANCE), 0, -1, -1);
    WarpIntoMap();
}

// Specials

// gSpecialVar_Result: 0 can enter, 1 first Pokémon is an Egg, 2 first Pokémon has fainted, 3 no room for a prize Pokémon
void BugContest_CheckEntry(void)
{
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];

    if (GetMonData(mon, MON_DATA_IS_EGG))
        gSpecialVar_Result = 1;
    else if (GetMonData(mon, MON_DATA_HP) == 0)
        gSpecialVar_Result = 2;
    else if (CalculatePlayerPartyCount() == PARTY_SIZE && IsPlayerPartyAndPokemonStorageFull())
        gSpecialVar_Result = 3;
    else
        gSpecialVar_Result = 0;
}

void BugContest_Start(void)
{
    s32 i;

    // Only the first Pokémon takes part. The rest waits with the officer.
    for (i = 0; i < PARTY_SIZE; i++)
        sSavedParty[i] = gParties[B_TRAINER_PLAYER][i];
    for (i = 1; i < PARTY_SIZE; i++)
        ZeroMonData(&gParties[B_TRAINER_PLAYER][i]);
    gPartiesCount[B_TRAINER_PLAYER] = 1;

    sHasContestMon = FALSE;
    gNumBugContestBalls = BUG_CONTEST_BALLS;
    gBugContestStepCounter = BUG_CONTEST_STEPS;
    sIsActive = TRUE;

    HideAllRivals();
    SelectRivals();
    VarSet(VAR_BUG_CONTEST_STATE, BUG_CONTEST_STATE_IN_CONTEST);
}

// Ends the contest and judges it.
void BugContest_Finish(void)
{
    if (sIsActive)
        FinishContest();
}

// gSpecialVar_Result: TRUE if there are results of a contest to show on the board.
// They are only kept in memory, so they are gone after the game is reset.
void BugContest_CheckBoard(void)
{
    gSpecialVar_Result = sHasStandings;
}

// gSpecialVar_0x8004: 0 first place, 1 second place, 2 third place
// Buffers the name, the species and the score of that place.
void BugContest_BufferPlace(void)
{
    const struct BugContestStanding *standing = &sStandings[gSpecialVar_0x8004];

    BufferContestantName(gStringVar1, standing->contestant);
    StringCopy(gStringVar2, GetSpeciesName(standing->species));
    ConvertIntToDecimalStringN(gStringVar3, standing->score, STR_CONV_MODE_LEFT_ALIGN, 3);
}

// gSpecialVar_Result: place of the player, 0 if none. Buffers the species and the score of the player.
void BugContest_BufferPlayerResult(void)
{
    gSpecialVar_Result = sPlayerPlace;
    gSpecialVar_0x8005 = sHasContestMon;
    if (sHasContestMon)
    {
        StringCopy(gStringVar2, GetSpeciesName(GetMonData(&sContestMon, MON_DATA_SPECIES)));
        ConvertIntToDecimalStringN(gStringVar3, CalculatePlayerScore(), STR_CONV_MODE_LEFT_ALIGN, 3);
    }
}

// gSpecialVar_Result: 0 nothing to give, 1 went to the party, 2 went to a box
void BugContest_GivePlayerMon(void)
{
    if (!sHasContestMon)
    {
        gSpecialVar_Result = 0;
        return;
    }

    GetMonNickname(&sContestMon, gStringVar1);
    gSpecialVar_Result = (GiveCapturedMonToPlayer(&sContestMon) == MON_GIVEN_TO_PARTY) ? 1 : 2;
    sHasContestMon = FALSE;
}

// gSpecialVar_0x8004: rival index. gSpecialVar_Result: TRUE if the rival took part in the last contest.
// Buffers the name, the species and the score of the rival.
void BugContest_BufferRival(void)
{
    u32 rival = gSpecialVar_0x8004;

    if (rival >= NUM_BUG_CONTEST_RIVALS || !(sCompetingRivals & (1 << rival)))
    {
        gSpecialVar_Result = FALSE;
        return;
    }

    BufferContestantName(gStringVar1, rival);
    StringCopy(gStringVar2, GetSpeciesName(sRivalSpecies[rival]));
    ConvertIntToDecimalStringN(gStringVar3, sRivalScore[rival], STR_CONV_MODE_LEFT_ALIGN, 3);
    gSpecialVar_Result = TRUE;
}

// Prize item for gSpecialVar_Result = place of the player. Stored for the officer in case the bag is full.
void BugContest_SetPrizeForPlace(void)
{
    enum Item prize;

    switch (gSpecialVar_Result)
    {
    case 1:
        prize = ITEM_SUN_STONE;
        break;
    case 2:
        prize = ITEM_EVERSTONE;
        break;
    case 3:
        prize = ITEM_SITRUS_BERRY;
        break;
    default:
        prize = ITEM_ORAN_BERRY;
        break;
    }
    VarSet(VAR_BUG_CONTEST_PRIZE, prize);
}
