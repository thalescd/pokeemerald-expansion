#include "global.h"
#include "test/battle.h"

TO_DO_BATTLE_TEST("Struggle deals recoil 1/4 of damage dealt (Gen 2-3)")

SINGLE_BATTLE_TEST("Struggle deals recoil 1/4 of user's hp (Gen 4+)")
{
    ASSUME(GetMoveEffect(MOVE_STRUGGLE) == EFFECT_STRUGGLE);

    s16 recoil;
    u32 atkStat = 0;
    u32 hpStat = 0;

    PARAMETRIZE { atkStat = 100; hpStat = 200; }
    PARAMETRIZE { atkStat = 50; hpStat = 200; }
    PARAMETRIZE { atkStat = 100; hpStat = 300; }

    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(hpStat); HP(hpStat); Attack(atkStat); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_STRUGGLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STRUGGLE, player);
        HP_BAR(player, captureDamage: &recoil);
    } THEN {
        EXPECT_MUL_EQ(hpStat, Q_4_12(0.25), recoil);
    }
}

SINGLE_BATTLE_TEST("Struggle can hit ghost types")
{
    ASSUME(GetSpeciesType(SPECIES_DRIFBLIM, 0) == TYPE_GHOST);

    s16 damage;

    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_DRIFBLIM);
    } WHEN {
        TURN { MOVE(player, MOVE_STRUGGLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STRUGGLE, player);
        HP_BAR(opponent, captureDamage: &damage);
    } THEN {
        EXPECT_NE(0, damage);
    }
}

SINGLE_BATTLE_TEST("Struggle does not receive normal-type STAB", s16 damage)
{
    // Measured by changing the attacker's type rather than by comparing against a control move of
    // equal power: no move other than Struggle itself is Normal, physical and 50 base power here.
    // Attack is forced equal so the attacker's type is the only difference between the two runs.
    u32 species;
    PARAMETRIZE { species = SPECIES_ZANGOOSE; } // Shares Struggle's type, so it would get the STAB
    PARAMETRIZE { species = SPECIES_SEVIPER; }  // Does not share it

    GIVEN {
        ASSUME(GetSpeciesType(SPECIES_ZANGOOSE, 0) == GetMoveType(MOVE_STRUGGLE));
        ASSUME(GetSpeciesType(SPECIES_SEVIPER, 0) != GetMoveType(MOVE_STRUGGLE));
        ASSUME(GetSpeciesType(SPECIES_SEVIPER, 1) != GetMoveType(MOVE_STRUGGLE));
        PLAYER(species) { Attack(100); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_STRUGGLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STRUGGLE, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Struggle recoil is subject to standard rounding (Gen 5+)")
{
    ASSUME(GetMoveEffect(MOVE_STRUGGLE) == EFFECT_STRUGGLE);

    s16 recoil;
    u32 hpStat = 0;

    PARAMETRIZE { hpStat = 200; }
    PARAMETRIZE { hpStat = 201; }
    PARAMETRIZE { hpStat = 202; }
    PARAMETRIZE { hpStat = 203; }

    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(hpStat); HP(hpStat); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_STRUGGLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STRUGGLE, player);
        HP_BAR(player, captureDamage: &recoil);
    } THEN {
        switch (hpStat)
        {
            case 200:
                EXPECT_EQ(player->hp, 150);
                break;
            case 201:
            case 202:
                EXPECT_EQ(player->hp, 151);
                break;
            case 203:
                EXPECT_EQ(player->hp, 152);
                break;
        }
    }
}
