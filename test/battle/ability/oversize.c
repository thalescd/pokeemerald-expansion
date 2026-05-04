#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Oversize doubles the power of Heat Crash", s16 damage)
{
    u16 ability;
    PARAMETRIZE { ability = ABILITY_ILLUMINATE; }
    PARAMETRIZE { ability = ABILITY_OVERSIZE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_HEAT_CRASH) == EFFECT_HEAT_CRASH);
        PLAYER(SPECIES_SNORLAX) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_HEAT_CRASH); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Oversize doubles the power of Heavy Slam", s16 damage)
{
    u16 ability;
    PARAMETRIZE { ability = ABILITY_ILLUMINATE; }
    PARAMETRIZE { ability = ABILITY_OVERSIZE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_HEAVY_SLAM) == EFFECT_HEAT_CRASH);
        PLAYER(SPECIES_SNORLAX) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_HEAVY_SLAM); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Oversize does not boost moves unrelated to weight", s16 damage)
{
    u16 ability;
    PARAMETRIZE { ability = ABILITY_ILLUMINATE; }
    PARAMETRIZE { ability = ABILITY_OVERSIZE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_TACKLE) != EFFECT_HEAT_CRASH);
        PLAYER(SPECIES_SNORLAX) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}
