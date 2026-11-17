#include "global.h"
#include "event_data.h"
#include "hidden_grotto.h"
#include "overworld.h"
#include "pokemon.h"
#include "random.h"
#include "wild_encounter.h"
#include "constants/event_objects.h"
#include "constants/secret_bases.h"
#include "constants/wild_encounter.h"

// Hidden grottos live behind the old secret base entrances: Secret Power opens the entrance,
// and the interior holds at most one wild Pokemon with its Hidden Ability.
//
// Each grotto keeps 4 bits in gSaveBlock1Ptr->hiddenGrottoStates:
//   bit 3    - the grotto has a Pokemon
//   bits 0-2 - the grotto_mons slot rolled when it was refilled
// Species and level come from the grotto_mons table of the route the entrance is on.

#define GROTTO_STATE_FULL       (1 << 3)
#define GROTTO_STATE_SLOT_MASK  0x7

#define GROTTO_REFILL_STEPS     256
#define GROTTO_REFILL_CHANCE    5 // Percent, rolled for every empty grotto each GROTTO_REFILL_STEPS steps

#define GROTTO_NONE             HIDDEN_GROTTO_COUNT

STATIC_ASSERT(NUM_GROTTO_MONS_ENCOUNTER_SLOTS <= GROTTO_STATE_SLOT_MASK + 1, GrottoSlotFitsInState);

// Grotto index -> secret base id of its entrance
static const u8 sHiddenGrottoSecretBaseIds[HIDDEN_GROTTO_COUNT] =
{
    SECRET_BASE_RED_CAVE1_1,
    SECRET_BASE_RED_CAVE1_2,
    SECRET_BASE_RED_CAVE1_3,
    SECRET_BASE_RED_CAVE2_1,
    SECRET_BASE_RED_CAVE2_2,
    SECRET_BASE_RED_CAVE2_3,
    SECRET_BASE_RED_CAVE3_1,
    SECRET_BASE_RED_CAVE3_2,
    SECRET_BASE_RED_CAVE3_3,
    SECRET_BASE_RED_CAVE4_1,
    SECRET_BASE_RED_CAVE4_2,
    SECRET_BASE_RED_CAVE4_3,
    SECRET_BASE_BROWN_CAVE1_1,
    SECRET_BASE_BROWN_CAVE1_2,
    SECRET_BASE_BROWN_CAVE1_3,
    SECRET_BASE_BROWN_CAVE2_1,
    SECRET_BASE_BROWN_CAVE2_2,
    SECRET_BASE_BROWN_CAVE2_3,
    SECRET_BASE_BROWN_CAVE3_1,
    SECRET_BASE_BROWN_CAVE3_2,
    SECRET_BASE_BROWN_CAVE3_3,
    SECRET_BASE_BROWN_CAVE4_1,
    SECRET_BASE_BROWN_CAVE4_2,
    SECRET_BASE_BROWN_CAVE4_3,
    SECRET_BASE_BLUE_CAVE1_1,
    SECRET_BASE_BLUE_CAVE1_2,
    SECRET_BASE_BLUE_CAVE1_3,
    SECRET_BASE_BLUE_CAVE2_1,
    SECRET_BASE_BLUE_CAVE2_2,
    SECRET_BASE_BLUE_CAVE2_3,
    SECRET_BASE_BLUE_CAVE3_1,
    SECRET_BASE_BLUE_CAVE3_2,
    SECRET_BASE_BLUE_CAVE3_3,
    SECRET_BASE_BLUE_CAVE4_1,
    SECRET_BASE_BLUE_CAVE4_2,
    SECRET_BASE_BLUE_CAVE4_3,
    SECRET_BASE_YELLOW_CAVE1_1,
    SECRET_BASE_YELLOW_CAVE1_2,
    SECRET_BASE_YELLOW_CAVE1_3,
    SECRET_BASE_YELLOW_CAVE2_1,
    SECRET_BASE_YELLOW_CAVE2_2,
    SECRET_BASE_YELLOW_CAVE2_3,
    SECRET_BASE_YELLOW_CAVE3_1,
    SECRET_BASE_YELLOW_CAVE3_2,
    SECRET_BASE_YELLOW_CAVE3_3,
    SECRET_BASE_YELLOW_CAVE4_1,
    SECRET_BASE_YELLOW_CAVE4_2,
    SECRET_BASE_YELLOW_CAVE4_3,
    SECRET_BASE_TREE1_1,
    SECRET_BASE_TREE1_2,
    SECRET_BASE_TREE1_3,
    SECRET_BASE_TREE1_4,
    SECRET_BASE_TREE2_1,
    SECRET_BASE_TREE2_2,
    SECRET_BASE_TREE2_3,
    SECRET_BASE_TREE2_4,
    SECRET_BASE_TREE3_1,
    SECRET_BASE_TREE3_2,
    SECRET_BASE_TREE3_3,
    SECRET_BASE_TREE4_1,
    SECRET_BASE_TREE4_2,
    SECRET_BASE_TREE4_3,
    SECRET_BASE_SHRUB1_1,
    SECRET_BASE_SHRUB1_2,
    SECRET_BASE_SHRUB1_3,
    SECRET_BASE_SHRUB1_4,
    SECRET_BASE_SHRUB2_1,
    SECRET_BASE_SHRUB2_2,
    SECRET_BASE_SHRUB2_3,
    SECRET_BASE_SHRUB3_1,
    SECRET_BASE_SHRUB3_2,
    SECRET_BASE_SHRUB3_3,
    SECRET_BASE_SHRUB4_1,
    SECRET_BASE_SHRUB4_2,
    SECRET_BASE_SHRUB4_3,
};

static u32 GetGrottoState(u32 grotto)
{
    u32 stateByte = gSaveBlock1Ptr->hiddenGrottoStates[grotto / 2];

    if (grotto % 2)
        return stateByte >> 4;
    return stateByte & 0xF;
}

static void SetGrottoState(u32 grotto, u32 state)
{
    u8 *stateByte = &gSaveBlock1Ptr->hiddenGrottoStates[grotto / 2];

    if (grotto % 2)
        *stateByte = (*stateByte & 0x0F) | (state << 4);
    else
        *stateByte = (*stateByte & 0xF0) | (state & 0xF);
}

static void RefillHiddenGrotto(u32 grotto)
{
    SetGrottoState(grotto, GROTTO_STATE_FULL | ChooseWildMonIndex_Grotto());
}

static u32 GetHiddenGrottoIndex(u32 secretBaseId)
{
    for (u32 i = 0; i < HIDDEN_GROTTO_COUNT; i++)
    {
        if (sHiddenGrottoSecretBaseIds[i] == secretBaseId)
            return i;
    }

    return GROTTO_NONE;
}

// The grotto is saved in a var so it survives saving and reloading inside the grotto.
void SetCurrentHiddenGrotto(u32 secretBaseId)
{
    VarSet(VAR_HIDDEN_GROTTO_ID, GetHiddenGrottoIndex(secretBaseId));
}

// Daily reset and new game: every grotto gets a fresh Pokemon, including the ones still full.
void RefillAllHiddenGrottos(void)
{
    for (u32 i = 0; i < HIDDEN_GROTTO_COUNT; i++)
        RefillHiddenGrotto(i);
}

void UpdateHiddenGrottoStepCounter(void)
{
    u16 *steps = GetVarPointer(VAR_HIDDEN_GROTTO_STEP_COUNTER);

    (*steps)++;
    (*steps) %= GROTTO_REFILL_STEPS;
    if (*steps != 0)
        return;

    for (u32 i = 0; i < HIDDEN_GROTTO_COUNT; i++)
    {
        if (!(GetGrottoState(i) & GROTTO_STATE_FULL) && RandomPercentage(RNG_HIDDEN_GROTTO_REFILL, GROTTO_REFILL_CHANCE))
            RefillHiddenGrotto(i);
    }
}

// The Pokemon waiting in the current grotto, or NULL if it is empty.
static const struct WildPokemon *GetHiddenGrottoWildMon(void)
{
    u32 grotto = VarGet(VAR_HIDDEN_GROTTO_ID);
    if (grotto >= HIDDEN_GROTTO_COUNT)
        return NULL;

    u32 state = GetGrottoState(grotto);
    u32 slot = state & GROTTO_STATE_SLOT_MASK;
    if (!(state & GROTTO_STATE_FULL) || slot >= NUM_GROTTO_MONS_ENCOUNTER_SLOTS)
        return NULL;

    // The dynamic warp points back at the entrance's route.
    const struct WildPokemonInfo *info = GetGrottoMonsInfo(gSaveBlock1Ptr->dynamicWarp.mapGroup, gSaveBlock1Ptr->dynamicWarp.mapNum);
    if (info == NULL)
        return NULL;

    return &info->wildPokemon[slot];
}

// Runs on transition into a grotto interior: shows the Pokemon's sprite, or hides the object when empty.
void SetupHiddenGrottoInterior(void)
{
    const struct WildPokemon *wildMon = GetHiddenGrottoWildMon();

    if (wildMon == NULL)
    {
        FlagSet(FLAG_HIDE_SECRET_BASE_TRAINER);
        return;
    }

    VarSet(VAR_OBJ_GFX_ID_F, wildMon->species + OBJ_EVENT_MON);
    FlagClear(FLAG_HIDE_SECRET_BASE_TRAINER);
}

// Creates the grotto Pokemon as the scripted wild battle opponent.
// Sets VAR_RESULT to FALSE if the grotto is empty, and VAR_0x8004 to the species for the cry.
void CreateHiddenGrottoMon(void)
{
    const struct WildPokemon *wildMon = GetHiddenGrottoWildMon();

    if (wildMon == NULL)
    {
        gSpecialVar_Result = FALSE;
        return;
    }

    u32 level = wildMon->minLevel;
    if (wildMon->maxLevel > wildMon->minLevel)
        level = RandomUniform(RNG_HIDDEN_GROTTO_LEVEL, wildMon->minLevel, wildMon->maxLevel);

    CreateWildMon(wildMon->species, level);

    struct Pokemon *mon = &gParties[B_TRAINER_OPPONENT_A][0];
    u8 abilityNum = NUM_NORMAL_ABILITY_SLOTS; // The hidden ability slot
    SetMonData(mon, MON_DATA_ABILITY_NUM, &abilityNum);

    // Without this the met location would be the grotto interior's map section.
    u8 metLocation = Overworld_GetMapHeaderByGroupAndId(gSaveBlock1Ptr->dynamicWarp.mapGroup, gSaveBlock1Ptr->dynamicWarp.mapNum)->regionMapSectionId;
    SetMonData(mon, MON_DATA_MET_LOCATION, &metLocation);

    gSpecialVar_0x8004 = wildMon->species;
    gSpecialVar_Result = TRUE;
}

// The grotto empties as soon as the battle starts, so fleeing, winning, catching and whiting out all empty it.
void ClearHiddenGrottoMon(void)
{
    u32 grotto = VarGet(VAR_HIDDEN_GROTTO_ID);

    if (grotto < HIDDEN_GROTTO_COUNT)
        SetGrottoState(grotto, 0);
}
