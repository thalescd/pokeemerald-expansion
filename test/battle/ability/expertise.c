#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Expertise boosts moves with 60 or less base power", s16 damage)
{
    u16 ability;
    PARAMETRIZE { ability = ABILITY_ILLUMINATE; }
    PARAMETRIZE { ability = ABILITY_EXPERTISE; }
    GIVEN {
        ASSUME(GetMovePower(MOVE_TACKLE) <= 60);
        ASSUME(GetMoveCategory(MOVE_TACKLE) == DAMAGE_CATEGORY_PHYSICAL);
        PLAYER(SPECIES_WOBBUFFET) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.5), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Expertise does not boost moves with more than 60 base power", s16 damage)
{
    u16 ability;
    PARAMETRIZE { ability = ABILITY_ILLUMINATE; }
    PARAMETRIZE { ability = ABILITY_EXPERTISE; }
    GIVEN {
        ASSUME(GetMovePower(MOVE_BODY_SLAM) > 60);
        ASSUME(GetMoveCategory(MOVE_BODY_SLAM) == DAMAGE_CATEGORY_PHYSICAL);
        PLAYER(SPECIES_WOBBUFFET) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_BODY_SLAM); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Expertise raises critical hit chance for moves with 60 or less base power")
{
    GIVEN {
        ASSUME(GetMovePower(MOVE_TACKLE) <= 60);
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_EXPERTISE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_FOCUS_ENERGY); }
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FOCUS_ENERGY, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        MESSAGE("A critical hit!");
    }
}
