#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(GetMoveEffect(MOVE_REFORGE) == EFFECT_RESTORE_HP);
    ASSUME(MoveHasAdditionalEffect(MOVE_REFORGE, MOVE_EFFECT_SUPPRESS_TYPE));
    ASSUME(GetMoveType(MOVE_REFORGE) == TYPE_STEEL);
    ASSUME(GetMoveType(MOVE_EARTHQUAKE) == TYPE_GROUND);
    ASSUME(GetMoveType(MOVE_STEEL_WING) == TYPE_STEEL);
}

SINGLE_BATTLE_TEST("Reforge fails when user is at full HP")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_REFORGE); }
    } SCENE {
        MESSAGE("Wobbuffet's HP is full!");
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_REFORGE, player);
            HP_BAR(player);
        }
    }
}

SINGLE_BATTLE_TEST("Reforge suppresses the user's Steel-typing this turn, then restores it at the end of the turn")
{
    GIVEN {
        ASSUME(GetSpeciesType(SPECIES_SKARMORY, 0) == TYPE_STEEL);
        ASSUME(GetSpeciesType(SPECIES_SKARMORY, 1) == TYPE_FLYING);
        PLAYER(SPECIES_SKARMORY) { HP(50); MaxHP(100); Ability(ABILITY_STURDY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_REFORGE); MOVE(opponent, MOVE_ICE_BEAM); }
        TURN { MOVE(opponent, MOVE_ICE_BEAM); }
    } SCENE {
        // Turn 1: Ice Beam hits Flying only (2x) when Reforged
        MESSAGE("Skarmory used Reforge!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_REFORGE, player);
        MESSAGE("Skarmory's HP was restored.");
        MESSAGE("The opposing Wobbuffet used Ice Beam!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ICE_BEAM, opponent);
        MESSAGE("It's super effective!");
        // Turn 2: Steel is back, Ice Beam is neutral against Steel/Flying (2x * 0.5x)
        MESSAGE("The opposing Wobbuffet used Ice Beam!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ICE_BEAM, opponent);
        NOT MESSAGE("It's super effective!");
    }
}

SINGLE_BATTLE_TEST("Reforge, if used by a Steel/Steel type, treats the user as a Normal-type (or Typeless in Gen. 4) until the end of the turn")
{
    GIVEN {
        ASSUME(GetSpeciesType(SPECIES_REGISTEEL, 0) == TYPE_STEEL);
        ASSUME(GetSpeciesType(SPECIES_REGISTEEL, 1) == TYPE_STEEL);
        PLAYER(SPECIES_REGISTEEL) { HP(200); MaxHP(400); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_REFORGE); MOVE(opponent, MOVE_EARTHQUAKE); }
        TURN { MOVE(opponent, MOVE_EARTHQUAKE); }
    } SCENE {
        MESSAGE("Registeel used Reforge!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_REFORGE, player);
        MESSAGE("Registeel's HP was restored.");
        // Turn 1: Earthquake is neutral against Normal/Typeless
        MESSAGE("The opposing Wobbuffet used Earthquake!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, opponent);
        NOT MESSAGE("It's super effective!");
        // Turn 2: Steel is back, Earthquake is super effective
        MESSAGE("The opposing Wobbuffet used Earthquake!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, opponent);
        MESSAGE("It's super effective!");
    }
}
