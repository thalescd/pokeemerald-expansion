#ifndef GUARD_POKEVIAL_H
#define GUARD_POKEVIAL_H

#include "constants/pokevial.h"

// When TRUE, using the Pokévial skips the party-menu cutscene and instantly
// heals the party with just a text message.
#define POKEVIAL_SKIP_CUTSCENE FALSE

enum PokevialIconPercent
{
    POKEVIAL_ICON_PERCENT_0,
    POKEVIAL_ICON_PERCENT_10,
    POKEVIAL_ICON_PERCENT_20,
    POKEVIAL_ICON_PERCENT_30,
    POKEVIAL_ICON_PERCENT_40,
    POKEVIAL_ICON_PERCENT_50,
    POKEVIAL_ICON_PERCENT_60,
    POKEVIAL_ICON_PERCENT_70,
    POKEVIAL_ICON_PERCENT_80,
    POKEVIAL_ICON_PERCENT_90,
    POKEVIAL_ICON_PERCENT_100,
};

u32 PokevialGetDose(void);
u32 PokevialGetSize(void);

void PokevialSizeUp(u8 sizeIncrease);
void PokevialDoseUp(u8 doseIncrease);

void PokevialSizeDown(u8 sizeDecrease);
void PokevialDoseDown(u8 doseDecrease);

bool32 PokevialRefill(void);
const u32 *PokevialGetDoseIcon(void);
void Pokevial_HealParty(void);

#endif // GUARD_POKEVIAL_H
