# Plano: usar as sprites de follower nos Pokémon do overworld (NPCs e cenas)

- **Branch:** `plan/follow-sprite`
- **Status:** em andamento
- **Criado em:** 2026-10-15
- **Escopo (pode alterar):** `data/maps/Route101/map.json`, `data/maps/SouthernIsland_Interior/scripts.inc` (teste inicial);
  o restante da fase 1 só depois do resultado do teste
- **Fora de escopo:** as constantes e os `gObjectEventGraphicsInfo_*` antigos (não remover), os mapas `*_Frlg`, o teleférico
  e as sprites de cena (fase 3)

Este plano só pode ser trabalhado na branch acima. As regras de trabalho (levantamento antes de executar, anotar o
progresso aqui, uma decisão por vez) estão no `CLAUDE.md`.

Valores de **Status**: `planejado`, `em andamento`, `concluído`, `abandonado`.

## Progresso

- [x] 2026-10-15: decisão do usuário: começar por um **teste** só com o Zigzagoon do Birch (Route101) e os Lati@s da
      Southern Island. O resto da fase 1 (Mt. Chimney, Route120, Route114, Lilycove Lady) fica para depois do teste.
- [x] 2026-10-15: Route101 (`map.json`) e Southern Island (`scripts.inc`) trocados para `OBJ_EVENT_GFX_SPECIES(...)`.
      **Não compilado, não testado** (o build é do usuário).
- [x] 2026-10-16: usuário compilou em outra máquina e testou em jogo: o build aceitou a macro no `map.json` (canário
      OK), o Zigzagoon do Birch (Route101) e o **Latios** (Southern Island) aparecem com a sprite de follower.
      **Ainda não testados**: o Latias, o movimento completo da perseguição na Route101 (direções e olhar final) e o
      objeto sumir após capturar/derrotar o Lati@s.
- [x] 2026-10-16: decisões do usuário: converter Mt. Chimney e Route114; na **Route120 trocar os Poochyenas por
      Mightyenas** (não por Poochyena de follower); Lilycove Lady fica para o usuário checar antes. Mt. Chimney (23,19)
      confirmado: `RUN_IN_PLACE_LEFT` → `JOG_IN_PLACE_LEFT`.
- [x] 2026-10-16: editados `MtChimney/map.json` (10× `OBJ_EVENT_GFX_SPECIES(POOCHYENA)` + o movimento do (23,19)),
      `Route114/map.json` (1×) e `Route120/map.json` (2× `OBJ_EVENT_GFX_SPECIES(MIGHTYENA)`). O `Bufoh!` dos
      Mightyenas não cita a espécie e ficou como está. **Não compilado, não testado.**
- [x] 2026-10-16: o usuário lembrou que um grunt da Route120 fala de Poochyena: `Route120_Text_KeepMakingMoreLand`
      (em `data/maps/Route110/scripts.inc`, grunt `Route120_EventScript_BusyMagmaGrunt`) trocado de "POOCHYENA isn't even a
      WATER POKéMON!" para "MIGHTYENA ...", para bater com os Mightyenas ao lado. Mt. Chimney não tem
      texto que cite a espécie (só os labels `MtChimney_EventScript_*Poochyena`), então nada muda lá.
- [x] 2026-10-17: usuário testou em jogo e **Mt. Chimney está certo** (o do Aqua no (23,19) não toca animação de bola).
- [x] 2026-10-17: usuário testou em jogo e **os Mightyenas da Route120 estão certos**. Nota: eles aparecem **antes** da
      6ª insígnia (`FLAG_BADGE06_GET` 0x86C desligada); flag ligada esconde o objeto.
- [x] 2026-10-17: decisão do usuário: o Poochyena da Route114 (19,12) passa de `FACE_RIGHT` para
      `WALK_IN_PLACE_RIGHT`, como os do Mt. Chimney. Editado em `Route114/map.json`. **Não compilado, não testado.**
- [x] 2026-10-17: pedido do usuário: o Azurill de Fallarbor Town (8,12, `LOOK_AROUND`, flag
      `FLAG_HIDE_FALLARBOR_AZURILL`) trocado para `OBJ_EVENT_GFX_SPECIES(AZURILL)` em `FallarborTown/map.json`. Item da
      fase 2 (tabela 2.2) feito fora de ordem. A sprite antiga é 16x16 e a de follower é 32x32: **o tamanho muda**.
      Movimento `LOOK_AROUND` é seguro (3.1). **Não compilado, não testado.**
- [ ] Usuário confere o Azurill de Fallarbor (tamanho/posição na grama, cry e fala ao conversar).
- [x] 2026-10-17: usuário testou em jogo e confirmou: **Route114 certa** (Poochyena andando no lugar), **sprites
      sobrevivem a salvar e resetar no mesmo mapa**, **Southern Island toda certa** (Latias, objeto sumindo após
      capturar) e **Route101 toda certa** (perseguição completa).
- [x] 2026-10-17: decisão do usuário: Lilycove Lady converte **os 5**. Editado `src/data/lilycove_lady.h`: os 5
      `.monGfxId` (Zigzagoon, Skitty, Poochyena, Kecleon, Pikachu) viraram `OBJ_EVENT_GFX_SPECIES(...)`. O objeto no
      `LilycoveCity_PokemonCenter_1F/map.json` usa `FACE_DOWN` (seguro, sem "faster"). Skitty, Kecleon e Pikachu eram
      16x16 e viram 32x32: **o tamanho muda**. **Não compilado, não testado.**
- [x] 2026-10-17: decisão do usuário: **Lilycove Lady fica de lado** (adiada, sem prazo). A edição acima foi
      **revertida**: `src/data/lilycove_lady.h` voltou ao original (constantes antigas `OBJ_EVENT_GFX_*`).
- [x] 2026-10-17: usuário testou em jogo e o **Azurill de Fallarbor está certo**.
- [x] 2026-10-17: fase 1 commitada (`4ea2f339e8`). Lilycove Lady adiada; Kecleon e Regis (2.3) seguem em aberto.
- [x] 2026-10-17: usuário pediu para começar a fase 2. Levantamento na seção 8. **Nada alterado; aguardando aprovação e
      as decisões da seção 8.5.**
- [x] 2026-10-17: decisão do usuário: **ignorar todos os mapas da Battle Frontier** (Battle Pike, Palace, Outside East,
      Pokémon Center) e o `battle_pike.c`. A lista por local está na seção 8.6; aguardando aprovação para editar.
- [x] 2026-10-17: decisão do usuário: editar **todos de uma vez** para checar em massa. Editados os 31 objetos da seção 8.6
      em 21 `map.json` (`graphics_id` → `OBJ_EVENT_GFX_SPECIES(...)`) e `FortreeCity_House4/scripts.inc`
      (`WingullExit`: `walk_in_place_faster_down` → `walk_in_place_fast_down`). **Não compilado, não testado.**
- [x] 2026-10-17: decisão do usuário: em **Dewford Town, casa 1** o Zigzagoon vira **Pawmi** (espécie, não só sprite).
      `DewfordTown_House1/map.json` → `OBJ_EVENT_GFX_SPECIES(PAWMI)`; `scripts.inc`: `playmoncry SPECIES_PAWMI` e a fala
      `PAWMI: Pawmi!`. O label `DewfordTown_House1_EventScript_Zigzagoon` e o label do texto foram mantidos.
      Pawmi conferido: `P_FAMILY_PAWMI` ligado e `overworld.png` 192x32. **Não compilado, não testado.**
- [x] 2026-10-17: conferidos em jogo pelo usuário: Dewford (Pawmi), Fallarbor, Fortree casa 1, Fortree casa 4, Lilycove,
      Mossdeep casa 2, Pacifidlog, Route 119 casa, Rustboro apto 2 e casa 3. **Ainda não conferidos (usuário vê depois):**
      Route 104 casa do Briney, Rusturf Tunnel, SS Tidal corredor.
- [x] 2026-10-17: decisões do usuário, trocas de espécie (sprite + `playmoncry` + fala do mon; falas inventadas por mim, só
      o nome da espécie é certo, ajustar se quiser):
      Fortree casa 5 → **Pikipek** (`PIKIPEK: Pipipi!`); Lavaridge casa → **Meowth de Galar** (`SPECIES_MEOWTH_GALAR`,
      `MEOWTH: Nyaa!`); Mossdeep casa 4 → **Snover** (`SNOVER: Fuuu?`); Route 109 → **Oshawott** (`OSHAWOTT: Osha?`, e o
      texto do velho `Route109_Text_ZigzagoonPicksUpLitter` agora diz "my OSHAWOTT"); Route 117 → Zigzagoon→**Magby**,
      Kecleon→**Eevee**, Azumarill→**Poliwag** (o Pikachu em (51,1) ficou como estava; esses 3 não têm script);
      Slateport Fan Club → Zigzagoon→**Growlithe de Hisui** (`SPECIES_GROWLITHE_HISUI`), Skitty→**Snubbull**,
      Azumarill→**Lombre**; Sootopolis casa 4 → **Slowking**; Verdanturf casa do Friendship Rater → **Clefairy**.
      Todas as espécies têm `overworld.png` (conferido, inclusive os de Galar e Hisui). Labels dos scripts e dos textos
      mantidos com o nome antigo (ver o item de 2026-10-17 abaixo, que os renomeou). **Não compilado, não testado.**
- [x] 2026-10-17: usuário testou em jogo e confirmou **todas as trocas de espécie acima** (Fortree casa 5, Lavaridge,
      Mossdeep casa 4, Route 109, Route 117, Slateport Fan Club, Sootopolis casa 4, Verdanturf).
- [x] 2026-10-17: pedido do usuário: **labels renomeados** para bater com a nova espécie, em `scripts.inc` e `map.json`
      de 8 mapas: `EventScript_`/`Text_` Zigzagoon→Pawmi (Dewford), →Pikipek (Fortree casa 5), →Meowth (Lavaridge),
      →Oshawott (Route109, inclusive `Text_OshawottPicksUpLitter` e `Text_OshawottCry`); Skitty→Snover (Mossdeep casa 4);
      Slateport Fan Club Zigzagoon→Growlithe, Skitty→Snubbull, Azumarill→Lombre; Azumarill→Slowking (Sootopolis);
      Pikachu→Clefairy (Verdanturf). Conferido que cada label tem a definição e a referência no `map.json`.
      **Não compilado.** Se faltar algum label, o assembler acusa na hora do build.
- [ ] Pendentes de conferência: Route 104 casa do Briney, Rusturf Tunnel e SS Tidal corredor (usuário vê depois). (tamanho 16x16 → 32x32, Route119_House com 6 Wingulls, paleta na Route117
      e no Fan Club de Slateport, e o Wingull saindo da casa de Fortree sem animação de Poké Bola).
- [ ] Decisões abertas: Kecleon e Regis (seção 2.3).
- [ ] Lilycove Lady (aguardando o usuário checar).
- [x] 2026-10-16: o usuário pediu para verificar Kecleon (Fortree, Route119/120) e os Regis. Levantamento na seção 2.3:
      nenhum tem estado ligado/desligado na sprite, nenhum tem comparação por id no C. Nada alterado ainda; aguardando as
      decisões (o Kecleon da ponte e a sombra, e se quer os Regis).

Levantamento feito em 2026-10-15. **Nada foi alterado no código ou nos mapas**; este documento só diz o que existe, o que
quebra e o que precisa ser feito. Números de linha são de 2026-10-15 e podem deslocar. Eu não compilo: o build e os
testes são do usuário (`make -j20`, no WSL).

## 0. Resumo

**Não precisa de código novo.** O upstream já tem exatamente esse recurso e ele está ligado neste projeto
(`OW_POKEMON_OBJECT_EVENTS TRUE` em `include/config/overworld.h:50`, com o comentário "Can be used for NPCs using the
`OBJ_EVENT_GFX_SPECIES` macro"). Em vez de `OBJ_EVENT_GFX_POOCHYENA` (a sprite antiga, tabela própria), basta usar
`OBJ_EVENT_GFX_SPECIES(POOCHYENA)` (a sprite de follower da espécie, `graphics/pokemon/poochyena/overworld.png`).

- **Em `map.json`**: trocar o `graphics_id` por `"OBJ_EVENT_GFX_SPECIES(POOCHYENA)"`. A doc do upstream manda fazer isso
  no campo "Sprite" do Porymap (`docs/tutorials/how_to_new_pokemon.md:1195`). O `mapjson` repassa o texto cru.
- **Em script**: `setvar VAR_OBJ_GFX_ID_1, OBJ_EVENT_GFX_SPECIES(LATIOS)`. O próprio jogo já faz isso com a creche
  (`GetDaycareGraphics`, `src/event_object_movement.c:11370`, grava o id de espécie em `VAR_OBJ_GFX_ID_*`).
- **Em C**: o id é só um `u16` (`species + OBJ_EVENT_MON`), então onde hoje há uma constante `OBJ_EVENT_GFX_X`
  basta pôr a macro (caso da Lilycove Lady).

**Há 4 armadilhas reais** (seção 3). A que realmente pode aparecer em jogo: as animações de "faster/fastest" do follower
são as de entrar/sair da Poké Bola, e **um** Poochyena do Mt. Chimney usa esse movimento.

**Escopo mínimo** (os casos que você citou, mais a Lilycove Lady): ~20 linhas em 6 arquivos (seção 5, fase 1).

---

## 1. Como o jogo trata essas sprites (verificado)

### 1.1 Duas famílias de gráficos de objeto
| | Sprite "antiga" (vanilla) | Sprite de follower (por espécie) |
|---|---|---|
| Id | `OBJ_EVENT_GFX_POOCHYENA` (índice do enum, `include/constants/event_objects.h`) | `OBJ_EVENT_GFX_SPECIES(POOCHYENA)` = `SPECIES_POOCHYENA + OBJ_EVENT_MON` (bit 14) |
| Dados | `gObjectEventGraphicsInfoPointers[]` → `gObjectEventGraphicsInfo_Poochyena` (`src/data/object_events/object_event_graphics_info.h:1765`) | `gSpeciesInfo[species].overworldData` (macro `OVERWORLD(...)` em `gen_3_families.h`) |
| Imagem | `sPicTable_PoochyenaOld` | `sPicTable_Poochyena` (`graphics/pokemon/poochyena/overworld.png`) |
| Paleta | fixa (`OBJ_EVENT_PAL_TAG_POOCHYENA`) | **dinâmica**, de `overworld_normal.pal` / `overworld_shiny.pal` da espécie |
| Anims | `sAnimTable_Standard` (20 entradas) | `sAnimTable_Following` (24 entradas, ver 3.1) |

É por isso que o seu Poochyena seguindo o jogador e o Poochyena do Team Magma parecem diferentes: são dois conjuntos de
arquivos distintos.

### 1.2 O caminho do id até a tela
1. `map.json` → `mapjson` gera `object_event N, <graphics_id>, ...` (`tools/mapjson/mapjson.cpp:267`), texto cru.
2. `GetObjectEventGraphicsInfo(graphicsId)` (`src/event_object_movement.c:3199`):
   - se `graphicsId` está em `OBJ_EVENT_GFX_VARS..VAR_F` → vira o valor da var `VAR_OBJ_GFX_ID_n`;
   - se tem o bit `OBJ_EVENT_MON` → `SpeciesToGraphicsInfo(espécie, shiny, fêmea)` (linha 2136);
   - senão → tabela vanilla.
3. A paleta é carregada por `LoadDynamicFollowerPalette` (linha 2171) quando o `paletteTag` é `OBJ_EVENT_PAL_TAG_DYNAMIC`.
4. `OBJ_EVENT_GFX_SPECIES_SHINY/FEMALE/SHINY_FEMALE(x)` existem se um dia quiser variante.

### 1.3 Pontos que eu conferi e estão OK
- As 4 espécies têm `overworld.png`, `overworld_normal.pal` e `overworld_shiny.pal` e usam `OVERWORLD(...)` no
  `species_info`: **Poochyena** (`gen_3_families.h:993`), **Zigzagoon** (`:1143`), **Latias** (`:12494`), **Latios** (`:12653`).
  Todas 32x32, mesma caixa das sprites antigas (`gObjectEventBaseOam_32x32`).
- `OW_SUBSTITUTE_PLACEHOLDER TRUE`: espécie sem sprite de OW aparece como placeholder em vez de travar.
- **Save/load**: `SaveObjectEvents` guarda o `graphicsId` com os bytes trocados e marca `spriteId = 127`;
  `LoadObjectEvents` só mascara os bits altos de saves sem essa marca (`src/load_save.c:204-240`). Ou seja, objetos com
  `OBJ_EVENT_MON` sobrevivem a salvar e continuar. Não há saves de jogadores ainda, de qualquer forma.
- `applymovement` em objeto de Pokémon limpa o movimento e reseta o frame (`scrcmd.c:1286-1292`): inofensivo.
- Loja (`shop.c:892`) ignora Pokémon de OW que não são o follower ao desenhar a vitrine: só afeta NPC-Pokémon dentro de
  loja, nenhum dos casos da fase 1.
- `SetOverworldObjectSpecies` (`wild_encounter_ow.c:413`) só lê a espécie do objeto; não muda o comportamento do NPC.
- Nenhum teste (`test/`) referencia essas sprites.

---

## 2. Inventário: onde cada sprite de Pokémon do vanilla é usada

Só mapas de Hoenn. Os `*_Frlg` têm um conjunto próprio de sprites (Snorlax, Nidoran, etc.) e ficam **fora do escopo**.

### 2.1 Os casos pedidos

**Poochyena** (`OBJ_EVENT_GFX_POOCHYENA`)
| Onde | Objetos | Movimento | Observação |
|---|---|---|---|
| `MtChimney/map.json` | 10 | 9× `WALK_IN_PLACE_*`, **1× `RUN_IN_PLACE_LEFT`** em (23,19) (o Poochyena do Aqua) | os grunts Magma/Aqua do topo; scripts só `msgbox` (`Bufoh`/`Bushaa`) |
| `Route120/map.json` | 2 | `WALK_IN_PLACE_RIGHT/LEFT`, (5,18) e (6,18) | os grunts bloqueando a ponte, flag `FLAG_BADGE06_GET` |
| `Route114/map.json` | 1 | `FACE_RIGHT` | Poochyena solto em (19,12); o script (`Route114_EventScript_Poochyena`) vira para o jogador, faz `playmoncry` e fala |
| `src/data/lilycove_lady.h:54` | 1 | n/a | mon da Contest Lady (categoria Cute), ver "Contest Lady" abaixo |

**Zigzagoon que corre atrás do Birch** (`OBJ_EVENT_GFX_ZIGZAGOON_1`, sprite "EnemyZigzagoon")
| Onde | Observação |
|---|---|
| `Route101/map.json` (`LOCALID_ROUTE101_ZIGZAGOON`, (10,13)) | `JOG_IN_PLACE_LEFT` + movimentos do `scripts.inc`. **Só usa `walk_fast_*` e `walk_in_place_fast_*`** (linhas 83-132), que no follower caem nas animações certas. Seguro. |
| `src/data/lilycove_lady.h:40` | mon da Contest Lady (categoria Cool) |
| `src/cable_car.c:783` | um dos 4 "hikers" aleatórios do teleférico (só com `BUGFIX`). Sprite avulso, não objeto; ver 3.5 |

**Latias / Latios** (`OBJ_EVENT_GFX_LATIAS` / `_LATIOS`)
- **Só** `data/maps/SouthernIsland_Interior/scripts.inc:23` e `:28` (`setvar VAR_OBJ_GFX_ID_1, ...`). O objeto no `map.json`
  usa `OBJ_EVENT_GFX_VAR_1`. A aproximação é `walk_down` + `delay_16` (linhas 133-145): **seguro**. O
  `walk_in_place_faster_up` da cena é aplicado à **câmera**, não ao Lati@s.
- Nenhum outro uso de Lati@s no overworld (a antiga TV/casa não usa sprite).

**Contest Lady (Lilycove)**: `src/data/lilycove_lady.h` guarda um `.monGfxId` por categoria, e
`lilycove_lady.c:52` faz `VarSet(VAR_OBJ_GFX_ID_1, monGfxId)`. O objeto fica em `LilycoveCity_PokemonCenter_1F`.
Os 5 valores são Pokémon (Zigzagoon_1, Skitty, Poochyena, Kecleon, Pikachu); trocar a constante pela macro funciona
sem mais nada, porque o campo é `u16` e o valor (`0x4000 + espécie`) cabe.

### 2.2 Candidatos fora do que você citou (mesmo problema, decisão sua)
Mesma lógica: o NPC usa a sprite antiga e o jogador pode ter o follower da mesma espécie.

| Sprite antiga | Onde | Notas |
|---|---|---|
| `ZIGZAGOON_2` (16x16, sprite "Old") | `BattleFrontier_BattlePalaceCorridor`, `BattleFrontier_OutsideEast`, `DewfordTown_House1`, `FortreeCity_House1`, `FortreeCity_House5`, `LavaridgeTown_House`, `Route109`, `Route117`, `SlateportCity_PokemonFanClub` | é **outra** sprite que o Zigzagoon do Birch, e menor (16x16). A troca muda o tamanho visual. |
| `WINGULL` | `Route119_House` (×6), `Route104_MrBrineysHouse`, `FortreeCity_House4`, `MossdeepCity_House2`, `RusturfTunnel`, `SSTidalCorridor`, `BattlePalaceCorridor` | |
| `AZUMARILL` | `LilycoveCity_DepartmentStore_1F`, `Route117`, `SlateportCity_PokemonFanClub`, `SootopolisCity_House4`, `BattlePalaceCorridor` | |
| `AZURILL` | `FallarborTown`, `PacifidlogTown_House2` (×2), `BattlePalaceBattleRoom`, `BattlePalaceCorridor` | |
| `SKITTY` | `BattleFrontier_PokemonCenter_1F`, `FallarborTown_Mart`, `MossdeepCity_House4`, `RustboroCity_Flat2_1F`, `SlateportCity_PokemonFanClub` | |
| `PIKACHU` | `Route117`, `RustboroCity_House3`, `VerdanturfTown_FriendshipRatersHouse`, `BattlePalaceCorridor` | |
| `KIRLIA` / `DUSCLOPS` | `BattlePalaceCorridor`/`BattlePalaceBattleRoom` e `src/battle_pike.c:548,569,571` (via `VAR_OBJ_GFX_ID_1`) | |

**Recomendação: deixar como estão** (têm lógica própria, não são "pets" comuns, ou o código compara o id):
`KECLEON*` (Route 119/120, cidades: revelar o camaleão é mecânica própria, vários `map.json`), `SUDOWOODO`
(`src/item_use.c:858` compara `graphicsId == OBJ_EVENT_GFX_SUDOWOODO`), `MEW` (`src/faraway_island.c:340` compara o id),
`RAYQUAZA`/`RAYQUAZA_STILL` (`src/rayquaza_scene.c:1246`), `KYOGRE_*`/`GROUDON_*`, `REGIROCK/REGICE/REGISTEEL`,
`DEOXYS*`, `LUGIA`, `HOOH`, `VIGOROTH_*` (carregando caixa), os bonecos de decoração. São sprites de cena, não "um Pokémon
andando".

(Atualização de 2026-10-16: **Kecleon** e **Regis** foram reavaliados a pedido do usuário, ver 2.3. A recomendação acima
não vale mais para eles.)

### 2.3 Kecleon e Regis (verificado em 2026-10-16, nada alterado)

**Nenhum dos dois tem estado "ligado/desligado" na sprite.** A invisibilidade do Kecleon é do objeto, não da imagem, e os
Regis têm uma única imagem. Não há código C que compare `OBJ_EVENT_GFX_KECLEON*` ou `OBJ_EVENT_GFX_REGI*` (conferido), então
a troca por `OBJ_EVENT_GFX_SPECIES(...)` é possível. As sprites de follower dos 4 existem (`overworld.png`, 192x32 = 32x32).

**Regirock / Regice / Registeel** (1 objeto cada: `DesertRuins`, `IslandCave`, `AncientTomb`, todos `FACE_DOWN`)
- A sprite antiga é **uma imagem só, a mesma para os três** (`pokemon_old/regi.png`, `sPicTable_Regi` repete o mesmo quadro
  9×), pintada com paleta diferente em cada um; é `inanimate = TRUE`. Não existe versão "desligada".
- A de follower é a figura normal do Pokémon (de frente/costas/lado), 32x32 igual à antiga: **não muda tamanho nem posição**.
- Os scripts só usam `special StartRegiBattle` e flags, sem `applymovement`. Seguro, 3 linhas de `map.json`.
- Diferença visual: a antiga é uma pose compacta e igual nos três; a de follower é a figura individual de cada um.
  Vale olhar em jogo se continua com cara de "lendário parado".

**Kecleon** (`OBJ_EVENT_GFX_KECLEON`, sprite antiga **16x16**; a de follower é 32x32, então **o tamanho muda**)
| Onde | Objetos | Tipo | Observação |
|---|---|---|---|
| `FortreeCity` | 1, (25,8) | invisível até o Devon Scope | `FortreeCity_Movement_KecleonFlee` é só `walk_right` |
| `Route119` | 2, (31,6) e (25,15) | invisível | batalha por `data/scripts/kecleon.inc` |
| `Route120` | **5** (+1 da ponte) | invisível | os 5 em `kecleon.inc`; o da ponte (12,16) tem script próprio (cena do Steven / Devon Scope) |
| `Route117` | 1, (39,4) | **visível**, `LOOK_AROUND`, sem script | pet de NPC |
| `LilycoveCity_House1`, `SootopolisCity_House1` | 1 em cada | **visível**, `FACE_DOWN` | falam com `playmoncry` |
- Os 8 invisíveis só usam `set_visible`/`set_invisible` (`Movement_KecleonAppears`), `Common_Movement_FacePlayer` e `walk_right`:
  nenhum movimento `faster`/`fastest`, então a armadilha 3.1 não aparece.
- **O da ponte da Route120 tem um objeto-sombra** (`OBJ_EVENT_GFX_KECLEON_BRIDGE_SHADOW`, também 16x16, no mesmo (12,16)).
  Se o Kecleon da ponte virar 32x32 e a sombra ficar 16x16, os dois deixam de casar. **Decisão do usuário** (trocar só os
  outros, ou trocar os dois).
- Total contado: Fortree 1 + Route119 2 + Route120 5 (+1 ponte, +1 sombra) + 3 visíveis.
- Paleta (3.3): no máximo uma espécie extra por mapa nesses casos, sem problema.

---

## 3. Diferenças entre os dois tipos de sprite (o que pode quebrar)

### 3.1 Tabela de animação: `faster` e `fastest` viram animação de Poké Bola (**a armadilha principal**)
`sAnimTable_Following` (`src/data/object_events/object_event_anims.h:1246`) reaproveita as posições 12-19:

| Índice | Vanilla (`sAnimTable_Standard`) | Follower |
|---|---|---|
| `ANIM_STD_GO_FAST_*` (8-11) | andar rápido | andar rápido (igual) |
| `ANIM_STD_GO_FASTER_*` (12-15) | andar mais rápido | **entrar na Poké Bola** |
| `ANIM_STD_GO_FASTEST_*` (16-19) | correr | **sair da Poké Bola** |
| 20-23 (`ANIM_RUN_*`, `ANIM_EXIT_POKEBALL_FAST_*`) | fora da tabela (20 entradas) | sair da Poké Bola (rápido) |

Mapeamento de velocidade → animação (`movement_action_func_tables.h:697`): `walk`/normal → GO; `walk_fast` e
`ride_water_current` → GO_FAST; **`walk_faster` → GO_FASTER**; `walk_fastest` → GO_FASTEST.

Efeito sobre os movimentos de mapa:
- `MOVEMENT_TYPE_WALK_IN_PLACE_*` → `walk_in_place_normal` → **OK**.
- `MOVEMENT_TYPE_JOG_IN_PLACE_*` → `walk_in_place_fast` → **OK**.
- `MOVEMENT_TYPE_FACE_*`, `LOOK_AROUND` → **OK**.
- **`MOVEMENT_TYPE_RUN_IN_PLACE_*` → `walk_in_place_faster` → animação de entrar na Poké Bola: QUEBRA.**
- Nos scripts: `walk_faster_*` / `walk_in_place_faster_*` / `walk_fastest_*` num Pokémon follower tocam a animação de bola.

**Estado dos casos pedidos**:
- `Route101` (Zigzagoon): só `walk_fast`/`walk_in_place_fast`. OK.
- `SouthernIsland_Interior` (Lati@s): só `walk_down`. OK.
- `Route114`, `Route120`: `FACE_RIGHT`, `WALK_IN_PLACE_*`. OK.
- **`MtChimney` (23,19): `RUN_IN_PLACE_LEFT`. Precisa trocar o `movement_type`** para algo equivalente que não use
  "faster": `MOVEMENT_TYPE_JOG_IN_PLACE_LEFT` (a mesma pose "agitada" em velocidade `fast`). É a única exceção.
- Lilycove Lady: confirmar o movimento do objeto em `LilycoveCity_PokemonCenter_1F/map.json` (ver 5, fase 1).

### 3.2 Tamanho do sprite
As 4 espécies da fase 1 são 32x32 dos dois lados: **sem mudança de tamanho/posição**. `ZIGZAGOON_2` é 16x16; trocá-la
por follower (32x32) muda a aparência (fase 2).

### 3.3 Paleta dinâmica
A sprite antiga usa paleta fixa (`PALSLOT_NPC_SPECIAL` etc.); a de follower ocupa **um slot de paleta de objeto por espécie
distinta** na tela. Para um ou dois Pokémon por mapa é irrelevante. Atenção apenas na fase 2, em mapas com várias espécies
distintas (`BattleFrontier_BattlePalaceCorridor` tem 6, além dos NPCs). Se faltar slot, o resultado é paleta errada.

### 3.4 Comparações por id no código
`graphicsId == OBJ_EVENT_GFX_X` deixa de ser verdadeiro após a troca. Nos casos da fase 1 **não existe nenhuma**
(conferido: nenhuma comparação com `POOCHYENA`, `ZIGZAGOON_1`, `LATIAS`, `LATIOS`). Existem para `SUDOWOODO`, `MEW` e
`RAYQUAZA`, que ficam de fora por isso.

### 3.5 Teleférico (`src/cable_car.c:783`)
Cria a sprite com `CreateObjectGraphicsSprite`, que **suporta** o id de espécie (resolve a paleta dinâmica), mas a tela
do teleférico gerencia as paletas à mão. O ganho é mínimo (um hiker raro passando de relance) e o risco é de paleta errada
lá. **Recomendação: não mexer**, ou fazer só depois de testar.

### 3.6 Build: ponto a validar na primeira troca
`OBJ_EVENT_MON` é `(1u << 14)` (`event_objects.h:451`) e as macros usam `SPECIES_##name`. Os `map.json` viram assembly
(`mapjson` → `.inc`), e o upstream documenta o uso em `map.json` justamente para isso, então **deve** montar. Mesmo assim,
é a única coisa que eu não consigo comprovar sem compilar: **a primeira troca (Route101) funciona como canário do build**.
Se o assembler reclamar do sufixo `u`, o conserto é local (usar o valor numérico no `map.json` ou ajustar a macro).

### 3.6b Porymap
A doc do upstream manda digitar `OBJ_EVENT_GFX_SPECIES(X)` no campo "Sprite". Se a versão de Porymap do projeto mostra a
sprite ou um quadrado de "desconhecido" é algo que só dá para ver aí; **não afeta o jogo**, só a pré-visualização.

---

## 4. Decisões abertas (suas)

- [ ] **Escopo**: só os casos citados (fase 1) ou também os "pets" (fase 2)? A recomendação é fase 1 agora; a 2 depende de você
      gostar do resultado, principalmente do `ZIGZAGOON_2` ficar maior.
- [ ] **Lilycove Lady**: converter os 5 (Zigzagoon, Skitty, Poochyena, Kecleon, Pikachu) ou só os 2 do pedido (Zigzagoon,
      Poochyena)? Os outros 3 também aparecem como follower do jogador, então a recomendação é converter os 5 para ficar consistente.
- [ ] **Mt. Chimney (23,19)**: confirmar a troca de `RUN_IN_PLACE_LEFT` por `JOG_IN_PLACE_LEFT` (a pose "correndo no
      lugar" some; vira a andar rápido no lugar).
- [ ] **Cable car**: deixar como está (recomendado).
- [ ] **Sprites de Lati@s**: a sprite de follower dos dois é a "voando" (32x32). Como o encontro na ilha é uma cena de
      lendário, vale ver no jogo se não ficou menor/menos imponente que a antiga.

---

## 5. Plano de execução

### Fase 1: os casos citados (+ Contest Lady)
Edições (nenhuma toca C além da tabela de dados):

| # | Arquivo | Mudança |
|---|---|---|
| 1 | `data/maps/Route101/map.json` | `ZIGZAGOON_1` → `OBJ_EVENT_GFX_SPECIES(ZIGZAGOON)` (1 objeto) **(canário do build)** |
| 2 | `data/maps/MtChimney/map.json` | 10× `POOCHYENA` → `OBJ_EVENT_GFX_SPECIES(POOCHYENA)`; o do (23,19) troca também `RUN_IN_PLACE_LEFT` → `JOG_IN_PLACE_LEFT` |
| 3 | `data/maps/Route120/map.json` | 2× `POOCHYENA` |
| 4 | `data/maps/Route114/map.json` | 1× `POOCHYENA` |
| 5 | `data/maps/SouthernIsland_Interior/scripts.inc` | `OBJ_EVENT_GFX_LATIOS` → `OBJ_EVENT_GFX_SPECIES(LATIOS)` (linha 23) e `LATIAS` (linha 28) |
| 6 | `src/data/lilycove_lady.h` | os 5 `.monGfxId` → `OBJ_EVENT_GFX_SPECIES(...)` |

Depois, `grep` para garantir que nada na fase 1 ficou com a constante antiga.

**Não remover** as constantes antigas (`OBJ_EVENT_GFX_POOCHYENA` etc.) nem os `gObjectEventGraphicsInfo_*` antigos: a
fase 2 e os `_Frlg` ainda podem usá-los, e remover renumera o enum. (Se um dia sobrarem sem uso, são inofensivos.)

### Fase 2 (opcional): pets de casa
Mesma troca, em lote, nos mapas da tabela 2.2 (`ZIGZAGOON_2`, `WINGULL`, `AZUMARILL`, `AZURILL`, `SKITTY`, `PIKACHU`,
`KIRLIA`, `DUSCLOPS`). Conferir antes, para cada mapa: os movimentos (tabela 3.1), o tamanho (`ZIGZAGOON_2` é 16x16) e a
contagem de espécies distintas (3.3). `battle_pike.c` (`KIRLIA`/`DUSCLOPS`) é código C que grava em `VAR_OBJ_GFX_ID_1`:
mesma troca de constante.

### Fase 3: não fazer
Sprites de cena (Kecleon, Sudowoodo, Mew, Rayquaza, Kyogre/Groudon, Regis, Deoxys, Lugia, Ho-Oh, Vigoroth, bonecos).

---

## 6. Como testar (usuário, no emulador; eu não compilo)

Comando: `make -j20` no WSL. O `make check` não deve mudar (nenhum teste referencia essas sprites).

1. **Canário** (Route101): fazer só a troca 1, compilar. Se compilar, o mecanismo está validado no build.
2. **Route101**: começo do jogo; o Zigzagoon persegue o Birch em círculos e olha para ele no fim. Conferir direções, andar
   rápido e que a sprite é a do follower. Repetir com um Zigzagoon próprio seguindo o jogador (para ver que agora são iguais).
3. **Mt. Chimney**: subir até os grunts. 10 Poochyenas parados andando no lugar nas 4 direções; o do Aqua (23,19) **não** pode
   tocar animação de bola. Falar com um: `Bufoh!`/`Bushaa!`.
4. **Route120** (ponte, depois da 6ª insígnia) e **Route114** (o que late ao falar com ele): sprite e cry.
5. **Southern Island**: com a flauta, entrar no interior, ver o Lati@s **oposto** à cor escolhida aparecer descendo (4 + 4
   passos com pausa) e a batalha. Conferir os dois (Latias e Latios, pelo `VAR_ROAMER_POKEMON`), e que após capturar/derrotar o
   objeto some como antes (`TryRemoveLati`).
6. **Lilycove Pokémon Center 1F**: conversar com a Contest Lady até o mon dela aparecer, nas 5 categorias (dá para forçar a
   categoria pelo debug se preciso).
7. **Com follower**: ter o mesmo Pokémon seguindo o jogador (Poochyena e Zigzagoon) e ver as duas sprites lado a lado iguais.
8. **Salvar e continuar** dentro de uma dessas cenas (Mt. Chimney é a mais fácil) para conferir que as sprites sobrevivem.

---

## 7. Fatos para não reinvestigar

- Existe **uma única** função que decide tudo: `GetObjectEventGraphicsInfo` (`event_object_movement.c:3199`). O bit 14
  (`OBJ_EVENT_MON`) manda para `SpeciesToGraphicsInfo`; bits 13/12 = shiny/fêmea.
- `VAR_OBJ_GFX_ID_n` aceita id de espécie (a creche já grava um lá).
- `OBJ_EVENT_GFX_OW_MON` (`gObjectEventGraphicsInfo_Follower`, `event_object_graphics_info.h:4417`) é só o "molde" do
  follower do jogador; **não** use no `map.json`.
- `sAnimTable_Following_Asym` existe para espécies com frames de "leste" próprios; as 4 da fase 1 usam `sAnimTable_Following`.
- O `Route120_EventScript_Poochyena` está definido em `data/maps/Route110/scripts.inc:1097` (esquisitice do vanilla, o nome do
  label não bate com o arquivo); não é problema, só não procurar em `Route120/scripts.inc`.
- Arquivos soltos no repo (`pokemon_stats.csv`, `trainer_classes.csv`, `trainer_names_classes.csv`, `trainers.txt`) não são
  desta feature; ficam fora dos commits.

- [ ] Fase 1: execução pendente (aguardando as decisões da seção 4).

---

## 8. Levantamento da fase 2: pets de casa (2026-10-17)

Nada foi alterado. Conferido agora: as 8 espécies têm `overworld.png` (192x32 = quadros de 32x32). As sprites antigas de
**todas** elas são 16x16 (inclusive `ZIGZAGOON_2`, `WINGULL`, `AZUMARILL`, `AZURILL`, `SKITTY`, `PIKACHU`, `KIRLIA`,
`DUSCLOPS`), então **todo objeto trocado fica maior (32x32)**. O Azurill de Fallarbor (já feito) tinha o mesmo caso.
O Azurill de Fallarbor sai desta lista (feito na fase 1), restam 4.

### 8.1 Objetos (só `map.json` de Hoenn, sem `*_Frlg`)
| Espécie | Qtd | Mapas (x,y) / movimento |
|---|---|---|
| `ZIGZAGOON_2` | 9 | `BattleFrontier_OutsideEast` (40,56) · `DewfordTown_House1` (4,2) · `FortreeCity_House1` (2,3) · `FortreeCity_House5` (6,3) · `LavaridgeTown_House` (6,6) · `Route109` (32,6) · `Route117` (33,3) · `SlateportCity_PokemonFanClub` (3,9) · `BattlePalaceCorridor` (4,9) |
| `WINGULL` | 12 | `FortreeCity_House4` (2,3) · `MossdeepCity_House2` (4,5) · `Route104_MrBrineysHouse` (6,3, Peeko) · `RusturfTunnel` (14,4, Peeko) · `SSTidalCorridor` (7,2, Peeko) · `Route119_House` ×6 (`WANDER_AROUND`) · `BattlePalaceCorridor` (3,10) |
| `AZUMARILL` | 5 | `LilycoveCity_DepartmentStore_1F` (2,6) · `Route117` (42,2) · `SlateportCity_PokemonFanClub` (10,8) · `SootopolisCity_House4` (2,3) · `BattlePalaceCorridor` (13,9) |
| `AZURILL` | 4 | `PacifidlogTown_House2` (8,6) e (1,7) · `BattlePalaceBattleRoom` (13,1) · `BattlePalaceCorridor` (3,5) |
| `SKITTY` | 5 | `BattleFrontier_PokemonCenter_1F` (1,3) · `FallarborTown_Mart` (2,5) · `MossdeepCity_House4` (2,4) · `RustboroCity_Flat2_1F` (11,4) · `SlateportCity_PokemonFanClub` (8,3) |
| `PIKACHU` | 4 | `RustboroCity_House3` (4,4) · `VerdanturfTown_FriendshipRatersHouse` (4,4) · `Route117` (51,1) · `BattlePalaceCorridor` (15,5) |
| `KIRLIA` | 1 | `BattlePalaceCorridor` (12,6) |
| `DUSCLOPS` | 1 | `BattlePalaceBattleRoom` (13,1) |

Total: 41 objetos em `map.json`. Movimentos usados nos mapas: `FACE_*`, `LOOK_AROUND`, `FACE_LEFT_AND_RIGHT`,
`WANDER_AROUND`. Nenhum é `RUN_IN_PLACE_*`, então a armadilha 3.1 não aparece pelo `movement_type`.

### 8.2 Código C
`src/battle_pike.c`: linha 548 (`VarSet(VAR_OBJ_GFX_ID_1, OBJ_EVENT_GFX_DUSCLOPS)`), 569 e 571 (`objGfx2` = `DUSCLOPS`/`KIRLIA`,
`u16`, cabe). Nenhum outro C, script em `data/scripts` ou teste referencia essas constantes (conferido). Nenhuma comparação
por id.

### 8.3 Scripts com `faster` (a armadilha 3.1) — **só 2 casos reais**
- `FortreeCity_House4_Movement_WingullExit` (`scripts.inc:47`) termina com `walk_in_place_faster_down` **no Wingull**; depois
  vem `removeobject`. Em follower isso toca "entrar na Poké Bola" antes de sumir. Conserto de 1 linha:
  `walk_in_place_fast_down`.
- `BattlePalaceBattleRoom`: `DusclopsEnter` e `AzurillEnter` (`scripts.inc` ~linhas 410 e 420) terminam com
  `walk_in_place_faster_left` nos dois Pokémon. Mesmo conserto: `walk_in_place_fast_left`.
- **Não são problema** (conferido): `Route109` `SailToDewford` é do barco do Briney, não do Zigzagoon; `RusturfTunnel` e
  `SSTidalCorridor` têm `faster` só no jogador/Wanda/Scott/marinheiro; os Peeko usam só `walk_*`/`lock_facing_direction`.

### 8.4 Paleta (3.3) e tamanho
- Cada espécie distinta ocupa 1 slot de paleta de objeto no mapa. Mais de 2 espécies por mapa: **`BattlePalaceCorridor` (6
  espécies: Zigzagoon, Wingull, Azumarill, Azurill, Pikachu, Kirlia, mais os NPCs)**, `Route117` (3, fora o Kecleon) e
  `SlateportCity_PokemonFanClub` (3, mais NPCs). O Palace Corridor é o único com risco real de faltar slot.
- Os objetos crescem de 16x16 para 32x32: casas pequenas e `Route119_House` (6 Wingulls em `WANDER_AROUND`) podem ficar
  apertadas ou se sobrepor. Só dá para julgar em jogo.

### 8.5 Decisões suas
- [ ] **Escopo**: converter todas as 8 espécies, ou só algumas (por exemplo, deixar fora Wingull ou Zigzagoon_2)?
- [ ] **`BattlePalaceCorridor`** (6 espécies, risco de paleta): deixar de fora (recomendado) ou converter e ver em jogo?
- [ ] **`BattlePalaceBattleRoom`** (Azurill + Dusclops, `walk_in_place_faster_left` → `fast`): converter com o ajuste ou
      deixar de fora?
- [ ] **`FortreeCity_House4`**: converter e trocar o `faster` por `fast` no `WingullExit` (recomendado)?
- [ ] **`battle_pike.c`** (Kirlia/Dusclops, 3 linhas): converter ou deixar? É sala de desafio, o ganho é pequeno.
- [ ] Ordem: tudo de uma vez, ou por espécie/mapa para você testar em partes (recomendado: por espécie).

### 8.6 Lista por local (2026-10-17, depois da decisão de ignorar a Battle Frontier)
**Fora do escopo por decisão do usuário:** `BattleFrontier_OutsideEast`, `BattleFrontier_PokemonCenter_1F`,
`BattleFrontier_BattlePalaceCorridor`, `BattleFrontier_BattlePalaceBattleRoom` e `src/battle_pike.c`. Sobram **31 objetos
em 20 mapas**, só `map.json` (e 1 linha de script em Fortree). Nenhum mapa tem `RUN_IN_PLACE`.

| Local (mapa) | Objeto (x,y), movimento | Troca |
|---|---|---|
| Dewford Town, casa (`DewfordTown_House1`) | Zigzagoon (4,2) `FACE_DOWN` | `ZIGZAGOON_2` → `SPECIES(ZIGZAGOON)` |
| Fallarbor Town, loja (`FallarborTown_Mart`) | Skitty (2,5) `LOOK_AROUND` | `SKITTY` → `SPECIES(SKITTY)` |
| Fortree City, casa 1 (`FortreeCity_House1`) | Zigzagoon (2,3) `FACE_LEFT` | `ZIGZAGOON_2` → `SPECIES(ZIGZAGOON)` |
| Fortree City, casa 4 (`FortreeCity_House4`) | Wingull (2,3) `FACE_DOWN`, flag `FLAG_HIDE_FORTREE_CITY_HOUSE_4_WINGULL` | `WINGULL` → `SPECIES(WINGULL)` **e** `scripts.inc:47-53`, `WingullExit`: último passo `walk_in_place_faster_down` → `walk_in_place_fast_down` |
| Fortree City, casa 5 (`FortreeCity_House5`) | Zigzagoon (6,3) `FACE_DOWN` | `ZIGZAGOON_2` → `SPECIES(ZIGZAGOON)` |
| Lavaridge Town, casa (`LavaridgeTown_House`) | Zigzagoon (6,6) `LOOK_AROUND` | `ZIGZAGOON_2` → `SPECIES(ZIGZAGOON)` |
| Lilycove City, loja de departamentos 1F (`LilycoveCity_DepartmentStore_1F`) | Azumarill (2,6) `LOOK_AROUND` | `AZUMARILL` → `SPECIES(AZUMARILL)` |
| Mossdeep City, casa 2 (`MossdeepCity_House2`) | Wingull (4,5) `FACE_UP`, flag `FLAG_HIDE_MOSSDEEP_CITY_HOUSE_2_WINGULL` | `WINGULL` → `SPECIES(WINGULL)` |
| Mossdeep City, casa 4 (`MossdeepCity_House4`) | Skitty (2,4) `LOOK_AROUND` | `SKITTY` → `SPECIES(SKITTY)` |
| Pacifidlog Town, casa 2 (`PacifidlogTown_House2`) | Azurill feliz (8,6) e Azurill triste (1,7), `LOOK_AROUND` | 2× `AZURILL` → `SPECIES(AZURILL)` |
| Route 104, casa do Mr. Briney (`Route104_MrBrineysHouse`) | Wingull/Peeko (6,3) `FACE_DOWN`, flag `FLAG_HIDE_BRINEYS_HOUSE_PEEKO` | `WINGULL` → `SPECIES(WINGULL)` |
| Route 109 | Zigzagoon (32,6) `LOOK_AROUND` | `ZIGZAGOON_2` → `SPECIES(ZIGZAGOON)` |
| Route 117 | Zigzagoon (33,3), Azumarill (42,2), Pikachu (51,1), todos `LOOK_AROUND` (3 espécies novas no mapa, mais o Kecleon antigo) | 3 trocas |
| Route 119, casa (`Route119_House`) | 6 Wingulls `WANDER_AROUND`: (1,6) (0,4) (2,2) (8,5) (6,6) (5,3) | 6× `WINGULL` → `SPECIES(WINGULL)` |
| Rustboro City, apto 2 1F (`RustboroCity_Flat2_1F`) | Skitty (11,4) `FACE_LEFT` | `SKITTY` → `SPECIES(SKITTY)` |
| Rustboro City, casa 3 (`RustboroCity_House3`) | Pikachu (4,4) `FACE_DOWN` | `PIKACHU` → `SPECIES(PIKACHU)` |
| Rusturf Tunnel (`RusturfTunnel`) | Wingull/Peeko (14,4) `FACE_LEFT`, flag `FLAG_HIDE_RUSTURF_TUNNEL_PEEKO` (cena do Briney; só usa `walk_*`) | `WINGULL` → `SPECIES(WINGULL)` |
| Slateport City, Pokémon Fan Club (`SlateportCity_PokemonFanClub`) | Zigzagoon (3,9), Azumarill (10,8), Skitty (8,3), todos `LOOK_AROUND` (3 espécies novas no mapa, mais os NPCs) | 3 trocas |
| SS Tidal, corredor (`SSTidalCorridor`) | Wingull/Peeko (7,2) `LOOK_AROUND`, flag `FLAG_HIDE_SS_TIDAL_CORRIDOR_MR_BRINEY` | `WINGULL` → `SPECIES(WINGULL)` |
| Sootopolis City, casa 4 (`SootopolisCity_House4`) | Azumarill (2,3) `LOOK_AROUND` | `AZUMARILL` → `SPECIES(AZUMARILL)` |
| Verdanturf Town, casa do Friendship Rater (`VerdanturfTown_FriendshipRatersHouse`) | Pikachu (4,4) `FACE_DOWN` | `PIKACHU` → `SPECIES(PIKACHU)` |

Resumo por espécie: Zigzagoon 7, Wingull 11, Azumarill 4, Azurill 2, Skitty 4, Pikachu 3 = **31**. Mantém-se o que já era
regra: não remover constantes nem `gObjectEventGraphicsInfo_*` antigos.

Pontos de atenção na checagem: (1) todo objeto cresce de 16x16 para 32x32, principalmente nas casas pequenas e nos 6
Wingulls da `Route119_House`; (2) `Route117` e `Slateport Fan Club` têm 3 espécies de follower cada (paleta); (3) o
`FortreeCity_House4` é o único com mudança de script; (4) vale conferir o cry e a fala de cada um, que não mudam.

