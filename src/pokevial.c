#include "global.h"
#include "pokevial.h"
#include "graphics.h"
#include "pokemon.h"
#include "constants/battle.h"

static void PokevialInit(void)
{
    if (gSaveBlock1Ptr->pokevial.size < VIAL_MIN_SIZE)
    {
        gSaveBlock1Ptr->pokevial.size = VIAL_MIN_SIZE;
        gSaveBlock1Ptr->pokevial.dose = VIAL_MIN_SIZE;
    }
}

u32 PokevialGetDose(void)
{
    PokevialInit();
    return gSaveBlock1Ptr->pokevial.dose;
}

u32 PokevialGetSize(void)
{
    PokevialInit();
    return gSaveBlock1Ptr->pokevial.size;
}

void PokevialSizeUp(u8 sizeIncrease)
{
    u32 newSize = PokevialGetSize() + sizeIncrease;
    gSaveBlock1Ptr->pokevial.size = (newSize > VIAL_MAX_SIZE) ? VIAL_MAX_SIZE : newSize;
}

void PokevialDoseUp(u8 doseIncrease)
{
    u32 newDose = PokevialGetDose() + doseIncrease;
    u32 size = PokevialGetSize();
    gSaveBlock1Ptr->pokevial.dose = (newDose > size) ? size : newDose;
}

void PokevialSizeDown(u8 sizeDecrease)
{
    s32 newSize = (s32)PokevialGetSize() - sizeDecrease;
    gSaveBlock1Ptr->pokevial.size = (newSize < VIAL_MIN_SIZE) ? VIAL_MIN_SIZE : newSize;
    // Shrinking the vial can leave more doses stored than the new size allows.
    PokevialDoseUp(0);
}

void PokevialDoseDown(u8 doseDecrease)
{
    gSaveBlock1Ptr->pokevial.dose = (doseDecrease > PokevialGetDose()) ? EMPTY_VIAL : gSaveBlock1Ptr->pokevial.dose - doseDecrease;
}

bool32 PokevialRefill(void)
{
    if (PokevialGetDose() == PokevialGetSize())
        return FALSE;

    gSaveBlock1Ptr->pokevial.dose = gSaveBlock1Ptr->pokevial.size;
    return TRUE;
}

static const u32 *const sPokevialIcons[VIAL_NUM_STATES] =
{
    gItemIcon_Pokevial0,
    gItemIcon_Pokevial1,
    gItemIcon_Pokevial2,
    gItemIcon_Pokevial3,
    gItemIcon_Pokevial4,
    gItemIcon_Pokevial5,
    gItemIcon_Pokevial6,
    gItemIcon_Pokevial7,
    gItemIcon_Pokevial8,
    gItemIcon_Pokevial9,
    gItemIcon_Pokevial,
};

const u32 *PokevialGetDoseIcon(void)
{
    u32 dose = PokevialGetDose();
    u32 size = PokevialGetSize();
    u32 percentTier;

    if (dose == EMPTY_VIAL)
        percentTier = POKEVIAL_ICON_PERCENT_0;
    else if (dose == size)
        percentTier = POKEVIAL_ICON_PERCENT_100;
    else
        percentTier = dose * 10 / size;

    // Round up so a container with any doses left never displays as empty.
    if (percentTier == POKEVIAL_ICON_PERCENT_0 && dose > EMPTY_VIAL)
        percentTier = POKEVIAL_ICON_PERCENT_10;

    return sPokevialIcons[percentTier];
}

// Used when POKEVIAL_SKIP_CUTSCENE is enabled, to heal the whole party without
// going through the party-menu slot-by-slot animation.
void Pokevial_HealParty(void)
{
    u32 i;

    for (i = 0; i < gPartiesCount[B_TRAINER_PLAYER]; i++)
        HealPokemon(&gParties[B_TRAINER_PLAYER][i]);
}
