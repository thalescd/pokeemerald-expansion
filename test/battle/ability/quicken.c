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
