#ifndef GUARD_PARTY_EXPORT_H
#define GUARD_PARTY_EXPORT_H

#include "global.h"

// Exports the player's party as Pokemon Showdown / pokepaste syntax, which is
// also what src/data/trainers.party uses (minus the "=== TRAINER_X ===" header).
// The text is plain ASCII so it can be embedded verbatim in a QR code.

// v33 is the largest QR symbol that fits the 160px screen height once a
// 4-module quiet zone is added (149 + 8 = 157). These are its measured byte-mode
// capacities, not estimates.
//
// A party of six with maximum-length nicknames, the longest species, item,
// ability and move names, shiny markers and full EVs comes to 1872 bytes, which
// overflows level M. Rather than drop a Pokemon in that case, the screen falls
// back to level L, whose capacity no possible party can exceed. Error
// correction is only weakened for parties that would otherwise not fit at all.
#define PARTY_EXPORT_QR_CAPACITY_ECC_M 1628
#define PARTY_EXPORT_QR_CAPACITY_ECC_L 2068

// The ceiling used when building the text, i.e. the largest symbol available.
#define PARTY_EXPORT_QR_CAPACITY PARTY_EXPORT_QR_CAPACITY_ECC_L

// Room to build the full text before deciding how much of it fits.
#define PARTY_EXPORT_BUFFER_SIZE 2048

// Stats are held in Showdown's order, which differs from the game's internal
// order (the game puts Speed before the special stats).
enum PartyExportStat
{
    PARTY_EXPORT_HP,
    PARTY_EXPORT_ATK,
    PARTY_EXPORT_DEF,
    PARTY_EXPORT_SPATK,
    PARTY_EXPORT_SPDEF,
    PARTY_EXPORT_SPEED,
    PARTY_EXPORT_STAT_COUNT,
};

// One party slot, with every string still in the game's charmap encoding.
struct ExportMon
{
    const u8 *nickname;      // NULL when the mon has not been renamed
    const u8 *speciesName;
    const u8 *itemName;      // NULL when holding nothing
    const u8 *abilityName;
    const u8 *natureName;
    const u8 *moveNames[MAX_MON_MOVES];   // NULL for an empty slot
    u8 ivs[PARTY_EXPORT_STAT_COUNT];
    u8 evs[PARTY_EXPORT_STAT_COUNT];
    u8 level;
    char gender;             // 'M', 'F', or 0 when genderless
    bool8 isShiny;
};

// Converts a charmap string (EOS-terminated) to ASCII. Returns the number of
// characters written, always leaving dst NUL-terminated.
u32 CharmapToAscii(char *dst, u32 dstSize, const u8 *src);

// Formats mons into Showdown syntax. Returns the number of bytes written.
// If the text would exceed maxLen, it stops at the last complete mon and
// reports how many were included through monsWritten.
u32 FormatPartyExport(char *dst, u32 dstSize, u32 maxLen,
                      const struct ExportMon *mons, u32 monCount, u32 *monsWritten);

// Fills the buffer from gPlayerParty. Returns bytes written; monsWritten
// receives how many mons made it in (eggs and empty slots are skipped).
u32 BuildPartyExportText(char *dst, u32 dstSize, u32 maxLen, u32 *monsWritten);

#endif // GUARD_PARTY_EXPORT_H
