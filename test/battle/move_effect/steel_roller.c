#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(GetMoveEffect(MOVE_STEEL_ROLLER) == EFFECT_STEEL_ROLLER);
}

// Covered in ice_spinner.c
// SINGLE_BATTLE_TEST("Ice Spinner and Steel Roller remove a terrain from field")

SINGLE_BATTLE_TEST("Steel Roller removes Terrain even if user faints during attack execution")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_LIFE_ORB); HP(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_ELECTRIC_TERRAIN); MOVE(opponent, MOVE_STEEL_ROLLER); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ELECTRIC_TERRAIN, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STEEL_ROLLER, opponent);
        MESSAGE("The electricity disappeared from the battlefield.");
    }
}

SINGLE_BATTLE_TEST("Steel Roller removes Terrain if user is switched out due to Red Card")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_RED_CARD); }
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_ELECTRIC_TERRAIN); MOVE(opponent, MOVE_STEEL_ROLLER); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ELECTRIC_TERRAIN, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STEEL_ROLLER, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("The electricity disappeared from the battlefield.");
    }
}

SINGLE_BATTLE_TEST("Steel Roller does not fail if there is no Terrain")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_STEEL_ROLLER); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STEEL_ROLLER, opponent);
        NOT MESSAGE("But it failed!");
    }
}

SINGLE_BATTLE_TEST("Steel Roller deals half damage without terrain", s16 damage)
{
    bool32 hasTerrain;
    PARAMETRIZE { hasTerrain = FALSE; }
    PARAMETRIZE { hasTerrain = TRUE; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        if (hasTerrain)
            TURN { MOVE(player, MOVE_ELECTRIC_TERRAIN); MOVE(opponent, MOVE_SPLASH); }
        TURN { MOVE(opponent, MOVE_STEEL_ROLLER); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STEEL_ROLLER, opponent);
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[1].damage);
    }
}

AI_SINGLE_BATTLE_TEST("Steel Roller is chosen by AI even without terrain on the field")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_STEEL_ROLLER, MOVE_SPLASH); }
    } WHEN {
        TURN { EXPECT_MOVE(opponent, MOVE_STEEL_ROLLER); }
    }
}

SINGLE_BATTLE_TEST("Steel Roller will remove terrain if target is behind a Substitute")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_GRASSY_TERRAIN); }
        TURN { MOVE(player, MOVE_SUBSTITUTE); MOVE(opponent, MOVE_STEEL_ROLLER); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GRASSY_TERRAIN, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STEEL_ROLLER, opponent);
        NOT HP_BAR(player);
    }
}
