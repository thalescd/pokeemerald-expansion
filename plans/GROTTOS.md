# Plano: GROTTOS

- **Branch:** `plan/grottos`
- **Status:** em andamento
- **Criado em:** 2026-11-17
- **Escopo (pode alterar):** o sistema de secret base inteiro (código, scripts, mapas interiores e tudo que o consome:
  TV, record mixing, batalhas, PC etc.); o sistema de decoração, reduzido ao quarto do jogador; o layout do
  `SaveBlock1`; as tabelas de encontro e a ferramenta que as gera; o código novo das grottos e os ganchos de passos e
  reset diário.
- **Fora de escopo:** trocar o conteúdo das lojas e fontes de decoração (só listar); mover as entradas ou redesenhar os
  mapas interiores (eles podem ganhar objetos e scripts); DexNav; outras partes do save que não sejam secret base e
  decoração.

Este plano só pode ser trabalhado na branch acima. As regras de trabalho (levantamento antes de executar, anotar o
progresso aqui, uma decisão por vez) estão no `CLAUDE.md`.

Valores de **Status**: `planejado`, `em andamento`, `concluído`, `abandonado`.

---

## Objetivo

Trocar as secret bases por algo no estilo das Hidden Grottos de Black 2 / White 2. As passagens abertas com Secret Power
continuam onde estão e levam aos mesmos mapas, mas lá dentro, em vez da opção de montar uma base, há um Pokémon selvagem
com hidden ability que se renova com o tempo.

O sistema de secret base em si não interessa. O ganho maior é o espaço que ele ocupa no `SaveBlock1` (cerca de 3,2 KB só
em `secretBases[20]`), que pode ir para conteúdo mais importante. As grottos custam cerca de 41 bytes.

## Decisões

- 2026-11-16: reaproveitar só a abertura de passagem com Secret Power, as passagens e os mapas interiores. O resto do
  sistema de secret base sai.
- 2026-11-16: as 75 entradas de secret base viram grottos, uma grotto por entrada.
- 2026-11-16: o interior fica como está; no lugar da opção de secret base aparece um Pokémon. Só Pokémon, sem itens.
- 2026-11-16: hidden ability em 100% dos Pokémon de grotto.
- 2026-11-17: a passagem exige Secret Power toda vez. Não se guarda no save quais passagens foram abertas.
- 2026-11-17: decorações continuam existindo só no quarto do jogador, e só com as categorias que ele já aceita
  (bonecos e almofadas). As outras categorias saem.
- 2026-11-17: as lojas e fontes de decoração só são listadas no plano; o usuário decide o que colocar no lugar.
- 2026-11-17: regeneração híbrida. Todas as grottos enchem no reset diário e, além disso, a cada 256 passos cada grotto
  vazia tem 5% de chance de encher (valores do BW2). Os 5% valem direto, sem o fator de 20% de Pokémon do BW2, porque aqui
  só existe Pokémon.
- 2026-11-17: espécie e nível vêm de uma tabela própria por rota (opção C), com 5 slots e pesos 60/30/5/4/1 como a do
  surf. Uma grotto específica pode ganhar tabela própria se precisar.
- 2026-11-17: o slot sorteado é escolhido quando a grotto regenera e fica salvo, para que entrar e sair não troque o
  Pokémon.
- 2026-11-17: o Pokémon de grotto pode vir shiny pelas regras normais (sem shiny lock).
- 2026-11-17: a grotto esvazia quando o jogador foge, derrota ou captura o Pokémon.
- 2026-11-17: um plano único cobre tudo: grottos, remoção da secret base, redução das decorações e encolhimento do
  `SaveBlock1`. Para o usuário, as grottos só estão prontas quando a secret base sair do save; o espaço liberado é o
  objetivo principal, por causa da gen 10 (espécies, golpes e habilidades novas).
- 2026-11-17: whiteout na batalha da grotto também esvazia a grotto (esvaziar antes de começar a batalha).
- 2026-11-17: sem popup de nome ao entrar na grotto. Hoje o popup não aparece na primeira entrada numa base
  (`SecretBaseMapPopupEnabled`, `secret_base.c:452-458`), e toda entrada em grotto é desse tipo. Os 24 interiores
  passam a ter `show_map_name: false`.
- 2026-11-17: decoração de categoria removida, comprada ou ganha, cai no caminho de "sem espaço" que já existe
  (inventário de tamanho 0). Nenhum script de loja ou prêmio muda neste plano.
- 2026-11-17: o código de colocação de móveis que não são sprite é apagado (não funciona no quarto).
- 2026-11-17: diálogos de NPC que mencionam secret base ficam anotados em Pendências para o usuário revisar depois.
- 2026-11-17: aceito o fim do record mixing com Ruby/Sapphire/Emerald originais.
- 2026-11-17: tabelas das grottos num campo novo `grotto_mons` do `src/data/wild_encounters.json`.
- 2026-11-17: o local de encontro na ficha do Pokémon é o nome da rota da entrada, não "SECRET BASE". O local é gravado
  quando o Pokémon é criado (`pokemon.c:1010-1011`, mapsec do mapa atual, que seria o interior); o special da grotto
  sobrescreve com o mapsec da rota guardada no `dynamicWarp`.
- 2026-11-17: o NPC do Route 111 que dá a TM do Secret Power (`data/scripts/secret_power_tm.inc`) passa a explicar as
  hidden grottos, com o mesmo papel de antes. Só o texto muda; script e movimentos ficam. Texto aprovado no mesmo dia.
- 2026-11-17: levantamento aprovado; começar pela fase 1.
- 2026-11-17: estado das grottos (38 bytes) recortado do `filler1` do `SaveBlock1`, não no lugar de `secretBases` como
  o levantamento previa. A fase 1 não muda tamanho nem offsets do save; o `test/save.c` continua valendo.
- 2026-11-17: vars livres 0x40F7 (`VAR_HIDDEN_GROTTO_ID`) e 0x40F8 (`VAR_HIDDEN_GROTTO_STEP_COUNTER`), não a 0x4054
  como o levantamento propunha: na fase 1 o código da secret base ainda lê a 0x4054 como índice de `secretBases[]`.
- 2026-11-17: conteúdo provisório das 13 tabelas `grotto_mons` derivado da rota (grama; surf nas rotas 125 e 127). O
  usuário edita depois no Porymap.
- 2026-11-17: o reset diário sorteia um Pokémon novo também para as grottos que ainda estavam cheias (aprovado).
- 2026-11-17: fundo de batalha (`MAP_BATTLE_SCENE_FRONTIER`) e música (`MUS_FORTREE`) dos interiores ficam como estão
  por enquanto.

## Levantamento

**Obrigatório antes de executar. Aprovado pelo usuário em:** _(ainda não)_

Feito em 2026-11-17, só leitura, sem compilar. Os números de linha valem para o commit `543d86b799`.

### Números

- `struct SecretBase` = 160 bytes (os enums `Move`/`Species`/`Item` são `packed` e ocupam 2 bytes). `secretBases[20]` =
  3200 bytes (`include/global.h:1145`).
- Inventários de decoração que saem (desk, chair, plant, ornament, mat, poster): 100 bytes (`global.h:1148-1153`).
  Ficam `decorationDolls[40]`, `decorationCushions[10]` e o quarto (24 bytes).
- Estado das grottos: 75 × 4 bits (1 de "tem Pokémon" + 3 do slot) ≈ 38 bytes, mais 1 var de contador de passos.
- Saldo: cerca de 3,26 KB livres no `SaveBlock1`. Saves existentes deixam de ser compatíveis (permitido pela decisão de
  não congelar o layout).

### Fase 1 — grottos

**Entrada (reaproveitada).** Entrar e sair de um interior não depende de `secretBases[]`: só do ID da entrada e do
`gSaveBlock1Ptr->dynamicWarp`, que é genérico.
- Botão A na entrada: `src/field_control_avatar.c:466-473`. Hoje só abre se `TrySetCurSecretBase()` disser que o ID
  não está em `secretBases[]`; passa a abrir sempre.
- Secret Power pelo menu: `SetUpFieldMove_SecretPower` (`src/fldeff_misc.c:532-569`) recusa se o jogador já tem base
  (`:536`); esse bloqueio sai.
- Script: `SecretBase_EventScript_CheckEntrance` e os blocos Cave/Tree/Shrub (`data/scripts/secret_base.inc:27-130`)
  ficam, sem o desvio `AlreadyHasSecretBase` (`:187-216`). `InitSecretBase` (`:132-144`) fica reduzido a fechar a
  mensagem, tocar `SE_EXIT` e entrar.
- Animações `FldEff_SecretPower*` e `ToggleSecretBaseEntranceMetatile` (`secret_base.c:320-350`) ficam como estão.
- Warp de entrada: `EnterSecretBase`/`Task_EnterSecretBase`/`SetSecretBaseWarpDestination` (`secret_base.c:411-450`),
  sem as linhas de `numTimesEntered` (`:430-432`). Saída pelo warp `MAP_DYNAMIC` de sempre.
- `SetOccupiedSecretBaseEntranceMetatiles` (`secret_base.c:380-409`, chamada em `fieldmap.c:138,146`) sai: com Secret
  Power toda vez, nenhuma entrada fica aberta.
- O ID da entrada hoje vive só em EWRAM (`sCurSecretBaseId`, `secret_base.c:77`). Ele passa a ser gravado numa var
  persistente (proposta: reaproveitar `VAR_CURRENT_SECRET_BASE` 0x4054), senão salvar e recarregar dentro da grotto
  perde qual grotto é. Os IDs são esparsos (1..233), então precisa de uma tabela ID → índice 0..74.

**Interior.** Os 24 mapas compartilham os scripts de `data/scripts/shared_secret_base.inc` (`"shared_scripts_map":
"SecretBase"`), que serão reescritos para a grotto.
- Cada mapa já tem um objeto com sprite dinâmico (`OBJ_EVENT_GFX_VAR_F`, flag `FLAG_HIDE_SECRET_BASE_TRAINER`, script
  do treinador de record mixing). Ele vira o Pokémon: no `ON_TRANSITION`, um special põe
  `VAR_OBJ_GFX_ID_F = espécie | OBJ_EVENT_MON` e liga/desliga a flag conforme a grotto esteja cheia. Precedente:
  `data/maps/SouthernIsland_Interior/scripts.inc:16-29`.
- Os objetos 2-15 (slots de decoração) e o PC saem dos 24 `map.json`.
- O jogador chega no tile da seta de saída; o `walk_up` ×2 do primeiro acesso pode continuar.
- Nome no popup: hoje `GetMapName` (`region_map.c:1896-1898`) monta "<dono>'s BASE" a partir de `secretBases[]`. Esse
  ramo sai e o nome vem da tabela de map sections.

**Encontro.** Precedente completo em `data/maps/AquaHideout_B1F/scripts.inc:6-52`.
- Criar o Pokémon com um special novo copiando `CreateDexNavWildMon` (`src/dexnav.c:1180-1199`): `CreateWildMon` +
  `MON_DATA_ABILITY_NUM = 2`. Synchronize/Cute Charm valem; shiny pelas rolagens normais
  (`ComputePlayerShinyOdds`, `pokemon.c:871`). Espécie sem HA cai na habilidade normal, sem assert.
- `dowildbattle` e depois `GetBattleOutcome`. Qualquer resultado esvazia a grotto. Exceção: no whiteout o script não
  volta (`battle_setup.c:724-729`) — ver Pendências.

**Tabelas.** Campo novo `grotto_mons` no `src/data/wild_encounters.json`, com pesos 60/30/5/4/1, nas 13 rotas.
- O gerador (`tools/wild_encounters/wild_encounters_to_header.py`) é genérico: gera as tabelas e os macros
  `ENCOUNTER_CHANCE_GROTTO_MONS_*` sem mudança. Só falta um membro `grottoMonsInfo` em `WildEncounterTypes`
  (`include/wild_encounter.h:31-38`), como o tutorial `docs/tutorials/how_to_time_of_day_encounters.md:107-121` mostra.
- O Porymap lê os `fields` do json, então as tabelas ficam editáveis lá.
- A tabela é lida pela rota da entrada (não pelo mapa atual, que é o interior).
- A substituição por grotto fica numa tabela C pequena, no estilo de `src/mass_outbreak.c:11-40`.
- Rolagens com `RandomWeighted` e `RandomPercentage` (`include/random.h`), com tags novas `RNG_GROTTO_*`.

**Regeneração.**
- Diária: chamada nova em `DoDailyEvents` (`src/clock.c:47-66`). Ela só roda com o relógio acertado, então o
  enchimento inicial vai em `NewGameInitData` (`src/new_game.c:189`, no lugar de `ClearSecretBases()`).
- Passos: função nova ao lado de `UpdateFriendshipStepCounter` em `TryStartStepCountScript`
  (`field_control_avatar.c:755-759`), contador módulo 256 numa var livre (22 `VAR_UNUSED_*`, ex.: 0x40F7).
- Estado: campo novo no `SaveBlock1`, no lugar de `secretBases`. Daily flags não servem (só 64, e o enchimento precisa
  sortear o slot).

### Fase 2 — remoção da secret base

- **Some inteiro:** `src/secret_base.c` (2076 linhas) exceto o que a fase 1 reaproveita; `include/secret_base.h`;
  `data/scripts/secret_base.inc` exceto a entrada; `data/text/secret_base_trainers.inc`; os programas de TV Secret Base
  Visit e Secret Base Secrets (`src/tv.c` e `data/text/tv.inc`, ~57 textos); os helpers de batalha em `pokemon.c:601-616`
  e `:3114-3156`; `PopSecretBaseBalloon` e `ShatterSecretBaseBreakableDoor` (`fldeff_misc.c:835-935`); 
  `OverrideSecretBaseDecorationSpriteScript` (`event_object_movement.c:3785-3800`); strings do registro
  (`strings.c:352-356`).
- **Edições pequenas (~45 pontos em ~25 arquivos):**
  - Batalha (~18): `battle.h:313`, `battle_util2.c`, `battle_message.c`, `battle_controller_opponent.c`,
    `battle_script_commands.c`, `battle_util.c`, `battle_ai_*.c`, `battle_setup.c`, `battle_special.c`, `data.h:251`,
    `data/battle_scripts_1.s:2918`.
  - Record mixing (8): `src/record_mixing.c`.
  - TV (6 cases do switch).
  - Campo: `overworld.c:950,987,2102,2355`, `fieldmap.c`, `field_tasks.c:34,68`, `field_control_avatar.c:298-301,608-634`,
    `fldeff_misc.c:1106-1169`, `item.c:387-391`, `region_map.c`, `new_game.c`.
  - Scripts: `data/specials.inc`, `data/event_scripts.s`, `src/data/script_menu.h` (menus do PC da base).
  - NPC `data/maps/MossdeepCity_House4/scripts.inc:7-28` (pergunta se você tem base e onde): precisa de outro diálogo.
- **Constantes:** IDs de enum que outros números seguem (`TVSHOW_*`, `STEP_CB_SECRET_BASE`, `SPECIAL_BATTLE_*`,
  `GAME_STAT_MOVED_SECRET_BASE`) ficam como slot vazio, sem renumerar.
- **Ficam livres:** vars 0x4026, 0x4089, 0x4097, 0x40EC-0x40F0; flags 0xAE-0xBB (decoração 1-14 só se o quarto não
  usar), 0x10C, `FLAG_DAILY_SECRET_BASE`.

### Fase 3 — decorações só no quarto

- Saem os 6 arrays de inventário (`global.h:1148-1153`) e as linhas `SET_DECOR_INV` correspondentes
  (`src/decoration_inventory.c:8-22`).
- O menu de categorias assume 8 categorias (`decoration.c:788,796,864,872`), e `DECOR_NONE` está na categoria DESK
  (`src/data/decoration/header.h:3-16`).
- Saem os ramos da base em `decoration.c` (`:110`, `:598-602`, `:659-667`, `:759-763`, `:1124-1212`, `:1700`, `:2317`) e,
  opcionalmente, o código de colocação de móveis que não são sprite (só servia para a base).
- Os IDs de decoração (`include/constants/decorations.h`) não são renumerados.

### Fase 4 — save

- Remover `secretBases`, `struct SecretBase`/`SecretBaseParty` e `SECRET_BASES_COUNT`. O `STATIC_ASSERT` de tamanho
  (`src/save.c:82`) continua valendo. Medir os offsets com sonda de `offsetof` (os comentários de `global.h` mentem).

### O que é compartilhado e pode quebrar

- **Quarto do jogador:** usa funções e scripts com nome de secret base que **não podem ser apagados**:
  `InitSecretBaseDecorationSprites` (ramo `!CurMapIsSecretBase()`, `secret_base.c:563-568`),
  `HideSecretBaseDecorationSprites` (`:634-651`), `SecretBase_EventScript_SetDecorationFlags`/`InitDecorations`/
  `SetDecoration`/`PutAwayDecoration` (`secret_base.inc:233-282`), `VAR_SECRET_BASE_INITIALIZED`, os specials
  `specials.inc:38,39,43` e as flags `FLAG_DECORATION_*`. Chamados de `LittlerootTown_BrendansHouse_2F` (editar o
  `scripts.pory`) e `LittlerootTown_MaysHouse_2F`. Serão movidos para arquivos de decoração.
- **TM do Secret Power:** `data/scripts/secret_base.inc:663` inclui `data/scripts/secret_power_tm.inc` (NPC do Route 111).
  Apagar o arquivo sem mover o include quebra a obtenção do golpe.
- **Easy Chat:** `EC_WORD_SECRET_BASE` fica (tabelas sensíveis a índice, usada no Battle Tent e no bardo).
- **`MAP_TYPE_SECRET_BASE` e `MAPSEC_SECRET_BASE`** ficam: dão ambiente de batalha "building" (`battle_setup.c:791`),
  cursor do mapa regional pelo `dynamicWarp` (`region_map.c:1275-1283`), transições de flash e o programa de TV do rival
  (`tv.c:5295`).
- **Record mixing com Ruby/Sapphire/Emerald originais** deixa de funcionar (o pacote muda de formato). Entre cópias deste
  ROM continua consistente.
- **`gDecorations`** é exportado no header da ROM (`rom_header_gf.c:33,112`); manter a numeração evita surpresas.

### O que não será tocado

- Posição das 75 entradas, os bg events, `include/constants/secret_bases.h` e o suporte em `tools/mapjson`.
- Layouts e tilesets dos 24 interiores e o tileset de secret base (os interiores usam).
- Conteúdo das lojas e prêmios de decoração (só listados abaixo).
- DexNav, Easy Chat, o restante do save.

### Fontes de decoração (para o usuário trocar)

Decoração que continua útil (boneco ou almofada) não precisa mudar. As que viram inúteis:

| Onde | O quê | Categoria |
|---|---|---|
| `LilycoveCity_DepartmentStore_5F/scripts.inc:72-87` | 9 pôsteres (balconista do meio à direita) | Poster |
| `LilycoveCity_DepartmentStore_5F/scripts.inc:95-111` | 10 tapetes (balconista da direita) | Mat |
| `LilycoveCity_DepartmentStoreRooftop/scripts.inc:31-42` | 12 ornamentos da liquidação (os 2 bonecos `:43-44` ficam) | Ornament |
| `FortreeCity_DecorationShop/scripts.inc:24-31, 46-53` | loja inteira: 8 mesas e 8 cadeiras | Desk, Chair |
| `SlateportCity/scripts.inc:509-522` | 6 tijolos/balões e 8 tapetes de nota (os bonecos `:486-488` ficam) | Ornament, Mat |
| `Route104_PrettyPetalFlowerShop/scripts.inc:54-59` | 6 plantas | Plant |
| `BattleFrontier_ExchangeServiceCorner/scripts.inc:110` | KISS_POSTER (o resto da loja é boneco/almofada) | Poster |
| `BattleFrontier_ScottsHouse/scripts.inc:99, 129` | escudos de prata e ouro (sequências de 50 e 100 vitórias) | Ornament |
| `LilycoveCity_LilycoveMuseum_2F/scripts.inc:125` | GLASS_ORNAMENT | Ornament |
| `Route113_GlassWorkshop/scripts.inc:133-157, 206, 268-281` | PRETTY_CHAIR e PRETTY_DESK (troca de cinzas) | Chair, Desk |
| `Route110_TrickHouseEnd/scripts.inc:172-176` e `Route110_TrickHouseEntrance/scripts.inc:422-426` | RED_TENT / BLUE_TENT | Ornament |
| `data/scripts/mauville_man.inc:148-215` + `src/trader.c:26-32` | Trader de Mauville: troca qualquer decoração; padrões incluem TIRE e PRETTY_FLOWERS | vários |

Menus que acompanham esses prêmios: `field_specials.c:2495-2543`, `src/data/battle_frontier/battle_frontier_exchange_corner.h`,
`MULTI_TENT` em `src/data/script_menu.h:473-476`. Texto que só menciona decoração: placa de dicas em
`Route119/scripts.inc:465-466,603-605`, NPC da loja de Fortree.

## Pontos de código

### Fase 1 (escrita em 2026-11-17)

- **Novo:** `src/hidden_grotto.c`, `include/hidden_grotto.h`, `data/scripts/hidden_grotto.inc` (map scripts
  `HiddenGrotto_MapScripts` e o script do Pokémon).
- **Save:** `include/global.h` — `hiddenGrottoStates[38]` recortado do `filler1`; `include/constants/global.h` —
  `HIDDEN_GROTTO_COUNT`, `HIDDEN_GROTTO_STATE_BYTES`.
- **Vars:** `VAR_HIDDEN_GROTTO_ID` (0x40F7), `VAR_HIDDEN_GROTTO_STEP_COUNTER` (0x40F8).
- **Tabelas:** campo `grotto_mons` em `src/data/wild_encounters.json` (13 rotas); membro `grottoMonsInfo` em
  `include/wild_encounter.h`; `ChooseWildMonIndex_Grotto` e `GetGrottoMonsInfo` em `src/wild_encounter.c`.
- **RNG:** `RNG_HIDDEN_GROTTO_SLOT`, `_REFILL`, `_LEVEL` em `include/random.h`.
- **Ganchos:** `DoDailyEvents` (`src/clock.c`), `NewGameInitData` (`src/new_game.c`), `TryStartStepCountScript`
  (`src/field_control_avatar.c`).
- **Entrada:** botão A sempre abre (`field_control_avatar.c`); Secret Power pelo menu sem o bloqueio de "já tem base"
  (`fldeff_misc.c`); `CheckEntrance` sem o desvio de mudança de base e `InitSecretBase` reduzido
  (`data/scripts/secret_base.inc`); `EnterSecretBase` grava a grotto e `Task_EnterSecretBase` não conta mais visitas
  (`secret_base.c`); `InitSecretBaseAppearance` só troca o PC por chão; `SetOccupiedSecretBaseEntranceMetatiles` não é
  mais chamada (`fieldmap.c`).
- **Interiores:** os 24 `map.json` — `shared_scripts_map: HiddenGrotto`, `show_map_name: false`, objeto 1 com
  `HiddenGrotto_EventScript_Pokemon`, objetos de decoração 2-15 removidos.
- **Specials:** `SetupHiddenGrottoInterior`, `CreateHiddenGrottoMon`, `ClearHiddenGrottoMon` no fim de
  `data/specials.inc`.
- **Texto:** NPC do Route 111 (`data/scripts/secret_power_tm.inc`).

Nomes herdados que a fase 2 renomeia: `FLAG_HIDE_SECRET_BASE_TRAINER` (esconde o Pokémon), `VAR_INIT_SECRET_BASE`
(controla os 2 passos de entrada), `SecretBase_EventScript_CheckEntrance` e os blocos Cave/Tree/Shrub.

## Progresso

- [x] Levantamento aprovado (2026-11-17)
- [x] Fase 1: implementação, build, `make check`, teste no emulador, commit
- [ ] Fase 2: remoção da secret base
- [ ] Fase 3: decorações só no quarto
- [ ] Fase 4: encolher o `SaveBlock1`
- [ ] Merge na `master`

- 2026-11-17: levantamento escrito (4 frentes: entrada, dependentes, encontro, decoração). Só leitura de código, nada
  compilado. Aguardando aprovação e as decisões em Pendências.
- 2026-11-17: fase 1 escrita (ver Pontos de código). **Não compilada e não testada.** Conferido só por leitura: o JSON
  só ganhou linhas (round-trip idêntico), os 24 `map.json` mantêm o formato, nenhum Pokémon fica no caminho dos 2 passos
  de entrada, e nenhum script do repo usa batalha selvagem dupla roteirizada (o `dowildbattle` da grotto sempre é
  simples). Escolha de implementação: o reset diário sorteia um Pokémon novo também para as grottos que ainda estavam
  cheias.
- 2026-11-17: fase 1 — build e `make check` passaram (rodados pelo usuário). Teste no emulador pendente.
- 2026-11-17: fase 1 testada no emulador pelo usuário, jogo novo. Passos 2 a 11 do roteiro funcionaram, inclusive
  hidden ability, local de encontro, esvaziar ao fugir, whiteout, save/load e o enchimento por passos. Testado sem som,
  então o grito e a música não foram ouvidos. **Não testados:** passo 1 (texto do NPC do Route 111) e o enchimento pelo
  reset diário (avançar o dia).

## Pendências

- Fase 1, ainda não testado: texto do NPC do Route 111 (passo 1) e enchimento pelo reset diário (passo 10).

Para o usuário revisar depois (fora deste plano):

- Lojas e prêmios de decoração da tabela "Fontes de decoração" no Levantamento.
- Diálogos que mencionam secret base ou decoração de base:
  - `data/maps/MossdeepCity_House4/scripts.inc:7-28, 51-58`: pergunta se você tem base e onde. Este precisa de lógica
    nova, não só texto, porque chama `CheckPlayerHasSecretBase` e `GetSecretBaseNearbyMapName`, que serão removidos.
    Até o usuário decidir, ele fica sempre no ramo "você ainda não tem base".
  - `data/maps/Route119/scripts.inc:462-466, 591-605`: placa de Trainer Tips e falas sobre decorar a base.
  - `data/maps/SlateportCity/scripts.inc:191, 201`: menina que, depois do Secret Power, fala de comprar tijolos.
  - `data/maps/FortreeCity_DecorationShop/scripts.inc`: falas dos NPCs da loja.
  - `data/text/mauville_man.inc:61` e `src/mauville_old_man.c:70`: pergunta do Giddy "SECRET BASES are so wonderful".
  - Storyteller de Mauville (`mauville_old_man.c:1060`, `data/scripts/mauville_man.inc:496-505`): a história de
    "mudou de base" fica inalcançável.

## Como testar

### Fase 1

Build: `make -j20`. O `test/save.c` deve continuar passando sem mudar o número (o `SaveBlock1` não muda de tamanho).

Precisa de um **jogo novo** (o enchimento inicial acontece no new game) ou de um save onde o reset diário já rodou.

1. Route 111: falar com o NPC do Secret Power. Conferir o texto novo (linhas cabem na caixa?) e que ele entrega a TM.
2. Numa entrada (árvore, parede com fenda ou arbusto) virado para o norte, apertar A: pergunta do Secret Power, animação,
   mensagem de descoberta, entra no interior e anda 2 passos para cima. Sem popup de nome.
3. Repetir pela opção Secret Power no menu do Pokémon, de frente para outra entrada.
4. Dentro: não há PC, não há decoração; o Pokémon da tabela da rota aparece com o sprite certo.
5. Falar com ele: grito, batalha simples. Conferir que a habilidade é a hidden ability da espécie.
6. Capturar: ficha mostra o nome da rota como local de encontro, não "SECRET BASE".
7. Fugir em outra grotto: o Pokémon some; sair e entrar de novo: continua vazia.
8. Perder a batalha numa grotto (whiteout): voltar lá, deve estar vazia.
9. Salvar dentro de uma grotto cheia, recarregar: o mesmo Pokémon continua lá.
10. Grotto vazia: andar uns milhares de passos fora (5% a cada 256 por grotto) e conferir que às vezes enche; ou avançar
    o dia pelo menu de debug e conferir que todas enchem.
11. Sair da grotto pela seta: volta para a rota, na frente da entrada, e a entrada aparece fechada.
12. Ambiente da batalha: fundo `MAP_BATTLE_SCENE_FRONTIER` e música `MUS_FORTREE` vêm do mapa antigo; avaliar se servem.
