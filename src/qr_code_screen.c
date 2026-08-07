#include "global.h"
#include "bg.h"
#include "gpu_regs.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "party_export.h"
#include "qr_code_screen.h"
#include "qrcodegen.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"

// Shows the player's party as a QR code holding Pokemon Showdown / pokepaste
// text. Scanning it with any camera yields a block that pastes straight into
// pokepast.es or src/data/trainers.party.

// v33 (149 modules) is the largest symbol that fits the 160px screen height
// once a 4-module quiet zone is added.
#define QR_MAX_VERSION 33

// One QR module is drawn as one pixel, so the whole symbol plus its quiet zone
// lives inside a 20x20 tile window.
#define QR_WINDOW_TILES_W 20
#define QR_WINDOW_TILES_H 20
#define QR_CANVAS_SIZE    (QR_WINDOW_TILES_W * TILE_WIDTH)

// Palette slots inside the QR window's palette.
#define QR_COLOR_LIGHT 1
#define QR_COLOR_DARK  2

enum {
    WIN_QR,
    WIN_INFO,
};

enum {
    STATE_ANNOUNCE,
    STATE_ENCODE,
    STATE_WAIT_INPUT,
    STATE_FADE_OUT,
    STATE_EXIT,
};

struct QrCodeScreen
{
    char text[PARTY_EXPORT_BUFFER_SIZE];
    u8 qrcode[qrcodegen_BUFFER_LEN_FOR_VERSION(QR_MAX_VERSION)];
    u8 temp[qrcodegen_BUFFER_LEN_FOR_VERSION(QR_MAX_VERSION)];
    u32 textLen;
    u32 monsWritten;
    u8 taskId;
    bool8 encoded;
};

static EWRAM_DATA struct QrCodeScreen *sScreen = NULL;

static void CB2_InitQrCodeScreen(void);
static void CB2_RunQrCodeScreen(void);
static void VBlankCB_QrCodeScreen(void);
static void Task_QrCodeScreen(u8 taskId);

static const u16 sQrPalette[16] =
{
    [0]              = RGB_WHITE,
    [QR_COLOR_LIGHT] = RGB_WHITE,
    [QR_COLOR_DARK]  = RGB_BLACK,
};

static const struct BgTemplate sBgTemplates[] =
{
    {
        .bg = 0,                 // text column
        .charBaseIndex = 2,
        .mapBaseIndex = 31,
        .priority = 0,
    },
    {
        .bg = 1,                 // QR canvas, on its own charblock because the
        .charBaseIndex = 0,      // 400 tiles it needs would not leave room for
        .mapBaseIndex = 30,      // the text window in a shared one
        .priority = 1,
    },
};

static const struct WindowTemplate sWindowTemplates[] =
{
    [WIN_QR] = {
        .bg = 1,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = QR_WINDOW_TILES_W,
        .height = QR_WINDOW_TILES_H,
        .paletteNum = 14,
        .baseBlock = 0x0001,
    },
    [WIN_INFO] = {
        .bg = 0,
        .tilemapLeft = QR_WINDOW_TILES_W,
        .tilemapTop = 1,
        .width = 10,
        .height = 18,
        .paletteNum = 15,
        .baseBlock = 0x0001,
    },
    DUMMY_WIN_TEMPLATE
};

static const u8 sTextColors[3] = { 0, 2, 3 };

static const u8 sText_Generating[] = _("Generating\nQR code...");
static const u8 sText_Instructions[] = _("Scan this QR\ncode to get\nyour party as\npokepaste\ntext!");
static const u8 sText_Close[] = _("B: Close");
static const u8 sText_NoParty[] = _("No Pokemon\nto export.");
static const u8 sText_TooBig[] = _("Party is too\nlarge to fit\nin a QR code.");
static const u8 sText_MonsSuffix[] = _(" of 6 shown");

// Writes a single 4bpp pixel into tile data. Tiles are stored one 8x8 block at
// a time, 4 bytes per row, with the low nibble holding the left pixel.
static void PlotPixel(u8 *tiles, u32 tilesWide, u32 x, u32 y, u32 color)
{
    u8 *byte = tiles
             + ((y / TILE_HEIGHT) * tilesWide + (x / TILE_WIDTH)) * TILE_SIZE_4BPP
             + (y % TILE_HEIGHT) * 4
             + (x % TILE_WIDTH) / 2;

    if (x & 1)
        *byte = (*byte & 0x0F) | (color << 4);
    else
        *byte = (*byte & 0xF0) | color;
}

// Draws one QR module per pixel, centred in the buffer. Whatever space is left
// over becomes the quiet zone, which decoders need in order to find the symbol.
void QrCode_DrawToTileBuffer(u8 *tiles, u32 tilesWide, const u8 *qrcode)
{
    u32 size = qrcodegen_getSize((u8 *)qrcode);
    u32 origin = (tilesWide * TILE_WIDTH - size) / 2;
    u32 x, y;

    for (y = 0; y < size; y++)
    {
        for (x = 0; x < size; x++)
        {
            if (qrcodegen_getModule((u8 *)qrcode, x, y))
                PlotPixel(tiles, tilesWide, origin + x, origin + y, QR_COLOR_DARK);
        }
    }
}

static void DrawQrCode(void)
{
    FillWindowPixelBuffer(WIN_QR, PIXEL_FILL(QR_COLOR_LIGHT));
    QrCode_DrawToTileBuffer((u8 *)GetWindowAttribute(WIN_QR, WINDOW_TILE_DATA),
                            QR_WINDOW_TILES_W, sScreen->qrcode);
    CopyWindowToVram(WIN_QR, COPYWIN_GFX);
}

static void PrintInfo(const u8 *message, bool32 showCount)
{
    FillWindowPixelBuffer(WIN_INFO, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WIN_INFO, FONT_SMALL, 0, 0, sTextColors, TEXT_SKIP_DRAW, message);

    if (showCount)
    {
        u8 line[24];
        u8 *end = ConvertIntToDecimalStringN(line, sScreen->monsWritten,
                                             STR_CONV_MODE_LEFT_ALIGN, 1);
        StringCopy(end, sText_MonsSuffix);
        AddTextPrinterParameterized3(WIN_INFO, FONT_SMALL, 0, 84, sTextColors,
                                     TEXT_SKIP_DRAW, line);
    }

    AddTextPrinterParameterized3(WIN_INFO, FONT_SMALL, 0, 120, sTextColors,
                                 TEXT_SKIP_DRAW, sText_Close);
    CopyWindowToVram(WIN_INFO, COPYWIN_GFX);
}

void ShowPartyQrCodeScreen(void)
{
    SetMainCallback2(CB2_InitQrCodeScreen);
}

// Script entry point. The caller is expected to follow this with "waitstate";
// the screen resumes the script through CB2_ReturnToFieldContinueScript.
void Script_ShowPartyQrCodeScreen(struct ScriptContext *ctx)
{
    ShowPartyQrCodeScreen();
}

static void CB2_InitQrCodeScreen(void)
{
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetVBlankCallback(NULL);

    sScreen = AllocZeroed(sizeof(*sScreen));

    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
    SetBgTilemapBuffer(0, AllocZeroed(BG_SCREEN_SIZE));
    SetBgTilemapBuffer(1, AllocZeroed(BG_SCREEN_SIZE));

    InitWindows(sWindowTemplates);
    DeactivateAllTextPrinters();
    ResetPaletteFade();
    ResetSpriteData();
    ResetTasks();
    ScanlineEffect_Stop();

    LoadPalette(sQrPalette, BG_PLTT_ID(14), sizeof(sQrPalette));
    Menu_LoadStdPalAt(BG_PLTT_ID(15));

    FillBgTilemapBufferRect(0, 0, 0, 0, 32, 32, 15);
    FillBgTilemapBufferRect(1, 0, 0, 0, 32, 32, 14);
    PutWindowTilemap(WIN_QR);
    PutWindowTilemap(WIN_INFO);
    FillWindowPixelBuffer(WIN_QR, PIXEL_FILL(QR_COLOR_LIGHT));
    FillWindowPixelBuffer(WIN_INFO, PIXEL_FILL(0));

    PrintInfo(sText_Generating, FALSE);
    CopyWindowToVram(WIN_QR, COPYWIN_FULL);
    CopyBgTilemapBufferToVram(0);
    CopyBgTilemapBufferToVram(1);

    ShowBg(0);
    ShowBg(1);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);

    sScreen->taskId = CreateTask(Task_QrCodeScreen, 0);
    SetVBlankCallback(VBlankCB_QrCodeScreen);
    SetMainCallback2(CB2_RunQrCodeScreen);
}

static void CB2_RunQrCodeScreen(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB_QrCodeScreen(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void Task_QrCodeScreen(u8 taskId)
{
    s16 *state = &gTasks[taskId].data[0];

    switch (*state)
    {
    case STATE_ANNOUNCE:
        // Let the "Generating" message reach the screen first. Encoding a full
        // party measures around 100ms on hardware, so this is a brief stall
        // rather than a real wait, but the message costs nothing.
        if (!gPaletteFade.active)
            (*state)++;
        break;

    case STATE_ENCODE:
        sScreen->textLen = BuildPartyExportText(sScreen->text, sizeof(sScreen->text),
                                                PARTY_EXPORT_QR_CAPACITY,
                                                &sScreen->monsWritten);
        if (sScreen->monsWritten == 0)
        {
            PrintInfo(sText_NoParty, FALSE);
        }
        else
        {
            sScreen->encoded = qrcodegen_encodeText(sScreen->text, sScreen->temp,
                                                    sScreen->qrcode, qrcodegen_Ecc_MEDIUM,
                                                    1, QR_MAX_VERSION,
                                                    qrcodegen_Mask_AUTO, true);
            if (sScreen->encoded)
            {
                DrawQrCode();
                PrintInfo(sText_Instructions, sScreen->monsWritten < PARTY_SIZE);
            }
            else
            {
                PrintInfo(sText_TooBig, FALSE);
            }
        }
        *state = STATE_WAIT_INPUT;
        break;

    case STATE_WAIT_INPUT:
        if (JOY_NEW(B_BUTTON) || JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            (*state)++;
        }
        break;

    case STATE_FADE_OUT:
        if (!gPaletteFade.active)
            (*state)++;
        break;

    case STATE_EXIT:
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        Free(GetBgTilemapBuffer(0));
        Free(GetBgTilemapBuffer(1));
        Free(sScreen);
        sScreen = NULL;
        SetMainCallback2(CB2_ReturnToFieldContinueScript);
        break;
    }
}
