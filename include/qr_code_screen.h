#ifndef GUARD_QR_CODE_SCREEN_H
#define GUARD_QR_CODE_SCREEN_H

#include "global.h"

struct ScriptContext;

// Full-screen display of the player's party encoded as a QR code containing
// Pokemon Showdown / pokepaste text.
void ShowPartyQrCodeScreen(void);

// Script entry point, used as:
//     callnative Script_ShowPartyQrCodeScreen
//     waitstate
void Script_ShowPartyQrCodeScreen(struct ScriptContext *ctx);

// Draws an encoded symbol into a 4bpp tile buffer, one module per pixel,
// centred so the leftover margin serves as the quiet zone. Exposed separately
// from the screen so the pixel layout can be checked against the encoder.
void QrCode_DrawToTileBuffer(u8 *tiles, u32 tilesWide, const u8 *qrcode);

#endif // GUARD_QR_CODE_SCREEN_H
