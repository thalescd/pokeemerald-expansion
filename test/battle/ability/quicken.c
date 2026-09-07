#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Quicken executes two-turn moves in one turn")
{
    bool32 hasQuicken;
    PARAMETRIZE { hasQuicken = FALSE; }
    PARAMETRIZE { hasQuicken = TRUE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SOLAR_BEAM) == EFFECT_SOLAR_BEAM);
        PLAYER(SPECIES_WOBBUFFET) { Ability(hasQuicken ? ABILITY_QUICKEN : ABILITY_ILLUMINATE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SOLAR_BEAM); }
    } SCENE {
        if (hasQuicken) {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SOLAR_BEAM, player);
            HP_BAR(opponent);
        } else {
            NOT HP_BAR(opponent);
        }
    }
}

SINGLE_BATTLE_TEST("Quicken allows semi-invulnerable two-turn moves to execute in one turn")
{
    bool32 hasQuicken;
    PARAMETRIZE { hasQuicken = FALSE; }
    PARAMETRIZE { hasQuicken = TRUE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_FLY) == EFFECT_SEMI_INVULNERABLE);
        PLAYER(SPECIES_PIDGEOT) { Ability(hasQuicken ? ABILITY_QUICKEN : ABILITY_KEEN_EYE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_FLY); }
    } SCENE {
        if (hasQuicken) {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_FLY, player);
            HP_BAR(opponent);
        } else {
            NOT HP_BAR(opponent);
        }
    }
}

SINGLE_BATTLE_TEST("Quicken semi-invulnerable moves do not keep the user untargetable that turn")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_PHANTOM_FORCE) == EFFECT_SEMI_INVULNERABLE);
        PLAYER(SPECIES_BASCULEGION) { Ability(ABILITY_QUICKEN); Speed(20); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_PHANTOM_FORCE); MOVE(opponent, MOVE_WATER_GUN); }
    } SCENE {
        MESSAGE("Basculegion used Phantom Force!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PHANTOM_FORCE, player);
        HP_BAR(opponent);
        MESSAGE("The opposing Wobbuffet used Water Gun!");
        NOT MESSAGE("Basculegion avoided the attack!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WATER_GUN, opponent);
        HP_BAR(player);
    }
}

SINGLE_BATTLE_TEST("Quicken semi-invulnerable moves do not keep the user untargetable on later turns")
{
    enum Move move;

    PARAMETRIZE { move = MOVE_FLY; }
    PARAMETRIZE { move = MOVE_DIG; }
    PARAMETRIZE { move = MOVE_DIVE; }
    PARAMETRIZE { move = MOVE_PHANTOM_FORCE; }

    GIVEN {
        ASSUME(GetMoveEffect(MOVE_FLY) == EFFECT_SEMI_INVULNERABLE);
        ASSUME(GetMoveEffect(MOVE_DIG) == EFFECT_SEMI_INVULNERABLE);
        ASSUME(GetMoveEffect(MOVE_DIVE) == EFFECT_SEMI_INVULNERABLE);
        ASSUME(GetMoveEffect(MOVE_PHANTOM_FORCE) == EFFECT_SEMI_INVULNERABLE);
        ASSUME(GetMoveAccuracy(MOVE_WATER_GUN) == 100);
        PLAYER(SPECIES_WOBBUFFET) { Speed(20); }
        OPPONENT(SPECIES_PIDGEOT) { Ability(ABILITY_QUICKEN); Speed(10); }
    } WHEN {
        TURN { MOVE(opponent, move); }
        TURN { MOVE(player, MOVE_WATER_GUN); }
    } SCENE {
        // Quicken turn: the move fires without a charging turn.
        ANIMATION(ANIM_TYPE_MOVE, move, opponent);
        HP_BAR(player);
        // Next turn: the user must be targetable again.
        MESSAGE("Wobbuffet used Water Gun!");
        NOT MESSAGE("The opposing Pidgeot avoided the attack!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WATER_GUN, player);
        HP_BAR(opponent);
    }
}

SINGLE_BATTLE_TEST("Quicken does not affect Sky Drop")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SKY_DROP) == EFFECT_SKY_DROP);
        PLAYER(SPECIES_PIDGEOT) { Ability(ABILITY_QUICKEN); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SKY_DROP); }
    } SCENE {
        NOT HP_BAR(opponent);
    }
}
