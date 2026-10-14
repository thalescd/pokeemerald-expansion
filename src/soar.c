// Eon Flute soaring, ported from Artrios's pokeemerald "soar" branch
// (https://github.com/Artrios/pokeemerald/tree/soar).
//
// The ground is the Hoenn region map on an affine BG2. HBlankCB_Soar rewrites
// its scale and offset every scanline (mode 7) to fake perspective. Landing
// uses the same destinations as Fly.
#include "global.h"
#include "bg.h"
#include "decompress.h"
#include "event_data.h"
#include "field_effect.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "gpu_regs.h"
#include "item_menu.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "region_map.h"
#include "scanline_effect.h"
#include "script.h"
#include "soar.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "trig.h"
#include "window.h"
#include "constants/region_map_sections.h"
#include "constants/rgb.h"
#include "constants/songs.h"

// BG2 (affine, 8bpp) is the ground, BG0 holds the text windows
#define BG2_CHAR_BASE 0  // 0x0000-0x3FFF
#define BG2_MAP_BASE  8  // 0x4000-0x7FFF
#define BG0_CHAR_BASE 2
#define BG0_MAP_BASE  31

// The region map tilemap is 64x64. It's copied into the corner of a 128x128
// tilemap, and the rest is left as tile 0 (sea) so the horizon isn't empty.
#define REGION_MAP_TILEMAP_SIZE 64
#define GROUND_TILEMAP_SIZE     128

#define STD_FRAME_TILE 0x01
#define DLG_FRAME_TILE 0x0A
#define STD_FRAME_PAL  14
#define DLG_FRAME_PAL  15

#define TAG_EON 1

// Scanlines above the horizon are sky
#define HORIZON_Y 32
// Distance from the camera to the screen
#define M7_D 128

// Movement limits
#define MIN_Z Q_24_8(4)
#define MAX_Z Q_24_8(50)
#define MIN_X Q_24_8(0)
#define MAX_X Q_24_8(30 * 8)
#define MIN_Y Q_24_8(0)
#define MAX_Y Q_24_8(20 * 8)

enum {
    WIN_MAPSEC_NAME,
    WIN_LAND_PROMPT,
};

struct Soar
{
    struct RegionMap regionMap; // The mapsec under the player, and where to land
    mapsec_u16_t shownMapSecId;
    u8 eonSpriteId;
};

static void Task_FadeOutToSoar(u8 taskId);
static void Task_StartSoar(u8 taskId);
static void CB2_InitSoar(void);
static void CB2_Soar(void);
static void CB2_LandPrompt(void);
static void CB2_Land(void);
static void CB2_ExitSoar(void);
static void VBlankCB_Soar(void);
static void HBlankCB_Soar(void);

static EWRAM_DATA struct Soar *sSoar = NULL;

// Read by HBlankCB_Soar every scanline, so they live outside sSoar.
// Positions are 24.8 fixed point, in region map pixels.
static EWRAM_DATA s32 sPlayerPosX = 0;
static EWRAM_DATA s32 sPlayerPosY = 0;
static EWRAM_DATA s32 sPlayerPosZ = 0;
static EWRAM_DATA u8 sPlayerYaw = 0;
// (1 << 16) / depth, to keep divisions out of the HBlank interrupt
static EWRAM_DATA u32 sDepthReciprocals[DISPLAY_HEIGHT - HORIZON_Y] = {0};

static const u16 sSkyColor = RGB(8, 8, 20);

static const u32 sLatiasBrendan_Gfx[] = INCGFX_U32("graphics/soar/latias_brendan.png", ".4bpp.smol");
static const u16 sLatiasBrendan_Pal[] = INCGFX_U16("graphics/soar/latias_brendan.png", ".gbapal");
static const u32 sLatiasMay_Gfx[] = INCGFX_U32("graphics/soar/latias_may.png", ".4bpp.smol");
static const u16 sLatiasMay_Pal[] = INCGFX_U16("graphics/soar/latias_may.png", ".gbapal");
static const u32 sLatiosBrendan_Gfx[] = INCGFX_U32("graphics/soar/latios_brendan.png", ".4bpp.smol");
static const u16 sLatiosBrendan_Pal[] = INCGFX_U16("graphics/soar/latios_brendan.png", ".gbapal");
static const u32 sLatiosMay_Gfx[] = INCGFX_U32("graphics/soar/latios_may.png", ".4bpp.smol");
static const u16 sLatiosMay_Pal[] = INCGFX_U16("graphics/soar/latios_may.png", ".gbapal");

static const u8 sText_PlayerUsedEonFlute[] = _("{PLAYER} used the Eon Flute.{PAUSE_UNTIL_PRESS}");
static const u8 sText_LandHere[] = _("Would you like to land here?");

static const struct BgTemplate sSoarBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = BG0_CHAR_BASE,
        .mapBaseIndex = BG0_MAP_BASE,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
};

static const struct WindowTemplate sSoarWindowTemplates[] =
{
    [WIN_MAPSEC_NAME] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 12,
        .height = 2,
        .paletteNum = DLG_FRAME_PAL,
        .baseBlock = 0x18
    },
    [WIN_LAND_PROMPT] = {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 27,
        .height = 4,
        .paletteNum = DLG_FRAME_PAL,
        .baseBlock = 0x30
    },
    DUMMY_WIN_TEMPLATE
};

static const struct WindowTemplate sYesNoWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 21,
    .tilemapTop = 9,
    .width = 5,
    .height = 4,
    .paletteNum = DLG_FRAME_PAL,
    .baseBlock = 0xA0
};

static const struct OamData sOamData_Eon =
{
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(64x32),
    .size = SPRITE_SIZE(64x32),
    .priority = 0,
};

static const struct SpriteTemplate sSpriteTemplate_Eon =
{
    .tileTag = TAG_EON,
    .paletteTag = TAG_EON,
    .oam = &sOamData_Eon,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

void ItemUseOnFieldCB_EonFlute(u8 taskId)
{
    LockPlayerFieldControls();
    StringExpandPlaceholders(gStringVar4, sText_PlayerUsedEonFlute);
    DisplayItemMessageOnField(taskId, gStringVar4, Task_FadeOutToSoar);
}

static void Task_FadeOutToSoar(u8 taskId)
{
    ClearDialogWindowAndFrame(0, TRUE);
    FadeScreen(FADE_TO_BLACK, 0);
    FadeOutAndFadeInNewMapMusic(MUS_SURF, 2, 2);
    gTasks[taskId].func = Task_StartSoar;
}

static void Task_StartSoar(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(CB2_InitSoar);
        DestroyTask(taskId);
    }
}

// The field leaves window and blend registers set, which can hide the whole screen
static void ClearVramOamPlttRegs(void)
{
    DmaClearLarge16(3, (void *)(VRAM), VRAM_SIZE, 0x1000);
    DmaClear32(3, OAM, OAM_SIZE);
    DmaClear16(3, PLTT, PLTT_SIZE);

    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    SetGpuReg(REG_OFFSET_BG1CNT, 0);
    SetGpuReg(REG_OFFSET_BG1HOFS, 0);
    SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    SetGpuReg(REG_OFFSET_BG2CNT, 0);
    SetGpuReg(REG_OFFSET_BG2HOFS, 0);
    SetGpuReg(REG_OFFSET_BG2VOFS, 0);
    SetGpuReg(REG_OFFSET_BG3CNT, 0);
    SetGpuReg(REG_OFFSET_BG3HOFS, 0);
    SetGpuReg(REG_OFFSET_BG3VOFS, 0);
    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
}

static void LoadGroundGfx(void)
{
    const struct RegionMapInfo *info = &gRegionMapInfos[REGION_MAP_HOENN];
    u8 *tilemap;
    u32 i;

    DecompressDataWithHeaderVram(info->regionMapGfx, (void *)BG_CHAR_ADDR(BG2_CHAR_BASE));

    tilemap = Alloc(GetDecompressedDataSize(info->regionMapTilemap));
    DecompressDataWithHeaderWram(info->regionMapTilemap, tilemap);
    for (i = 0; i < REGION_MAP_TILEMAP_SIZE; i++)
        DmaCopy16(3, &tilemap[i * REGION_MAP_TILEMAP_SIZE], (u8 *)BG_SCREEN_ADDR(BG2_MAP_BASE) + i * GROUND_TILEMAP_SIZE, REGION_MAP_TILEMAP_SIZE);
    Free(tilemap);

    LoadPalette(info->regionMapPalette, BG_PLTT_ID(7), 3 * PLTT_SIZE_4BPP);
    LoadPalette(&sSkyColor, BG_PLTT_ID(0), PLTT_SIZEOF(1));
}

static void LoadEonGraphics(void)
{
    struct CompressedSpriteSheet sheet = {.size = 64 * 32 / 2, .tag = TAG_EON};
    struct SpritePalette palette = {.tag = TAG_EON};
    bool32 isMale = (gSaveBlock2Ptr->playerGender == MALE);

    if (FlagGet(FLAG_EON_LATI))
    {
        sheet.data = isMale ? sLatiasBrendan_Gfx : sLatiasMay_Gfx;
        palette.data = isMale ? sLatiasBrendan_Pal : sLatiasMay_Pal;
    }
    else
    {
        sheet.data = isMale ? sLatiosBrendan_Gfx : sLatiosMay_Gfx;
        palette.data = isMale ? sLatiosBrendan_Pal : sLatiosMay_Pal;
    }

    LoadCompressedSpriteSheet(&sheet);
    LoadSpritePalette(&palette);
    sSoar->eonSpriteId = CreateSprite(&sSpriteTemplate_Eon, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, 0);
}

static void InitPlayerPosition(void)
{
    u32 i;

    RegionMap_InitPlayerLocation(&sSoar->regionMap);
    sPlayerPosX = Q_24_8(sSoar->regionMap.cursorPosX * 8);
    sPlayerPosY = Q_24_8(sSoar->regionMap.cursorPosY * 8);
    sPlayerPosZ = Q_24_8(8);
    sPlayerYaw = 0;
    sSoar->shownMapSecId = MAPSEC_NONE;

    // Depth 0 (the horizon itself) is never drawn, it only needs to not divide by 0
    sDepthReciprocals[0] = 1 << 16;
    for (i = 1; i < ARRAY_COUNT(sDepthReciprocals); i++)
        sDepthReciprocals[i] = (1 << 16) / i;
}

static void UpdateMapSecPopup(void)
{
    mapsec_u16_t mapSecId;

    RegionMap_SetCursorPos(&sSoar->regionMap, Q_24_8_TO_INT(sPlayerPosX) / 8, Q_24_8_TO_INT(sPlayerPosY) / 8);
    mapSecId = sSoar->regionMap.mapSecId;
    // Like on the Fly map, the Battle Frontier stays a secret until the player has been there
    if (mapSecId == MAPSEC_BATTLE_FRONTIER && !FlagGet(FLAG_LANDMARK_BATTLE_FRONTIER))
        mapSecId = MAPSEC_NONE;
    if (mapSecId == sSoar->shownMapSecId)
        return;

    sSoar->shownMapSecId = mapSecId;
    if (mapSecId == MAPSEC_NONE)
    {
        ClearStdWindowAndFrameToTransparent(WIN_MAPSEC_NAME, TRUE);
    }
    else
    {
        GetMapName(gStringVar4, mapSecId, 0);
        DrawStdFrameWithCustomTileAndPalette(WIN_MAPSEC_NAME, FALSE, STD_FRAME_TILE, STD_FRAME_PAL);
        AddTextPrinterParameterized(WIN_MAPSEC_NAME, FONT_NORMAL, gStringVar4, 0, 1, TEXT_SKIP_DRAW, NULL);
        CopyWindowToVram(WIN_MAPSEC_NAME, COPYWIN_FULL);
    }
}

static void CB2_InitSoar(void)
{
    switch (gMain.state)
    {
    case 0:
        SetVBlankCallback(NULL);
        SetHBlankCallback(NULL);
        ClearVramOamPlttRegs();
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        FreeSpriteTileRanges();
        ResetPaletteFade();
        FreeAllSpritePalettes();
        sSoar = AllocZeroed(sizeof(*sSoar));
        gMain.state++;
        break;
    case 1:
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(1, sSoarBgTemplates, ARRAY_COUNT(sSoarBgTemplates));
        InitWindows(sSoarWindowTemplates);
        DeactivateAllTextPrinters();
        LoadUserWindowBorderGfx(WIN_MAPSEC_NAME, STD_FRAME_TILE, BG_PLTT_ID(STD_FRAME_PAL));
        LoadMessageBoxGfx(WIN_LAND_PROMPT, DLG_FRAME_TILE, BG_PLTT_ID(DLG_FRAME_PAL));
        gMain.state++;
        break;
    case 2:
        LoadGroundGfx();
        gMain.state++;
        break;
    case 3:
        InitPlayerPosition();
        LoadEonGraphics();
        UpdateMapSecPopup();
        gMain.state++;
        break;
    case 4:
        // BG2 isn't managed by bg.c, HBlankCB_Soar turns it on below the horizon
        SetGpuReg(REG_OFFSET_BG2CNT, BGCNT_PRIORITY(1)
                                   | BGCNT_CHARBASE(BG2_CHAR_BASE)
                                   | BGCNT_256COLOR
                                   | BGCNT_SCREENBASE(BG2_MAP_BASE)
                                   | BGCNT_WRAP
                                   | BGCNT_AFF1024x1024);
        SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB_Soar);
        SetHBlankCallback(HBlankCB_Soar);
        EnableInterrupts(INTR_FLAG_HBLANK);
        SetMainCallback2(CB2_Soar);
        break;
    }
}

static void VBlankCB_Soar(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();

    // Each frame starts with the sky
    REG_DISPCNT &= ~DISPCNT_BG2_ON;
    REG_BG2X = 0;
    REG_BG2Y = 0;
    REG_BG2PA = 0;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = 0;
}

static void HBlankCB_Soar(void)
{
    const u32 bldcntFog = BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT1_BD | BLDCNT_TGT1_BG2;
    s32 sinYaw, cosYaw;
    s32 lam, lcf, lsf, lxr, lyr;
    u32 scanline = REG_VCOUNT - 1;

    if (scanline >= DISPLAY_HEIGHT) // In VBlank, nothing to draw
        return;

    if (scanline < HORIZON_Y) // Sky gradient
    {
        REG_DISPCNT &= ~DISPCNT_BG2_ON;
        REG_BLDCNT = bldcntFog;
        REG_BLDY = scanline / 2;
        return;
    }

    if (scanline == HORIZON_Y)
        REG_DISPCNT |= DISPCNT_BG2_ON;

    // Fog that fades out towards the camera
    if (scanline <= 16 * 6)
    {
        REG_BLDCNT = bldcntFog;
        REG_BLDY = 16 - (scanline / 6);
    }
    else
    {
        REG_BLDCNT = 0;
    }

    sinYaw = gSineTable[sPlayerYaw];
    cosYaw = gSineTable[sPlayerYaw + 64];

    lam = (sPlayerPosZ * sDepthReciprocals[scanline - HORIZON_Y]) >> 12; // .8 * .16 / .12 = .12
    lcf = (lam * cosYaw) >> 8;                                          // .12 * .8 / .8 = .12
    lsf = (lam * sinYaw) >> 8;                                          // .12 * .8 / .8 = .12

    REG_BG2PA = lcf >> 4;
    REG_BG2PC = lsf >> 4;

    // Horizontal offset
    lxr = 120 * (lcf >> 4);
    lyr = (M7_D * lsf) >> 4;
    REG_BG2X = sPlayerPosX - lxr + lyr;

    // Vertical offset
    lxr = 120 * (lsf >> 4);
    lyr = (M7_D * lcf) >> 4;
    REG_BG2Y = sPlayerPosY - lxr - lyr;
}

#define sTiltAngle     data[0]
#define sBarrelRollDir data[1]
#define sDestAngle     data[2]

#define TILT_MAX 0x1000
#define TILT_MIN -0x1000
#define TILT_STEP 0x100
#define ROLL_TILT_STEP (TILT_STEP * 8)

static void UpdateEonSpriteTilt(struct Sprite *sprite)
{
    switch (sprite->sBarrelRollDir)
    {
    case 0: // No barrel roll
        if (sprite->sTiltAngle < sprite->sDestAngle)
            sprite->sTiltAngle += TILT_STEP;
        else if (sprite->sTiltAngle > sprite->sDestAngle)
            sprite->sTiltAngle -= TILT_STEP;
        break;
    case 1: // Increase angle
        if (sprite->sTiltAngle < TILT_MIN) // Rotation is past 180 degrees
        {
            sprite->sTiltAngle += ROLL_TILT_STEP;
            if (sprite->sTiltAngle >= TILT_MIN)
                sprite->sBarrelRollDir = 0;
            break;
        }
        sprite->sTiltAngle += ROLL_TILT_STEP;
        break;
    case -1: // Decrease angle
        if (sprite->sTiltAngle > TILT_MAX) // Rotation is past 180 degrees
        {
            sprite->sTiltAngle -= ROLL_TILT_STEP;
            if (sprite->sTiltAngle <= TILT_MAX)
                sprite->sBarrelRollDir = 0;
            break;
        }
        sprite->sTiltAngle -= ROLL_TILT_STEP;
        break;
    }

    SetOamMatrixRotationScaling(sprite->oam.matrixNum, 1 << 8, 1 << 8, -sprite->sTiltAngle);
}

static void StartBarrelRoll(struct Sprite *sprite)
{
    if (sprite->sBarrelRollDir == 0)
    {
        if (sprite->sDestAngle >= 0)
            sprite->sBarrelRollDir = 1;
        else
            sprite->sBarrelRollDir = -1;
    }
}

static void RunSoarFrame(void)
{
    RunTextPrinters();
    UpdatePaletteFade();
    UpdateEonSpriteTilt(&gSprites[sSoar->eonSpriteId]);
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
}

static void ShowLandPrompt(void)
{
    DrawDialogFrameWithCustomTileAndPalette(WIN_LAND_PROMPT, FALSE, DLG_FRAME_TILE, DLG_FRAME_PAL);
    AddTextPrinterParameterized(WIN_LAND_PROMPT, FONT_NORMAL, sText_LandHere, 0, 1, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(WIN_LAND_PROMPT, COPYWIN_FULL);
    CreateYesNoMenu(&sYesNoWindowTemplate, STD_FRAME_TILE, STD_FRAME_PAL, 0);
}

static void HandleSoarInput(void)
{
    struct Sprite *eon = &gSprites[sSoar->eonSpriteId];
    s32 sinYaw, cosYaw, speed;
    u8 mapSecType = sSoar->regionMap.mapSecType;
    mapsec_u16_t mapSecId = sSoar->regionMap.mapSecId;

    // The Southern Island isn't a Fly destination: soaring is the only way to land there
    if (JOY_NEW(A_BUTTON) && (mapSecType == MAPSECTYPE_CITY_CANFLY || mapSecType == MAPSECTYPE_BATTLE_FRONTIER || mapSecId == MAPSEC_SOUTHERN_ISLAND))
    {
        PlaySE(SE_SELECT);
        ShowLandPrompt();
        SetMainCallback2(CB2_LandPrompt);
        return;
    }

    if (JOY_NEW(B_BUTTON | START_BUTTON))
    {
        PlaySE(SE_PC_OFF);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        SetMainCallback2(CB2_ExitSoar);
        return;
    }

    if (JOY_NEW(R_BUTTON))
        StartBarrelRoll(eon);

    eon->sDestAngle = 0;
    if (JOY_HELD(DPAD_LEFT))
    {
        sPlayerYaw--;
        eon->sDestAngle = TILT_MIN;
    }
    if (JOY_HELD(DPAD_RIGHT))
    {
        sPlayerYaw++;
        eon->sDestAngle = TILT_MAX;
    }
    if (JOY_HELD(DPAD_UP))
    {
        sPlayerPosZ += 0x10;
        if (sPlayerPosZ > MAX_Z)
            sPlayerPosZ = MAX_Z;
    }
    if (JOY_HELD(DPAD_DOWN))
    {
        sPlayerPosZ -= 0x10;
        if (sPlayerPosZ < MIN_Z)
            sPlayerPosZ = MIN_Z;
    }

    // L flies at double speed
    speed = JOY_HELD(L_BUTTON) ? 2 : 1;
    sinYaw = gSineTable[sPlayerYaw];
    cosYaw = gSineTable[sPlayerYaw + 64];
    sPlayerPosX += speed * (sinYaw / 8);
    sPlayerPosY -= speed * (cosYaw / 8);

    if (sPlayerPosX < MIN_X)
        sPlayerPosX = MIN_X;
    else if (sPlayerPosX > MAX_X)
        sPlayerPosX = MAX_X;

    if (sPlayerPosY < MIN_Y)
        sPlayerPosY = MIN_Y;
    else if (sPlayerPosY > MAX_Y)
        sPlayerPosY = MAX_Y;

    UpdateMapSecPopup();
}

static void CB2_Soar(void)
{
    if (!gPaletteFade.active)
        HandleSoarInput();
    RunSoarFrame();
}

static void CB2_LandPrompt(void)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0: // Yes
        ClearDialogWindowAndFrameToTransparent(WIN_LAND_PROMPT, TRUE);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        SetMainCallback2(CB2_Land);
        break;
    case 1: // No
    case MENU_B_PRESSED:
        ClearDialogWindowAndFrameToTransparent(WIN_LAND_PROMPT, TRUE);
        SetMainCallback2(CB2_Soar);
        break;
    }
    RunSoarFrame();
}

static void FreeSoarResources(void)
{
    DisableInterrupts(INTR_FLAG_HBLANK);
    SetHBlankCallback(NULL);
    SetVBlankCallback(NULL);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
    FreeAllWindowBuffers();
}

static void CB2_Land(void)
{
    RunSoarFrame();
    if (!gPaletteFade.active)
    {
        FreeSoarResources();
        SetFlyDestination(&sSoar->regionMap);
        FREE_AND_SET_NULL(sSoar);
        WarpAndFlyIntoMap();
    }
}

static void CB2_ExitSoar(void)
{
    RunSoarFrame();
    if (!gPaletteFade.active)
    {
        FreeSoarResources();
        FREE_AND_SET_NULL(sSoar);
        SetMainCallback2(CB2_ReturnToField);
        gFieldCallback = FieldCB_ReturnToFieldNoScriptCheckMusic;
    }
}
