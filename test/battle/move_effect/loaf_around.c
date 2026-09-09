#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(GetMoveEffect(MOVE_LOAF_AROUND) == EFFECT_LOAF_AROUND);
    ASSUME(GetMoveTarget(MOVE_LOAF_AROUND) == TARGET_USER);
}

SINGLE_BATTLE_TEST("Loaf Around fails if the user does not have Truant")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(50); MaxHP(128); Moves(MOVE_LOAF_AROUND); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_LOAF_AROUND); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_LOAF_AROUND, player);
        MESSAGE("But it failed!");
    } THEN {
        EXPECT(player->hp == 50);
    }
}

SINGLE_BATTLE_TEST("Loaf Around brings Truant's idle turn forward, leaving the next turn free")
{
    GIVEN {
        PLAYER(SPECIES_SLAKING) { Moves(MOVE_SCRATCH, MOVE_LOAF_AROUND); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
        TURN { MOVE(player, MOVE_SCRATCH); }        // Truant's idle turn
        TURN { MOVE(player, MOVE_LOAF_AROUND); }
        TURN { MOVE(player, MOVE_SCRATCH); }        // Free to act again
        TURN { MOVE(player, MOVE_SCRATCH); }        // Truant's idle turn
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);

        ABILITY_POPUP(player, ABILITY_TRUANT);
        MESSAGE("Slaking is loafing around!");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);

        ANIMATION(ANIM_TYPE_MOVE, MOVE_LOAF_AROUND, player);
        MESSAGE("Slaking is loafing around!");

        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);

        ABILITY_POPUP(player, ABILITY_TRUANT);
        MESSAGE("Slaking is loafing around!");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
    }
}

SINGLE_BATTLE_TEST("Loaf Around restores 1/8 of the user's max HP")
{
    GIVEN {
        PLAYER(SPECIES_SLAKING) { HP(50); MaxHP(128); Moves(MOVE_LOAF_AROUND); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_LOAF_AROUND); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LOAF_AROUND, player);
        HP_BAR(player, hp: 66);
    } THEN {
        EXPECT(player->hp == 66);
    }
}

SINGLE_BATTLE_TEST("Loaf Around still shifts Truant's idle turn at full HP")
{
    GIVEN {
        PLAYER(SPECIES_SLAKING) { Moves(MOVE_SCRATCH, MOVE_LOAF_AROUND); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_LOAF_AROUND); }
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LOAF_AROUND, player);
        MESSAGE("Slaking is loafing around!");
        NOT HP_BAR(player);

        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
    } THEN {
        EXPECT(player->hp == player->maxHP);
    }
}

SINGLE_BATTLE_TEST("Loaf Around is not blocked by Heal Block but does not heal either")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_HEAL_BLOCK) == EFFECT_HEAL_BLOCK);
        PLAYER(SPECIES_SLAKING) { HP(50); MaxHP(128); Speed(5); Moves(MOVE_SCRATCH, MOVE_LOAF_AROUND); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); Moves(MOVE_HEAL_BLOCK, MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_HEAL_BLOCK); MOVE(player, MOVE_LOAF_AROUND); }
        TURN { MOVE(opponent, MOVE_SPLASH); MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HEAL_BLOCK, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LOAF_AROUND, player);
        MESSAGE("Slaking is loafing around!");

        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
    } THEN {
        EXPECT(player->hp == 50);
    }
}

// The idle turn has to line up with the target being underground: the AI only gains anything
// from Loaf Around on a turn it could act but could not land a hit.
AI_SINGLE_BATTLE_TEST("Loaf Around is chosen by the AI when it is faster and the target is semi-invulnerable")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_TRAPINCH) { Speed(5); Moves(MOVE_DIG, MOVE_SPLASH); }
        OPPONENT(SPECIES_SLAKING) { Speed(50); Moves(MOVE_SCRATCH, MOVE_LOAF_AROUND); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_SCRATCH); }
        TURN { MOVE(player, MOVE_DIG); EXPECT_MOVE(opponent, MOVE_SCRATCH); }   // Truant's idle turn
        TURN { SKIP_TURN(player); EXPECT_MOVE(opponent, MOVE_LOAF_AROUND); }
    }
}
