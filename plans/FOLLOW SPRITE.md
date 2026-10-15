# Plano: usar as sprites de follower nos Pokémon do overworld (NPCs e cenas)

- **Branch:** `plan/follow-sprite`
- **Status:** planejado
- **Criado em:** 2026-10-15
- **Escopo (pode alterar):** <arquivos, pastas ou sistemas que este plano pode tocar>
- **Fora de escopo:** <o que não deve ser tocado>

Este plano só pode ser trabalhado na branch acima. As regras de trabalho (levantamento antes de executar, anotar o
progresso aqui, uma decisão por vez) estão no `CLAUDE.md`.

Valores de **Status**: `planejado`, `em andamento`, `concluído`, `abandonado`.

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
