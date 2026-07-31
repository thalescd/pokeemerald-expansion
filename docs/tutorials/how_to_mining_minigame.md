# How to use the Mining Minigame

A recreation of the Gen 4 Underground mining minigame. The player is shown a wall of
terrain with items and stones buried in it, and chips away at it with a hammer or a
pickaxe to uncover what is inside before the wall collapses.

Ported from the [`mining_minigame` branch of volromhacking/pokeemerald-expansion](https://github.com/volromhacking/pokeemerald-expansion/tree/mining_minigame),
written by vol and psf with contributions from skeli and sbird. Their
[wiki page](https://github.com/volromhacking/pokeemerald-expansion/wiki/Mining-Minigame)
is the upstream documentation.

## Starting the minigame

From a script:

```
	fadescreen FADE_TO_BLACK
	special StartMining
```

The `fadescreen` is not optional — the minigame takes over the screen with its own
`MainCallback` and expects to fade in from black.

For testing there is also **Debug → Utilities → Mining Minigame**, which runs exactly
that script (`Debug_EventScript_Mining_Minigame` in `data/scripts/debug.inc`).

## Controls

| Input | Action |
| --- | --- |
| D-Pad | Move the cursor |
| A | Strike the terrain at the cursor |
| R | Switch to the hammer |
| L | Switch to the pickaxe |

The hammer clears a wide area — the tile under the cursor, its four neighbours and its
four corners. The pickaxe clears only the cursor tile and its four neighbours, but digs
the centre tile twice as deep. Both cost the same: every strike, whichever tool is held,
advances the *stress level* shown at the top of the screen by exactly one step. So the
hammer is for clearing ground quickly and the pickaxe is for getting at something you
have already found without wasting strikes around it.

The stress level has eight steps (`STRESS_LEVEL_POS_0` through `STRESS_LEVEL_POS_7`);
once it passes the last one the wall collapses and the session ends. The session also
ends early once everything buried has been dug up.

The player only receives the items they actually uncovered.

## What gets buried

Each session buries:

- **2 to 4 items** (`RANDOM(3) + 2`), one per quadrant of the wall, with the quadrants
  picked by a Fisher-Yates shuffle so the items are spread out.
- **2 stones**, which are pure obstacles — they are not collected, they just cost
  strikes to clear.

Rarity is rolled per item with `RANDOM(7)`:

| Roll | Rarity | Odds |
| --- | --- | --- |
| 0–3 | `ItemRarityTable_Common` | 4/7 |
| 4–5 | `ItemRarityTable_Uncommon` | 2/7 |
| 6 | `ItemRarityTable_Rare` | 1/7 |

The three tables live next to each other in `src/mining_minigame.c`. To change what the
minigame hands out, move `MININGID_*` entries between them — an item that appears in no
table is never generated.

## Configuration

Everything tunable is in `include/constants/mining_minigame.h`.

```c
#define MINING_FLAG_USE_DEFAULT_MESSAGE_BOX  FALSE
```

`FALSE` uses the minigame's own message box graphic; `TRUE` uses the player's chosen
overworld textbox frame instead.

Sound effects:

```c
#define MINING_SE_HIT_HAMMER      SE_M_ROCK_THROW
#define MINING_SE_HIT_PICKAXE     SE_M_DIG
#define MINING_SE_HIT_DUG_UP      SE_ICE_STAIRS
#define MINING_SE_WALL_COLLAPSE   SE_M_EARTHQUAKE
#define MINING_SE_TOOL_SWITCH     SE_ROTATING_GATE
#define MINING_SE_ITEM_SPARKLE    SE_RG_CARD_OPEN
```

Debug options, all gated behind `MINING_DEBUG_ENABLE`:

```c
#define MINING_DEBUG_ENABLE                              TRUE

#define MINING_DEBUG_ALL_SPRITES_VISIBLE                 TRUE   // see through the terrain
#define MINING_DEBUG_INFINITE_HITS                       FALSE  // the wall never collapses
#define MINING_DEBUG_ENABLE_ITEM_GENERATION_OPTIONS      TRUE   // force which items spawn
#define MINING_DEBUG_ENABLE_STONE_GENERATION_OPTIONS     FALSE  // force which stones spawn
```

With the item generation options on, `MINING_DEBUG_DESIRED_NUMBER_OF_ITEMS` and
`MINING_DEBUG_MININGID_ITEM1`–`ITEM4` replace the random rolls. Leave
`MINING_DEBUG_ENABLE` as `FALSE` for a release build.

## The generated tile table

`src/data/mining_minigame.h` holds `sSpriteTileTable`, which records for every item and
stone which of its 16 tiles contain non-transparent pixels. That is how the minigame
knows when enough of a buried sprite has been exposed to count as dug up.

It is **generated, not committed** — `tools/mining_minigame/analyze_sprites.py` reads the
built `.4bpp` sprites and writes it during the build. You should never edit it by hand.
The wiring lives in the `Makefile`: the header is an `AUTO_GEN_TARGETS` entry, so
`make generated` builds it and `make clean-generated` removes it.

This is also why `tools/mining_minigame/table.json` matters: it maps each sprite file to
its `MININGID_*` name, and the tool refuses to run if a sprite has no entry.

## Adding a new item

Sprites are 64×64 px, indexed, 16 colours, with the first palette colour used as
transparency. Each item carries its own palette.

1. Save the sprite as `graphics/mining_minigame/items/my_item.png`.
2. In `include/constants/mining_minigame.h`, add a tag to the sprite-tag enum
   (`MINING_TAG_ITEM_MY_ITEM`) and an ID to the `MININGID_*` enum
   (`MININGID_MY_ITEM`). Add item IDs **after** the stones — `MINING_COUNT_ID_STONE`
   must stay pointing at the last stone.
3. In `src/mining_minigame.c`, include the graphics:

   ```c
   const u32 gItemMyItemGfx[] = INCGFX_U32("graphics/mining_minigame/items/my_item.png", ".4bpp.smol");
   static const u16 gItemMyItemPal[] = INCGFX_U16("graphics/mining_minigame/items/my_item.png", ".gbapal");
   ```

4. Add its sprite sheet, following the ones already there:

   ```c
   static const struct CompressedSpriteSheet sSpriteSheet_ItemMyItem =
   {
       gItemMyItemGfx,
       2048,
       MINING_TAG_ITEM_MY_ITEM,
   };
   ```

5. Add it to `MiningItemList[]`, which is what ties the mining ID to the actual bag item:

   ```c
   [MININGID_MY_ITEM] =
   {
       .bagItemId = ITEM_MY_ITEM,
       .tag = MINING_TAG_ITEM_MY_ITEM,
       .sheet = &sSpriteSheet_ItemMyItem,
       .paldata = gItemMyItemPal,
   },
   ```

6. Add `MININGID_MY_ITEM` to one of the three rarity tables.
7. Add the sprite to `tools/mining_minigame/table.json` under `"items"`, pointing at the
   **`.4bpp`** path, not the `.png`:

   ```json
   {
       "id": "MININGID_MY_ITEM",
       "path": "./graphics/mining_minigame/items/my_item.4bpp"
   }
   ```

Items do not need a `SpriteTemplate` of their own — `CreatePaletteAndReturnTemplate()`
builds one at runtime from the `MiningItemList` entry.

## Adding a new stone

Stones differ from items in three ways: they all share one palette
(`graphics/mining_minigame/stones/stones.pal`), they need their own `SpriteTemplate`, and
they are drawn from an explicit `switch` rather than from a table.

1. Save the sprite as `graphics/mining_minigame/stones/my_stone.png`.
2. Add `MINING_TAG_STONE_MY_STONE` and `MININGID_STONE_MY_STONE` to the enums in
   `include/constants/mining_minigame.h`. Stones must stay **first** in the `MININGID_*`
   enum, and `MINING_COUNT_ID_STONE` must be updated to name the new last stone.
3. Include the graphics in `src/mining_minigame.c` (the palette already exists as
   `gStonePal`):

   ```c
   const u32 gStoneMyStoneGfx[] = INCGFX_U32("graphics/mining_minigame/stones/my_stone.png", ".4bpp.smol");
   ```

4. Add the sheet, palette and template — note that stone sheets and palettes are
   `NULL`-terminated arrays, unlike the item sheets:

   ```c
   static const struct CompressedSpriteSheet sSpriteSheet_StoneMyStone[] =
   {
       {gStoneMyStoneGfx, 2048, MINING_TAG_STONE_MY_STONE},
       {NULL},
   };

   static const struct SpritePalette sSpritePal_StoneMyStone[] =
   {
       {gStonePal, MINING_TAG_STONE_MY_STONE},
       {NULL},
   };

   static const struct SpriteTemplate gSpriteStoneMyStone =
   {
       .tileTag = MINING_TAG_STONE_MY_STONE,
       .paletteTag = MINING_TAG_STONE_MY_STONE,
       .oam = &gOamItem64x64,
       .anims = gDummySpriteAnimTable,
       .images = NULL,
       .affineAnims = gDummySpriteAffineAnimTable,
       .callback = SpriteCallbackDummy,
   };
   ```

5. Add a case to `DrawItemSprite()`:

   ```c
   case MININGID_STONE_MY_STONE:
       LoadSpritePalette(sSpritePal_StoneMyStone);
       LoadCompressedSpriteSheet(sSpriteSheet_StoneMyStone);
       CreateSprite(&gSpriteStoneMyStone, posX + POS_OFFS_64X64, posY + POS_OFFS_64X64, 3);
       break;
   ```

6. Add the sprite to `tools/mining_minigame/table.json` under `"stones"`, again with the
   `.4bpp` path.

## Files

| File | Purpose |
| --- | --- |
| `src/mining_minigame.c` | The whole minigame |
| `include/constants/mining_minigame.h` | Config, sprite tags, `MININGID_*` enum |
| `include/mining_minigame.h` | Declares `StartMining` |
| `src/data/mining_minigame.h` | Generated tile table — do not edit |
| `tools/mining_minigame/analyze_sprites.py` | Generates the tile table |
| `tools/mining_minigame/table.json` | Sprite file → `MININGID_*` mapping |
| `graphics/mining_minigame/` | UI, cursor, tools, hit effects, items, stones |
