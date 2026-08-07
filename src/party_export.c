#include "global.h"
#include "party_export.h"
#include "item.h"
#include "move.h"
#include "pokemon.h"
#include "string_util.h"
#include "constants/battle.h"
#include "constants/characters.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/species.h"
#include "data/charmap_ascii.h"

// Bounded text builder. Once it overflows it stops writing but keeps counting,
// so callers can tell how much room the full text would have needed.
struct TextBuf
{
    char *data;
    u32 size;
    u32 len;
};

static void PutChar(struct TextBuf *buf, char c)
{
    if (buf->len + 1 < buf->size)
        buf->data[buf->len] = c;
    buf->len++;
}

static void PutAscii(struct TextBuf *buf, const char *s)
{
    while (*s != '\0')
        PutChar(buf, *s++);
}

static void PutCharmap(struct TextBuf *buf, const u8 *s)
{
    while (*s != EOS)
    {
        const char *ascii = sCharmapToAscii[*s];
        // Characters with no ASCII spelling are dropped rather than replaced,
        // so a stray glyph in a nickname cannot corrupt the surrounding syntax.
        if (ascii[0] != '\0')
            PutAscii(buf, ascii);
        s++;
    }
}

static void PutNum(struct TextBuf *buf, u32 n)
{
    char digits[10];
    u32 i = 0;

    if (n == 0)
    {
        PutChar(buf, '0');
        return;
    }
    while (n != 0)
    {
        digits[i++] = '0' + (n % 10);
        n /= 10;
    }
    while (i != 0)
        PutChar(buf, digits[--i]);
}

u32 CharmapToAscii(char *dst, u32 dstSize, const u8 *src)
{
    struct TextBuf buf = { dst, dstSize, 0 };

    PutCharmap(&buf, src);
    if (dstSize != 0)
        dst[(buf.len < dstSize) ? buf.len : dstSize - 1] = '\0';
    return buf.len;
}

static const char *const sStatNames[PARTY_EXPORT_STAT_COUNT] =
{
    [PARTY_EXPORT_HP]    = "HP",
    [PARTY_EXPORT_ATK]   = "Atk",
    [PARTY_EXPORT_DEF]   = "Def",
    [PARTY_EXPORT_SPATK] = "SpA",
    [PARTY_EXPORT_SPDEF] = "SpD",
    [PARTY_EXPORT_SPEED] = "Spe",
};

// Writes "EVs: 4 HP / 252 Atk / ..." style lines, listing only nonzero stats.
static void PutStatLine(struct TextBuf *buf, const char *label, const u8 *stats, bool32 skipZero)
{
    bool32 first = TRUE;
    u32 i;

    PutAscii(buf, label);
    for (i = 0; i < PARTY_EXPORT_STAT_COUNT; i++)
    {
        if (skipZero && stats[i] == 0)
            continue;
        if (!first)
            PutAscii(buf, " / ");
        PutNum(buf, stats[i]);
        PutChar(buf, ' ');
        PutAscii(buf, sStatNames[i]);
        first = FALSE;
    }
    PutChar(buf, '\n');
}

static bool32 AnyNonzero(const u8 *stats)
{
    u32 i;

    for (i = 0; i < PARTY_EXPORT_STAT_COUNT; i++)
    {
        if (stats[i] != 0)
            return TRUE;
    }
    return FALSE;
}

static void FormatMon(struct TextBuf *buf, const struct ExportMon *mon)
{
    u32 i;

    // "Nickname (Species) (M) @ Item", with each part omitted when absent.
    if (mon->nickname != NULL)
    {
        PutCharmap(buf, mon->nickname);
        PutAscii(buf, " (");
        PutCharmap(buf, mon->speciesName);
        PutChar(buf, ')');
    }
    else
    {
        PutCharmap(buf, mon->speciesName);
    }

    if (mon->gender != 0)
    {
        PutAscii(buf, " (");
        PutChar(buf, mon->gender);
        PutChar(buf, ')');
    }

    if (mon->itemName != NULL)
    {
        PutAscii(buf, " @ ");
        PutCharmap(buf, mon->itemName);
    }
    PutChar(buf, '\n');

    PutAscii(buf, "Ability: ");
    PutCharmap(buf, mon->abilityName);
    PutChar(buf, '\n');

    PutAscii(buf, "Level: ");
    PutNum(buf, mon->level);
    PutChar(buf, '\n');

    if (mon->isShiny)
        PutAscii(buf, "Shiny: Yes\n");

    // Battle EVs are disabled in this romhack (B_EV_CAP_TYPE is EV_CAP_NO_GAIN),
    // but vitamins bypass that cap, so emit the line only when it is nonzero
    // rather than assuming it is always empty.
    if (AnyNonzero(mon->evs))
        PutStatLine(buf, "EVs: ", mon->evs, TRUE);

    PutCharmap(buf, mon->natureName);
    PutAscii(buf, " Nature\n");

    PutStatLine(buf, "IVs: ", mon->ivs, FALSE);

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (mon->moveNames[i] == NULL)
            continue;
        PutAscii(buf, "- ");
        PutCharmap(buf, mon->moveNames[i]);
        PutChar(buf, '\n');
    }
}

u32 FormatPartyExport(char *dst, u32 dstSize, u32 maxLen,
                      const struct ExportMon *mons, u32 monCount, u32 *monsWritten)
{
    struct TextBuf buf = { dst, dstSize, 0 };
    u32 committed = 0;
    u32 written = 0;
    u32 i;

    for (i = 0; i < monCount; i++)
    {
        if (i != 0)
            PutChar(&buf, '\n');    // blank line between mons
        FormatMon(&buf, mons + i);

        // Only keep this mon if all of it fit; a half-written mon would make
        // the whole export unparseable.
        if (buf.len > maxLen || buf.len >= dstSize)
        {
            buf.len = committed;
            break;
        }
        committed = buf.len;
        written++;
    }

    buf.len = committed;
    if (dstSize != 0)
        dst[buf.len] = '\0';
    if (monsWritten != NULL)
        *monsWritten = written;
    return buf.len;
}

u32 BuildPartyExportText(char *dst, u32 dstSize, u32 maxLen, u32 *monsWritten)
{
    struct ExportMon mons[PARTY_SIZE];
    u8 nicknames[PARTY_SIZE][POKEMON_NAME_LENGTH + 1];
    u32 count = 0;
    u32 i;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        struct ExportMon *out = &mons[count];
        enum Species species = GetMonData(mon, MON_DATA_SPECIES, NULL);
        enum Item item;
        u32 j;

        if (species == SPECIES_NONE || GetMonData(mon, MON_DATA_IS_EGG, NULL))
            continue;

        out->speciesName = GetSpeciesName(species);

        GetMonData(mon, MON_DATA_NICKNAME, nicknames[count]);
        out->nickname = StringCompare(nicknames[count], out->speciesName) != 0
                      ? nicknames[count] : NULL;

        item = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
        out->itemName = (item != ITEM_NONE) ? GetItemName(item) : NULL;

        out->abilityName = gAbilitiesInfo[GetAbilityBySpecies(species,
                               GetMonData(mon, MON_DATA_ABILITY_NUM, NULL))].name;
        out->natureName = gNaturesInfo[GetNatureFromPersonality(
                               GetMonData(mon, MON_DATA_PERSONALITY, NULL))].name;

        out->level = GetMonData(mon, MON_DATA_LEVEL, NULL);
        out->isShiny = GetMonData(mon, MON_DATA_IS_SHINY, NULL);

        switch (GetMonGender(mon))
        {
        case MON_MALE:   out->gender = 'M'; break;
        case MON_FEMALE: out->gender = 'F'; break;
        default:         out->gender = 0;   break;
        }

        // The game orders stats HP/Atk/Def/Speed/SpAtk/SpDef; Showdown puts
        // Speed last, so these are remapped rather than copied in sequence.
        out->ivs[PARTY_EXPORT_HP]    = GetMonData(mon, MON_DATA_HP_IV, NULL);
        out->ivs[PARTY_EXPORT_ATK]   = GetMonData(mon, MON_DATA_ATK_IV, NULL);
        out->ivs[PARTY_EXPORT_DEF]   = GetMonData(mon, MON_DATA_DEF_IV, NULL);
        out->ivs[PARTY_EXPORT_SPATK] = GetMonData(mon, MON_DATA_SPATK_IV, NULL);
        out->ivs[PARTY_EXPORT_SPDEF] = GetMonData(mon, MON_DATA_SPDEF_IV, NULL);
        out->ivs[PARTY_EXPORT_SPEED] = GetMonData(mon, MON_DATA_SPEED_IV, NULL);

        out->evs[PARTY_EXPORT_HP]    = GetMonData(mon, MON_DATA_HP_EV, NULL);
        out->evs[PARTY_EXPORT_ATK]   = GetMonData(mon, MON_DATA_ATK_EV, NULL);
        out->evs[PARTY_EXPORT_DEF]   = GetMonData(mon, MON_DATA_DEF_EV, NULL);
        out->evs[PARTY_EXPORT_SPATK] = GetMonData(mon, MON_DATA_SPATK_EV, NULL);
        out->evs[PARTY_EXPORT_SPDEF] = GetMonData(mon, MON_DATA_SPDEF_EV, NULL);
        out->evs[PARTY_EXPORT_SPEED] = GetMonData(mon, MON_DATA_SPEED_EV, NULL);

        for (j = 0; j < MAX_MON_MOVES; j++)
        {
            enum Move move = GetMonData(mon, MON_DATA_MOVE1 + j, NULL);
            out->moveNames[j] = (move != MOVE_NONE) ? GetMoveName(move) : NULL;
        }
        count++;
    }

    return FormatPartyExport(dst, dstSize, maxLen, mons, count, monsWritten);
}
