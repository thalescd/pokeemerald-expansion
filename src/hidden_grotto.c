#include "global.h"
#include "event_data.h"
#include "field_camera.h"
#include "field_player_avatar.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "fieldmap.h"
#include "hidden_grotto.h"
#include "main.h"
#include "overworld.h"
#include "palette.h"
#include "pokemon.h"
#include "random.h"
#include "script.h"
#include "task.h"
#include "wild_encounter.h"
#include "constants/event_bg.h"
#include "constants/event_objects.h"
#include "constants/metatile_behaviors.h"
#include "constants/metatile_labels.h"
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

// Interior map of each secret base group, indexed by secret base id / 10.
// SECRET_BASE_GROUP() multiplies the group by 4, hence the division in the designators.
static const u8 sGrottoInteriorMapNums[NUM_SECRET_BASE_GROUPS] =
{
    [SECRET_BASE_RED_CAVE1 / 4]    = MAP_NUM(MAP_SECRET_BASE_RED_CAVE1),
    [SECRET_BASE_RED_CAVE2 / 4]    = MAP_NUM(MAP_SECRET_BASE_RED_CAVE2),
    [SECRET_BASE_RED_CAVE3 / 4]    = MAP_NUM(MAP_SECRET_BASE_RED_CAVE3),
    [SECRET_BASE_RED_CAVE4 / 4]    = MAP_NUM(MAP_SECRET_BASE_RED_CAVE4),
    [SECRET_BASE_BROWN_CAVE1 / 4]  = MAP_NUM(MAP_SECRET_BASE_BROWN_CAVE1),
    [SECRET_BASE_BROWN_CAVE2 / 4]  = MAP_NUM(MAP_SECRET_BASE_BROWN_CAVE2),
    [SECRET_BASE_BROWN_CAVE3 / 4]  = MAP_NUM(MAP_SECRET_BASE_BROWN_CAVE3),
    [SECRET_BASE_BROWN_CAVE4 / 4]  = MAP_NUM(MAP_SECRET_BASE_BROWN_CAVE4),
    [SECRET_BASE_BLUE_CAVE1 / 4]   = MAP_NUM(MAP_SECRET_BASE_BLUE_CAVE1),
    [SECRET_BASE_BLUE_CAVE2 / 4]   = MAP_NUM(MAP_SECRET_BASE_BLUE_CAVE2),
    [SECRET_BASE_BLUE_CAVE3 / 4]   = MAP_NUM(MAP_SECRET_BASE_BLUE_CAVE3),
    [SECRET_BASE_BLUE_CAVE4 / 4]   = MAP_NUM(MAP_SECRET_BASE_BLUE_CAVE4),
    [SECRET_BASE_YELLOW_CAVE1 / 4] = MAP_NUM(MAP_SECRET_BASE_YELLOW_CAVE1),
    [SECRET_BASE_YELLOW_CAVE2 / 4] = MAP_NUM(MAP_SECRET_BASE_YELLOW_CAVE2),
    [SECRET_BASE_YELLOW_CAVE3 / 4] = MAP_NUM(MAP_SECRET_BASE_YELLOW_CAVE3),
    [SECRET_BASE_YELLOW_CAVE4 / 4] = MAP_NUM(MAP_SECRET_BASE_YELLOW_CAVE4),
    [SECRET_BASE_TREE1 / 4]        = MAP_NUM(MAP_SECRET_BASE_TREE1),
    [SECRET_BASE_TREE2 / 4]        = MAP_NUM(MAP_SECRET_BASE_TREE2),
    [SECRET_BASE_TREE3 / 4]        = MAP_NUM(MAP_SECRET_BASE_TREE3),
    [SECRET_BASE_TREE4 / 4]        = MAP_NUM(MAP_SECRET_BASE_TREE4),
    [SECRET_BASE_SHRUB1 / 4]       = MAP_NUM(MAP_SECRET_BASE_SHRUB1),
    [SECRET_BASE_SHRUB2 / 4]       = MAP_NUM(MAP_SECRET_BASE_SHRUB2),
    [SECRET_BASE_SHRUB3 / 4]       = MAP_NUM(MAP_SECRET_BASE_SHRUB3),
    [SECRET_BASE_SHRUB4 / 4]       = MAP_NUM(MAP_SECRET_BASE_SHRUB4),
};

struct GrottoEntranceMetatiles
{
    u16 closedMetatileId;
    u16 openMetatileId;
};

static const struct GrottoEntranceMetatiles sGrottoEntranceMetatiles[] =
{
    {.closedMetatileId = METATILE_General_SecretBase_TreeLeft,  .openMetatileId = METATILE_General_SecretBase_VineLeft},
    {.closedMetatileId = METATILE_General_SecretBase_TreeRight, .openMetatileId = METATILE_General_SecretBase_VineRight},
    {.closedMetatileId = METATILE_General_RedCaveIndent,        .openMetatileId = METATILE_General_RedCaveOpen},
    {.closedMetatileId = METATILE_General_YellowCaveIndent,     .openMetatileId = METATILE_General_YellowCaveOpen},
    {.closedMetatileId = METATILE_General_BlueCaveIndent,       .openMetatileId = METATILE_General_BlueCaveOpen},
    {.closedMetatileId = METATILE_Fallarbor_BrownCaveIndent,    .openMetatileId = METATILE_Fallarbor_BrownCaveOpen},
    {.closedMetatileId = METATILE_Fortree_SecretBase_Shrub,     .openMetatileId = METATILE_Fortree_SecretBase_ShrubOpen},
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
// It is set when the player faces an entrance, before Secret Power is used.
void SetCurrentHiddenGrotto(u32 secretBaseId)
{
    VarSet(VAR_HIDDEN_GROTTO_ID, GetHiddenGrottoIndex(secretBaseId));
}

void SetCurrentHiddenGrottoFromPosition(const struct MapPosition *position, const struct MapEvents *events)
{
    for (u32 i = 0; i < events->bgEventCount; i++)
    {
        if (events->bgEvents[i].kind == BG_EVENT_SECRET_BASE
         && position->x == events->bgEvents[i].x + MAP_OFFSET
         && position->y == events->bgEvents[i].y + MAP_OFFSET)
        {
            SetCurrentHiddenGrotto(events->bgEvents[i].bgUnion.secretBaseId);
            return;
        }
    }
}

// Special: sets VAR_0x8007 to the SECRET_BASE_* entrance type in front of the player, or 0.
void GetHiddenGrottoEntranceType(void)
{
    s16 x, y;
    u32 behavior;

    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
    behavior = MapGridGetMetatileBehaviorAt(x, y) & 0xFFF;

    if (behavior == MB_SECRET_BASE_SPOT_RED_CAVE || behavior == MB_SECRET_BASE_SPOT_RED_CAVE_OPEN)
        gSpecialVar_0x8007 = SECRET_BASE_RED_CAVE;
    else if (behavior == MB_SECRET_BASE_SPOT_BROWN_CAVE || behavior == MB_SECRET_BASE_SPOT_BROWN_CAVE_OPEN)
        gSpecialVar_0x8007 = SECRET_BASE_BROWN_CAVE;
    else if (behavior == MB_SECRET_BASE_SPOT_BLUE_CAVE || behavior == MB_SECRET_BASE_SPOT_BLUE_CAVE_OPEN)
        gSpecialVar_0x8007 = SECRET_BASE_BLUE_CAVE;
    else if (behavior == MB_SECRET_BASE_SPOT_YELLOW_CAVE || behavior == MB_SECRET_BASE_SPOT_YELLOW_CAVE_OPEN)
        gSpecialVar_0x8007 = SECRET_BASE_YELLOW_CAVE;
    else if (behavior == MB_SECRET_BASE_SPOT_TREE_LEFT  || behavior == MB_SECRET_BASE_SPOT_TREE_LEFT_OPEN
          || behavior == MB_SECRET_BASE_SPOT_TREE_RIGHT || behavior == MB_SECRET_BASE_SPOT_TREE_RIGHT_OPEN)
        gSpecialVar_0x8007 = SECRET_BASE_TREE;
    else if (behavior == MB_SECRET_BASE_SPOT_SHRUB || behavior == MB_SECRET_BASE_SPOT_SHRUB_OPEN)
        gSpecialVar_0x8007 = SECRET_BASE_SHRUB;
    else
        gSpecialVar_0x8007 = 0;
}

// Opens or closes the entrance metatile in front of the player (Secret Power animations).
void ToggleHiddenGrottoEntranceMetatile(void)
{
    u32 i;
    s16 x, y;
    u32 metatileId;

    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
    metatileId = MapGridGetMetatileIdAt(x, y);

    for (i = 0; i < ARRAY_COUNT(sGrottoEntranceMetatiles); i++)
    {
        if (sGrottoEntranceMetatiles[i].closedMetatileId == metatileId)
        {
            MapGridSetMetatileIdAt(x, y, sGrottoEntranceMetatiles[i].openMetatileId | MAPGRID_IMPASSABLE);
            CurrentMapDrawMetatileAt(x, y);
            return;
        }
    }

    for (i = 0; i < ARRAY_COUNT(sGrottoEntranceMetatiles); i++)
    {
        if (sGrottoEntranceMetatiles[i].openMetatileId == metatileId)
        {
            MapGridSetMetatileIdAt(x, y, sGrottoEntranceMetatiles[i].closedMetatileId | MAPGRID_IMPASSABLE);
            CurrentMapDrawMetatileAt(x, y);
            return;
        }
    }
}

#define tState data[0]

static void Task_EnterHiddenGrotto(u8 taskId)
{
    u32 secretBaseId;

    switch (gTasks[taskId].tState)
    {
    case 0:
        if (!gPaletteFade.active)
            gTasks[taskId].tState = 1;
        break;
    case 1:
        secretBaseId = sHiddenGrottoSecretBaseIds[VarGet(VAR_HIDDEN_GROTTO_ID)];
        SetWarpDestinationToMapWarp(MAP_GROUP(MAP_SECRET_BASE_RED_CAVE1), sGrottoInteriorMapNums[secretBaseId / 10], 0);
        WarpIntoMap();
        gFieldCallback = FieldCB_ContinueScriptHandleMusic;
        SetMainCallback2(CB2_LoadMap);
        DestroyTask(taskId);
        break;
    }
}

#undef tState

// Special (waitstate): warps into the current grotto. The interior's exit warp leads back here.
void EnterHiddenGrotto(void)
{
    if (VarGet(VAR_HIDDEN_GROTTO_ID) >= HIDDEN_GROTTO_COUNT)
    {
        ScriptContext_Enable();
        return;
    }

    CreateTask(Task_EnterHiddenGrotto, 0);
    FadeScreen(FADE_TO_BLACK, 0);
    SetDynamicWarp(0, gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum, WARP_ID_NONE);
}

bool32 CurMapIsHiddenGrotto(void)
{
    return gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_SECRET_BASE_RED_CAVE1)
        && (u8)gSaveBlock1Ptr->location.mapNum <= MAP_NUM(MAP_SECRET_BASE_SHRUB4);
}

// The interiors are the old secret base maps: their PC becomes plain ground.
void InitHiddenGrottoMetatiles(void)
{
    const struct MapLayout *mapLayout = gMapHeader.mapLayout;

    if (!CurMapIsHiddenGrotto())
        return;

    for (s32 y = 0; y < mapLayout->height; y++)
    {
        for (s32 x = 0; x < mapLayout->width; x++)
        {
            if ((mapLayout->map[y * mapLayout->width + x] & MAPGRID_METATILE_ID_MASK) == METATILE_SecretBase_PC)
            {
                MapGridSetMetatileIdAt(x + MAP_OFFSET, y + MAP_OFFSET, METATILE_SecretBase_Ground | MAPGRID_IMPASSABLE);
                return;
            }
        }
    }
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
        FlagSet(FLAG_HIDE_HIDDEN_GROTTO_POKEMON);
        return;
    }

    VarSet(VAR_OBJ_GFX_ID_F, wildMon->species + OBJ_EVENT_MON);
    FlagClear(FLAG_HIDE_HIDDEN_GROTTO_POKEMON);
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
