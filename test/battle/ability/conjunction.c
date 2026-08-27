#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Conjunction activates when targeted by ground type moves")
{
    GIVEN {
        ASSUME(GetMoveType(MOVE_MUD_SLAP) == TYPE_GROUND);
        PLAYER(SPECIES_LUNATONE) { Ability(ABILITY_CONJUNCTION); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_MUD_SLAP); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_CONJUNCTION);
        MESSAGE("It doesn't affect Lunatone…");
    }
}

SINGLE_BATTLE_TEST("Conjunction does not activate if attacked by an opponent with Mold Breaker")
{
    GIVEN {
        ASSUME(GetMoveType(MOVE_MUD_SLAP) == TYPE_GROUND);
        PLAYER(SPECIES_LUNATONE) { Ability(ABILITY_CONJUNCTION); }
        OPPONENT(SPECIES_TINKATON) { Ability(ABILITY_MOLD_BREAKER); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_MUD_SLAP); }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_CONJUNCTION);
        }
        HP_BAR(player);
    }
}

DOUBLE_BATTLE_TEST("Conjunction reduces damage by 0.75 only if the ally also has Conjunction", s16 damage)
{
    enum Species partner;
    PARAMETRIZE { partner = SPECIES_WOBBUFFET; }
    PARAMETRIZE { partner = SPECIES_SOLROCK; }
    GIVEN {
        ASSUME(GetMoveType(MOVE_DARK_PULSE) == TYPE_DARK);
        ASSUME(gTypeEffectivenessTable[TYPE_DARK][TYPE_PSYCHIC] > UQ_4_12(1.0));
        PLAYER(SPECIES_LUNATONE) { Ability(ABILITY_CONJUNCTION); }
        PLAYER(partner);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_DARK_PULSE, target: playerLeft); }
    } SCENE {
        HP_BAR(playerLeft, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(0.75), results[1].damage);
    }
}

DOUBLE_BATTLE_TEST("Conjunction does not reduce damage from a Mold Breaker attacker", s16 damage)
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_MOLD_BREAKER; }
    PARAMETRIZE { ability = ABILITY_OWN_TEMPO; }
    GIVEN {
        ASSUME(GetMoveType(MOVE_DARK_PULSE) == TYPE_DARK);
        PLAYER(SPECIES_LUNATONE) { Ability(ABILITY_CONJUNCTION); }
        PLAYER(SPECIES_SOLROCK) { Ability(ABILITY_CONJUNCTION); }
        OPPONENT(SPECIES_TINKATON) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_DARK_PULSE, target: playerLeft); }
    } SCENE {
        HP_BAR(playerLeft, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(0.75), results[1].damage);
    }
}
