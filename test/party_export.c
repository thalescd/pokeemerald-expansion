#include "global.h"
#include "battle.h"
#include "party_export.h"
#include "pokemon.h"
#include "qr_code_screen.h"
#include "qrcodegen.h"
#include "test/test.h"
#include "constants/battle.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/abilities.h"
#include "constants/species.h"

// Exercises the half of party_export.c that reads real Pokemon through
// GetMonData. The formatting half is covered by host tests; what matters here
// is that the game's own data lands in the right fields -- in particular that
// the stats are remapped, since the game orders them HP/Atk/Def/Speed/SpA/SpD
// while Showdown puts Speed last.

static bool32 TextStartsWith(const char *actual, const char *prefix)
{
    while (*prefix != '\0')
    {
        if (*actual != *prefix)
            return FALSE;
        actual++;
        prefix++;
    }
    return TRUE;
}

static bool32 TextEquals(const char *actual, const char *expected)
{
    while (*expected != '\0')
    {
        if (*actual != *expected)
            return FALSE;
        actual++;
        expected++;
    }
    return *actual == '\0';
}

// Shininess is derived from personality XOR OT ID. Personality is pinned to 0
// so that nature, gender and ability stay predictable, which makes an OT ID of
// 0 shiny -- use a non-zero one unless a test is specifically about that.
#define OTID_PLAIN 0x0100
#define OTID_SHINY 0x0000

// Builds a mon whose every exported field is pinned to a known value.
static void SetUpMonWithOtId(u32 slot, enum Species species, u32 level, u32 otId)
{
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][slot];
    static const u8 ivs[] = { 1, 2, 3, 4, 5, 6 };   // HP, Atk, Def, Speed, SpA, SpD
    static const u8 ivFields[] = {
        MON_DATA_HP_IV, MON_DATA_ATK_IV, MON_DATA_DEF_IV,
        MON_DATA_SPEED_IV, MON_DATA_SPATK_IV, MON_DATA_SPDEF_IV,
    };
    u32 i;
    u32 item = ITEM_LEFTOVERS;
    u32 move;

    CreateMon(mon, species, level, 0, OTID_STRUCT_PRESET(otId));

    for (i = 0; i < ARRAY_COUNT(ivFields); i++)
        SetMonData(mon, ivFields[i], &ivs[i]);

    SetMonData(mon, MON_DATA_HELD_ITEM, &item);

    move = MOVE_TACKLE;   SetMonData(mon, MON_DATA_MOVE1, &move);
    move = MOVE_SPLASH;   SetMonData(mon, MON_DATA_MOVE2, &move);
    move = MOVE_NONE;     SetMonData(mon, MON_DATA_MOVE3, &move);
                          SetMonData(mon, MON_DATA_MOVE4, &move);
}

static void SetUpMon(u32 slot, enum Species species, u32 level)
{
    SetUpMonWithOtId(slot, species, level, OTID_PLAIN);
}

TEST("Party export writes one mon in Showdown syntax")
{
    char buffer[PARTY_EXPORT_BUFFER_SIZE];
    u32 written, len;

    ZeroPlayerPartyMons();
    SetUpMon(0, SPECIES_WOBBUFFET, 42);

    len = BuildPartyExportText(buffer, sizeof(buffer), PARTY_EXPORT_QR_CAPACITY, &written);

    EXPECT_EQ(written, 1);
    EXPECT_GT(len, 0);
    // Speed must come out as 4 (the game's 4th slot) in the last position, not
    // in the middle where the game keeps it.
    EXPECT(TextEquals(buffer,
        "Wobbuffet (F) @ Leftovers\n"
        "Ability: Shadow Tag\n"
        "Level: 42\n"
        "Hardy Nature\n"
        "IVs: 1 HP / 2 Atk / 3 Def / 5 SpA / 6 SpD / 4 Spe\n"
        "- Tackle\n"
        "- Splash\n"));
}

TEST("Party export separates mons with a blank line")
{
    char buffer[PARTY_EXPORT_BUFFER_SIZE];
    u32 written, len;

    ZeroPlayerPartyMons();
    SetUpMon(0, SPECIES_WOBBUFFET, 42);
    SetUpMon(1, SPECIES_WOBBUFFET, 42);

    len = BuildPartyExportText(buffer, sizeof(buffer), PARTY_EXPORT_QR_CAPACITY, &written);

    EXPECT_EQ(written, 2);
    EXPECT_GT(len, 0);
    EXPECT(TextEquals(buffer,
        "Wobbuffet (F) @ Leftovers\n"
        "Ability: Shadow Tag\n"
        "Level: 42\n"
        "Hardy Nature\n"
        "IVs: 1 HP / 2 Atk / 3 Def / 5 SpA / 6 SpD / 4 Spe\n"
        "- Tackle\n"
        "- Splash\n"
        "\n"
        "Wobbuffet (F) @ Leftovers\n"
        "Ability: Shadow Tag\n"
        "Level: 42\n"
        "Hardy Nature\n"
        "IVs: 1 HP / 2 Atk / 3 Def / 5 SpA / 6 SpD / 4 Spe\n"
        "- Tackle\n"
        "- Splash\n"));
}

TEST("Party export marks shiny mons")
{
    char buffer[PARTY_EXPORT_BUFFER_SIZE];
    u32 written;

    ZeroPlayerPartyMons();
    SetUpMonWithOtId(0, SPECIES_WOBBUFFET, 42, OTID_SHINY);

    BuildPartyExportText(buffer, sizeof(buffer), PARTY_EXPORT_QR_CAPACITY, &written);

    EXPECT_EQ(written, 1);
    EXPECT(TextEquals(buffer,
        "Wobbuffet (F) @ Leftovers\n"
        "Ability: Shadow Tag\n"
        "Level: 42\n"
        "Shiny: Yes\n"
        "Hardy Nature\n"
        "IVs: 1 HP / 2 Atk / 3 Def / 5 SpA / 6 SpD / 4 Spe\n"
        "- Tackle\n"
        "- Splash\n"));
}

TEST("Party export skips eggs and empty slots")
{
    char buffer[PARTY_EXPORT_BUFFER_SIZE];
    u32 written;
    u32 isEgg = TRUE;

    ZeroPlayerPartyMons();
    SetUpMon(0, SPECIES_WOBBUFFET, 42);
    SetUpMon(1, SPECIES_WOBBUFFET, 42);
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_IS_EGG, &isEgg);

    BuildPartyExportText(buffer, sizeof(buffer), PARTY_EXPORT_QR_CAPACITY, &written);

    EXPECT_EQ(written, 1);
}

TEST("Party export produces nothing for an empty party")
{
    char buffer[PARTY_EXPORT_BUFFER_SIZE];
    u32 written, len;

    ZeroPlayerPartyMons();

    len = BuildPartyExportText(buffer, sizeof(buffer), PARTY_EXPORT_QR_CAPACITY, &written);

    EXPECT_EQ(written, 0);
    EXPECT_EQ(len, 0);
}

TEST("Party export truncates on a mon boundary rather than mid-mon")
{
    char buffer[PARTY_EXPORT_BUFFER_SIZE];
    u32 written, len;
    u32 i;

    ZeroPlayerPartyMons();
    for (i = 0; i < PARTY_SIZE; i++)
        SetUpMon(i, SPECIES_WOBBUFFET, 42);

    // Room for two mons but not three.
    len = BuildPartyExportText(buffer, sizeof(buffer), 300, &written);

    EXPECT_EQ(written, 2);
    EXPECT_LE(len, 300);
    EXPECT_EQ(buffer[len - 1], '\n');
}

TEST("Party export keeps a full party inside the QR capacity")
{
    char buffer[PARTY_EXPORT_BUFFER_SIZE];
    u32 written, len;
    u32 i;

    ZeroPlayerPartyMons();
    for (i = 0; i < PARTY_SIZE; i++)
        SetUpMon(i, SPECIES_WOBBUFFET, 100);

    len = BuildPartyExportText(buffer, sizeof(buffer), PARTY_EXPORT_QR_CAPACITY, &written);

    EXPECT_EQ(written, PARTY_SIZE);
    EXPECT_LE(len, PARTY_EXPORT_QR_CAPACITY);
}

// Too large for IWRAM, which the test build is already close to filling.
static EWRAM_DATA char sBuffer[PARTY_EXPORT_BUFFER_SIZE] = {0};
static EWRAM_DATA u8 sQrCode[qrcodegen_BUFFER_LEN_FOR_VERSION(33)] = {0};
static EWRAM_DATA u8 sQrTemp[qrcodegen_BUFFER_LEN_FOR_VERSION(33)] = {0};

// The encoder was validated on the host against an independent decoder, but
// that was x86. This runs it on the real ARM target to catch anything
// alignment- or width-related, and reports how long it takes: the screen has
// to show a "generating" message if this is slow.
TEST("Party export encodes into a QR symbol that fits the screen")
{
    char *buffer = sBuffer;
    u8 *qrcode = sQrCode;
    u8 *temp = sQrTemp;
    struct Benchmark encoding;
    u32 written, len, size, version;
    bool32 ok = FALSE;   // BENCHMARK is a loop, so the compiler cannot see the assignment
    u32 i;

    ZeroPlayerPartyMons();
    for (i = 0; i < PARTY_SIZE; i++)
        SetUpMon(i, SPECIES_WOBBUFFET, 100);

    len = BuildPartyExportText(buffer, PARTY_EXPORT_BUFFER_SIZE, PARTY_EXPORT_QR_CAPACITY, &written);
    EXPECT_EQ(written, PARTY_SIZE);

    BENCHMARK(&encoding)
    {
        ok = qrcodegen_encodeText(buffer, temp, qrcode, qrcodegen_Ecc_MEDIUM,
                                  1, 33, qrcodegen_Mask_AUTO, TRUE);
    }
    EXPECT(ok);

    size = qrcodegen_getSize(qrcode);
    version = (size - 17) / 4;
    Test_MgbaPrintf("%d bytes -> v%d, %dx%d modules, %d ticks",
                    len, version, size, size, encoding.ticks);

    // The symbol plus a 4-module quiet zone has to clear the 160px screen.
    EXPECT_LE(size + 8, 160);
}

// Reads a 4bpp pixel back out of a tile buffer, mirroring PlotPixel.
static u32 ReadTilePixel(const u8 *tiles, u32 tilesWide, u32 x, u32 y)
{
    u8 byte = tiles[((y / TILE_HEIGHT) * tilesWide + (x / TILE_WIDTH)) * TILE_SIZE_4BPP
                    + (y % TILE_HEIGHT) * 4
                    + (x % TILE_WIDTH) / 2];

    return (x & 1) ? (byte >> 4) : (byte & 0x0F);
}

#define QR_TILES_WIDE 20
#define QR_CANVAS_PX  (QR_TILES_WIDE * TILE_WIDTH)

static EWRAM_DATA u8 sTileBuffer[QR_TILES_WIDE * QR_TILES_WIDE * TILE_SIZE_4BPP] = {0};

// The screen draws the symbol by poking nibbles into tile data directly, so a
// wrong nibble order or tile stride would silently produce a mirrored or
// scrambled QR. This checks every pixel of the canvas against the encoder.
TEST("QR code draws one module per pixel with a quiet zone")
{
    u8 *tiles = sTileBuffer;
    u32 size, origin, x, y, dark, mismatches = 0, quietZoneViolations = 0;
    u32 fullParty = 0, i;

    // A tiny symbol sits well inside the canvas; a real party nearly fills it,
    // which is the case where a stride mistake would actually run off the edge.
    PARAMETRIZE { fullParty = 0; }
    PARAMETRIZE { fullParty = 1; }

    memset(tiles, 0, QR_TILES_WIDE * QR_TILES_WIDE * TILE_SIZE_4BPP);

    if (fullParty)
    {
        u32 written;
        ZeroPlayerPartyMons();
        for (i = 0; i < PARTY_SIZE; i++)
            SetUpMon(i, SPECIES_WOBBUFFET, 100);
        BuildPartyExportText(sBuffer, PARTY_EXPORT_BUFFER_SIZE,
                             PARTY_EXPORT_QR_CAPACITY, &written);
    }
    else
    {
        // Plain ASCII, so this is memcpy rather than StringCopy (which walks
        // charmap strings looking for EOS).
        memcpy(sBuffer, "Wobbuffet @ Leftovers", sizeof("Wobbuffet @ Leftovers"));
    }

    EXPECT(qrcodegen_encodeText(sBuffer, sQrTemp, sQrCode,
                                qrcodegen_Ecc_MEDIUM, 1, 33, qrcodegen_Mask_AUTO, TRUE));

    QrCode_DrawToTileBuffer(tiles, QR_TILES_WIDE, sQrCode);

    size = qrcodegen_getSize(sQrCode);
    origin = (QR_CANVAS_PX - size) / 2;

    for (y = 0; y < QR_CANVAS_PX; y++)
    {
        for (x = 0; x < QR_CANVAS_PX; x++)
        {
            bool32 insideSymbol = (x >= origin && x < origin + size
                                && y >= origin && y < origin + size);
            u32 pixel = ReadTilePixel(tiles, QR_TILES_WIDE, x, y);

            if (insideSymbol)
            {
                dark = qrcodegen_getModule(sQrCode, x - origin, y - origin);
                if (dark != (pixel != 0))
                    mismatches++;
            }
            else if (pixel != 0)
            {
                quietZoneViolations++;   // nothing may be drawn outside the symbol
            }
        }
    }

    Test_MgbaPrintf("v%d, %dx%d at origin %d", (size - 17) / 4, size, size, origin);
    EXPECT_EQ(mismatches, 0);
    EXPECT_EQ(quietZoneViolations, 0);
    // Decoders need at least 4 light modules around the symbol.
    EXPECT_GE(origin, 4);
}

// Returns the ASCII length of a charmap string, since accents fold to one
// character but the gender signs expand to two ("-M").
static u32 AsciiLengthOf(const u8 *charmapStr)
{
    char scratch[64];
    return CharmapToAscii(scratch, sizeof(scratch), charmapStr);
}

// The truncation path is graceful, but it is worth knowing whether real data
// can ever reach it. This scans every name the exporter can emit, builds the
// worst mon those names allow, and checks six of them still fit.
TEST("Party export cannot overflow the QR capacity with the longest names")
{
    u32 i;
    u32 maxSpecies = 0, maxMove = 0, maxItem = 0, maxAbility = 0, maxNature = 0;
    u32 perMon, worstCase;

    // This romhack disables a chunk of the dex, and GetSpeciesName asserts on
    // those, so only reachable species count towards the worst case.
    for (i = 1; i < NUM_SPECIES; i++)
    {
        u32 len;
        if (!IsSpeciesEnabled(i))
            continue;
        len = AsciiLengthOf(GetSpeciesExportName(i));
        if (len > maxSpecies)
            maxSpecies = len;
    }
    for (i = 1; i < MOVES_COUNT; i++)
    {
        u32 len = AsciiLengthOf(GetMoveName(i));
        if (len > maxMove)
            maxMove = len;
    }
    for (i = 1; i < ITEMS_COUNT; i++)
    {
        u32 len = AsciiLengthOf(GetItemName(i));
        if (len > maxItem)
            maxItem = len;
    }
    for (i = 1; i < ABILITIES_COUNT; i++)
    {
        u32 len = AsciiLengthOf(gAbilitiesInfo[i].name);
        if (len > maxAbility)
            maxAbility = len;
    }
    for (i = 0; i < NUM_NATURES; i++)
    {
        u32 len = AsciiLengthOf(gNaturesInfo[i].name);
        if (len > maxNature)
            maxNature = len;
    }

    // Mirrors FormatMon, taking the longest branch at every choice.
    perMon = (POKEMON_NAME_LENGTH + 2 + maxSpecies + 1)  // "Nick (Species)"
           + 4                                           // " (M)"
           + (3 + maxItem) + 1                           // " @ Item\n"
           + (9 + maxAbility + 1)                        // "Ability: X\n"
           + sizeof("Level: 100\n") - 1
           + sizeof("Shiny: Yes\n") - 1
           + sizeof("EVs: 252 HP / 252 Atk / 252 Def / 252 SpA / 252 SpD / 252 Spe\n") - 1
           + (maxNature + sizeof(" Nature\n") - 1)
           + sizeof("IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe\n") - 1
           + MAX_MON_MOVES * (2 + maxMove + 1)           // "- Move\n"
           + 1;                                          // blank line between mons

    worstCase = perMon * PARTY_SIZE;

    Test_MgbaPrintf("longest names: species %d, move %d, item %d, ability %d, nature %d",
                    maxSpecies, maxMove, maxItem, maxAbility, maxNature);
    Test_MgbaPrintf("worst mon %d bytes, party %d bytes; ECC M holds %d, ECC L holds %d",
                    perMon, worstCase,
                    PARTY_EXPORT_QR_CAPACITY_ECC_M, PARTY_EXPORT_QR_CAPACITY_ECC_L);

    // The worst possible party is allowed to exceed level M -- the screen drops
    // to level L for those. What must never happen is exceeding level L, which
    // is the point where a Pokemon would be cut from the export.
    EXPECT_LE(worstCase, PARTY_EXPORT_QR_CAPACITY_ECC_L);
}

// speciesName is capped at POKEMON_NAME_LENGTH, so all four Urshifu forms are
// spelled "Urshifu" and a bare export would be read back as Single Strike --
// Dark instead of Water. Forms must carry their Showdown-style name.
TEST("Party export distinguishes forms that share a display name")
{
    char buffer[PARTY_EXPORT_BUFFER_SIZE];
    u32 written;
    u32 species = SPECIES_NONE;

    PARAMETRIZE { species = SPECIES_URSHIFU; }
    PARAMETRIZE { species = SPECIES_URSHIFU_RAPID_STRIKE; }
    PARAMETRIZE { species = SPECIES_ROTOM; }
    PARAMETRIZE { species = SPECIES_ROTOM_WASH; }

    if (!IsSpeciesEnabled(species))
        return;   // not ASSUME: a disabled form is not a failure

    ZeroPlayerPartyMons();
    SetUpMon(0, species, 50);
    BuildPartyExportText(buffer, sizeof(buffer), PARTY_EXPORT_QR_CAPACITY, &written);

    EXPECT_EQ(written, 1);
    Test_MgbaPrintf("species %d exports as: %s", species, buffer);

    // The base forms keep the bare name; the alternate forms must not.
    if (species == SPECIES_URSHIFU)
        EXPECT(TextStartsWith(buffer, "Urshifu ") || TextStartsWith(buffer, "Urshifu\n"));
    else if (species == SPECIES_URSHIFU_RAPID_STRIKE)
        EXPECT(TextStartsWith(buffer, "Urshifu-Rapid-Strike"));
    else if (species == SPECIES_ROTOM)
        EXPECT(TextStartsWith(buffer, "Rotom ") || TextStartsWith(buffer, "Rotom\n"));
    else
        EXPECT(TextStartsWith(buffer, "Rotom-Wash"));
}
