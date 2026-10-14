# Plano: Eon Flute + evento dos Lati@s

Documento de continuidade da branch `feature/eon-flute`. O soar (seção 1) está commitado e testado. O evento
descrito abaixo **está implementado e commitado**: o build compila e o `make check` passa, mas **ainda não foi
testado no emulador** (ver "Progresso da implementação").

Última atualização: 2026-10-14.

## Progresso da implementação

Iniciada e concluída em 2026-10-14. Eu não compilo: o build é do usuário (`make -j20`, no WSL).
Resultado: **compilou sem erros** (depois de corrigir o id do menu, ver abaixo) e o **`make check` passou sem falhas**.
O que **não** foi feito: teste no emulador (o usuário não podia testar no momento do commit).

- [x] **Flags** (`include/constants/flags.h`): `FLAG_RECEIVED_EON_FLUTE` em `0xFF` (substituiu
      `FLAG_LATIOS_OR_LATIAS_ROAMING`, entre as outras `FLAG_RECEIVED_*`) e
      `FLAG_HIDE_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_REPORTER` em `0x264` (antigo `FLAG_UNUSED_0x264`).
      O usuário trocou os slots depois da minha primeira versão (que tinha a repórter em `0xFF` e a flauta em `0x109`);
      os nomes não mudaram, então o código não foi afetado. O `0x109` voltou a ser `FLAG_UNUSED_0x109`.
- [x] **FRLG**: **não** se mexe em `flags_frlg.h` (decisão do usuário). Eu tinha editado e **revertido**; o arquivo está
      igual ao commit.
- [x] **Nível 60**: `src/roamer.c` (Latias e Latios) e `SouthernIsland_Interior/scripts.inc` (os dois `seteventmon`).
- [x] **Portão da ilha**: o encontro agora exige `checkitem ITEM_EON_FLUTE` em vez de `FLAG_ENABLE_SHIP_SOUTHERN_ISLAND`.
- [x] **Soar** (`src/soar.c`): A pousa na Southern Island (`mapSecId == MAPSEC_SOUTHERN_ISLAND`) e o popup esconde a
      Battle Frontier até `FLAG_LANDMARK_BATTLE_FRONTIER`.
- [x] **Casa pós-Liga** (`data/scripts/players_house.inc`): cortada a TV, a escolha de cor e o `InitRoamer`. Ficam o pai,
      o SS Ticket, a fala `DadShouldStayLonger` e os `setvar ... 4`. A cena foi renomeada para
      `PlayersHouse_1F_EventScript_GetSSTicket` (3 referências: a cena e os dois `map_script_2`).
      `tv.inc` ficou só com o texto do noticiário (sem `InitRoamer` e sem a flag antiga).
      **Código órfão mantido de propósito** (inofensivo): os scripts `MomNoticesLatiBroadcast*`,
      `PlayerApproachTVForLati*`, `MomApproachPlayerAfterTV*`, seus movimentos, e os textos `IsThatABreakingStory`,
      `WhatColorDidTheySay` e `StillUnknownPokemon`.
- [x] **Menu** `MULTI_EON_FLUTE_LATI` (LATIAS / LATIOS / CANCEL) em `include/constants/script_menu.h` e `src/data/script_menu.h`.
      Id **160**, definido ao lado de `MULTI_REGION` (159, fixo). Dentro do enum ele caía no 159 e o build falhava com
      `initialized field overwritten`.
- [x] **Telhado**: scripts, movimentos e textos em `LilycoveCity_DepartmentStoreRooftop/scripts.inc`, e os três NPCs
      ligados no `map.json` (scripts e a `FLAG_HIDE_..._REPORTER` na repórter e no cameraman).
- [x] **Build** (`make -j20`): compila. **`make check`**: sem falhas.
- [x] **Commits**: feitos (ver `git log`; quatro commits: flags e cena da casa, níveis/ilha/soar, telhado e menu, este plano).
- [ ] **Teste no emulador** (usuário), quando puder. Roteiro de teste na seção 8. É o único passo que falta.

---

## 1. O que já está implementado

Commit `a0cfb514c3` (`feat: add Eon Flute soaring, ported from Artrios's pokeemerald`), baseado em
https://github.com/Artrios/pokeemerald/tree/soar (remote `artrios` já adicionado).

- `src/soar.c` / `include/soar.h`: tela de voo em mode 7 sobre o mapa de Hoenn.
- `ITEM_EON_FLUTE = 879` (item-chave, `ITEM_USE_FIELD`), ícone e descrição.
- `FLAG_EON_LATI` (`SYSTEM_FLAGS + 0x21`, antigo `FLAG_UNUSED_0x881`): **setada = Latias, limpa = Latios**.
  Sem nada setado, aparece o Latios.
- `WarpAndFlyIntoMap` em `src/field_effect.c` e `RegionMap_InitPlayerLocation` / `RegionMap_SetCursorPos`
  em `src/region_map.c`.
- Pouso reaproveita `SetFlyDestination`: **o soar pousa exatamente onde o Fly pousa**. Rotas não são
  pousáveis (decisão do usuário; as 35 flags `FLAG_VISITED_ROUTE*` do original foram descartadas de propósito).
- `CREDITS.md`: seção "Eon Flute Soaring Credits" (Artrios + sprites de Eduardo Quezada).

Controles atuais: ← → vira, ↑ ↓ altitude, L (segurar) dobra a velocidade, R barrel roll, A pousa (cidade
liberada), B/START sai. **O voo sempre avança sozinho** (herdado do original; igual ao ORAS).

**Testado no emulador (2026-10-14):** o soar e a flauta registrada no SELECT funcionam bem. Controles aprovados
como estão (inclusive o avanço contínuo). Ainda não testados: pouso na Southern Island e o evento do telhado,
porque nada disso foi implementado.

---

## 2. Decisões fechadas

| Tema | Decisão |
|---|---|
| Pouso | Só onde o Fly pousa. Fly (HM) **não** pousa na Southern Island, só o soar. |
| Southern Island | Acessível **somente pelo soar**. O Eon Ticket fica sem uso (nunca é dado). |
| Lati@s da ilha | O **oposto** do que vaga (já é assim no código). **Nível 60** (hoje 50). |
| Roamer | **Nível 60** (hoje 40). O level cap nessa parte do jogo é 61. |
| Quem inicia o roamer | A **repórter**, no telhado da Lilycove Department Store, fazendo uma reportagem **junto com o enthusiast** (cena **opcional**, ao falar com ela). |
| Quem entrega a flauta | O **"lat enthusiast"**. Ele já tem uma flauta **"quebrada"** que só reage quando há um Lati@s por perto; ao ver isso, entrega a flauta ao jogador que mostra um Lati@s da party. |
| `FLAG_EON_LATI` | Setada para o Lati@s **mostrado** ao enthusiast. |
| Dois Lati@s na party | O enthusiast deixa o jogador **escolher qual a flauta invoca** (alterna a flag). Vale depois também. |
| Roamer nocauteado | Perdido para sempre, como no vanilla. Manter, **sem nenhum aviso** em falas ou descrições. |
| Alvo de pouso da ilha | 1x1 no mapa do soar. **Manter assim por enquanto** (confirmado); rever depois de implementar o pouso. |
| Casa pós-Liga | Tirar a parte da TV (e a da mãe), mas **continuar passando o estado** (3 → 4). SS Ticket do pai fica. |
| Gatilho por progressão | Nenhum. O jogador pode fazer a cena do repórter assim que chegar ao telhado. |
| Enthusiast antes da cena | Só diz "Shh! I'm being interviewed!". Quem dispara a cena é apenas a repórter. |

---

## 3. Fluxo desejado, de ponta a ponta

1. **Hall da Fama** → pai entrega o SS Ticket em casa (como hoje), **sem** TV nem escolha de cor. Estado das casas vai a 4.
2. **Lilycove, telhado da loja** → a **repórter** faz uma reportagem com o **enthusiast** como entrevistado.
   Jogador fala com ela: transmissão com "BZZT", o microfone chia, ela pergunta a cor (Red/Blue), o roamer
   (Latias/Latios, nível 60) começa a vagar e a repórter sai de cena junto com o cameraman. O enthusiast fica.
3. O **enthusiast** passa a ser o NPC dos Lati@s:
   - sem Lati@s na party: fala pouco ("jatos neon") e comenta que tem uma flauta estranha que parece quebrada;
   - com um Lati@s: a flauta **reage**, o jogador mostra qual Pokémon é, ele entrega a **EON FLUTE**,
     `FLAG_EON_LATI` é setada e ele só insinua a ilha;
   - com os dois: troca qual Lati@s a flauta invoca.
4. **Soar** → pousa na **Southern Island** (alvo 1x1) → entra no interior → encontro com o Lati@s oposto (nível 60, Soul Dew).
5. Volta: marinheiro da praia (leva a Lilycove sem ticket), Fly (HM) ou a própria flauta. Sem soft lock.

---

## 4. Pontos de código a mexer

Os números de linha são do momento em que foram verificados e podem ter deslocado.

### Níveis
- `src/roamer.c:159` e `:161`: `TryAddRoamer(SPECIES_LATIAS/LATIOS, 40)` → 60.
- `data/maps/SouthernIsland_Interior/scripts.inc:105` e `:109`: `seteventmon ..., 50, ITEM_SOUL_DEW` → 60.

### Southern Island só pelo soar
- `src/soar.c`, `HandleSoarInput`: hoje só aceita A em `MAPSECTYPE_CITY_CANFLY` ou `MAPSECTYPE_BATTLE_FRONTIER`.
  Adicionar `MAPSEC_SOUTHERN_ISLAND`. **Não mexer em `GetMapsecType`** (senão o Fly também passa a pousar lá).
- `FilterFlyDestination` (`src/region_map.c`) **já** tem o caso `MAPSEC_SOUTHERN_ISLAND` →
  `HEAL_LOCATION_SOUTHERN_ISLAND_EXTERIOR` (código do vanilla nunca usado).
- `SouthernIsland_Interior/scripts.inc:56`: o encontro exige `FLAG_ENABLE_SHIP_SOUTHERN_ISLAND`, que só o
  ticket seta. Trocar por checar `ITEM_EON_FLUTE` na bolsa (`checkitem`), assim funciona mesmo com a flauta
  vinda do debug. O ferry de Lilycove também exige o ticket na bolsa, então essa flag sozinha não abria nada.

### Casa pós-Liga (retirar TV e mãe, manter o estado)
- `data/scripts/players_house.inc:416-478` (`PlayersHouse_1F_EventScript_GetSSTicketAndSeeLatiTV`): cortar da TV
  em diante e ir direto aos `setvar VAR_LITTLEROOT_HOUSES_STATE_MAY/BRENDAN, 4` (linhas ~476-477).
  Decidido: a fala `DadShouldStayLonger` (mãe) **fica**. Ela é a última antes da TV (`players_house.inc:451-452`),
  então o corte é logo depois do `closemessage` dela: remover de `setflag FLAG_SYS_TV_LATIAS_LATIOS` (~453) até
  o `msgbox ..._StillUnknownPokemon` / `closemessage` (~474-475), e manter os `setvar ... 4`, `releaseall` e `end`.
  O pai sair e `setflag FLAG_RECEIVED_SS_TICKET` (~446) ficam antes do corte e não mudam.
- **Dependem do estado 4**: `PlayersHouse_1F_EventScript_Mom` (~linha 302, "DontPushYourselfTooHard") e
  `LittlerootTown/scripts.inc:43-44` (converte 4 em 5). Não quebrar.
- `data/scripts/tv.inc:37-44` (`EventScript_PlayersHouseLatiNewsFlash`): segunda entrada para o mesmo
  noticiário, com um bug conhecido (`special InitRoamer` com `VAR_0x8004` sem valor). Neutralizar.
- `FLAG_LATIOS_OR_LATIAS_ROAMING`: só `players_house.inc` e `tv.inc` a usam. Pode sair.
- **Cuidado com `FLAG_SYS_TV_LATIAS_LATIOS`**: `src/tv.c:3161` (`CheckForPlayersHouseNews`) a **lê**. Se a
  flag nunca for setada, a função cai nos outros ramos (filme / fallback), sem problema, mas **não apagar a
  constante** sem mexer no `tv.c`.

### Telhado da Lilycove Department Store
- Mapa `LilycoveCity_DepartmentStoreRooftop`: 18x12, `MAP_TYPE_INDOOR`, sem eventos de coordenada. Os 4 NPCs
  originais ficam em (4,4), (7,5), (6,1) e (15,5), todos com `elevation: 3`.
- **NPCs criados no Porymap** (2026-10-14, commit `35c11647b2`). Scripts e flags já ligados na implementação
  (cameraman e repórter com `FLAG_HIDE_..._REPORTER`; enthusiast sem flag):

  | ID | Papel | Sprite | Posição | Olha para |
  |---|---|---|---|---|
  | 5 | Enthusiast | `OBJ_EVENT_GFX_MAN` | (11, 6) | cima |
  | 6 | Cameraman | `OBJ_EVENT_GFX_CAMERAMAN` | (12, 5) | baixo |
  | 7 | Repórter | `OBJ_EVENT_GFX_REPORTER_F` | (11, 5) | baixo |

  A repórter (11,5) e o enthusiast (11,6) ficam um de frente para o outro (a entrevista). O cameraman fica ao lado da repórter.
- **O cameraman é um NPC novo no plano.** Ele sai junto com a repórter (o "Pack it up!" é dito a ele), então deve usar a
  **mesma** `FLAG_HIDE_..._REPORTER`. Não precisa de flag própria.
- Os três estão com `elevation: 3`, como os originais (o usuário ajustou do 0 para o 3).
- **`local_id` nomeados** (feito em 2026-10-14, no `map.json`; os números 5, 6 e 7 continuam os mesmos):
  - `LOCALID_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_ENTHUSIAST` (5)
  - `LOCALID_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_CAMERAMAN` (6)
  - `LOCALID_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_REPORTER` (7)

  Usar esses nomes nos `applymovement` (saída da repórter e do cameraman). O `map_event_ids.h` é gerado no build e está no
  `.gitignore`, então as constantes só aparecem depois do próximo `make`.
- Textos: este mapa não tem `text.inc` próprio; as falas ficam em `data/maps/LilycoveCity_DepartmentStoreRooftop/scripts.inc`.
- Estado persistente: ver a subseção de flags logo abaixo (2 flags, 0 vars).

### Flags e vars: o que criar e o que liberar (decidido em 2026-10-14)

**Criar: 2 flags, 0 vars.** O resto do estado do evento é derivado de coisas que já existem:

| Estado | De onde vem | Novo? |
|---|---|---|
| Cena da repórter feita (e repórter escondida) | `FLAG_HIDE_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_REPORTER` | **sim, flag 1** |
| Flauta já entregue | `FLAG_RECEIVED_EON_FLUTE` (flag, e não `checkitem`, para facilitar testar o script) | **sim, flag 2** |
| Qual Lati@s vaga | `VAR_ROAMER_POKEMON` (0 = Latias, 1 = Latios) | não |
| Qual Lati@s a flauta chama | `FLAG_EON_LATI` | já criada (slot do antigo `FLAG_UNUSED_0x881`) |
| Tem um Lati@s / os dois na party | `checkspecies` | não |
| Ilha já visitada | `FLAG_LANDMARK_SOUTHERN_ISLAND` | não |
| Coreografia da cena (repórter sai andando etc.) | `FLAG_TEMP_*` / `VAR_TEMP_*` do mapa (não são salvos) | não |

**Slots (decidido, ajustado pelo usuário em 2026-10-14):**
- `FLAG_RECEIVED_EON_FLUTE`: **`0xFF`**. **Substitui** `FLAG_LATIOS_OR_LATIAS_ROAMING`, no mesmo slot: uma sai, a outra
  entra, e o número de flags em uso não muda por causa dela. Fica junto das outras `FLAG_RECEIVED_*`
  (`FLAG_RECEIVED_CHARCOAL` 0xFE, `FLAG_RECEIVED_REPEAT_BALL` 0x100).
- `FLAG_HIDE_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_REPORTER`: **`0x264`** (antes `FLAG_UNUSED_0x264`, na faixa de
  flags livres). Serve de `flag` do objeto da repórter e do cameraman no `map.json` (como `FLAG_HIDE_..._SALE_WOMAN`)
  e é o que o enthusiast lê para saber que a reportagem já aconteceu. O enthusiast não precisa de flag própria
  (está sempre visível).
- O `0x109` **não** foi usado e continua `FLAG_UNUSED_0x109`.
- Ela é setada junto com o `giveitem ITEM_EON_FLUTE`. Para testar: setar pelo debug pula a entrega e cai no diálogo
  de "depois de entregar"; limpar permite repetir a entrega.
- O portão do encontro da ilha continua checando **o item** (`checkitem ITEM_EON_FLUTE`), porque sem o item
  o jogador nem consegue usar o soar.

**Liberar:**
| Flag | Valor | Situação |
|---|---|---|
| `FLAG_LATIOS_OR_LATIAS_ROAMING` | `0xFF` | **Sai (substituída por `FLAG_RECEIVED_EON_FLUTE`).** Só `players_house.inc` e `tv.inc` a usavam. |
| `FLAG_SYS_TV_LATIAS_LATIOS` | `SYSTEM_FLAGS + 0x5D` | **Fica por enquanto.** Nada mais a seta, mas `src/tv.c` ainda a lê (~3161), além do `case PLAYERS_HOUSE_TV_LATI` (~760) e do fallback (~3167). Liberar exige mexer em `tv.c`, `tv.inc` e `constants/tv.h`. Deixar a constante parada é inofensivo. |

**Não dá para liberar** (continuam usadas): `FLAG_ENABLE_SHIP_SOUTHERN_ISLAND`, `FLAG_SHOWN_EON_TICKET` e
`VAR_DISTRIBUTE_EON_TICKET` (o código do ticket, Mystery Gift, Record Mixing e o ferry de Lilycove continua no
jogo, só que o ticket nunca é dado), e `FLAG_ENCOUNTERED/CAUGHT/DEFEATED_LATIAS_OR_LATIOS` (a ilha usa).
`VAR_ROAMER_POKEMON` também fica.

**Espelho em `flags_frlg.h`: não é necessário (decisão do usuário).** `flags.h` usa `flags_frlg.h` só nos builds
`make firered` / `make leafgreen`, e esses builds não importam para o romhack. O arquivo não foi alterado.
(Efeito colateral aceito: `FLAG_EON_LATI` não existe no build FRLG, e o espelho da flag antiga
`FLAG_LATIOS_OR_LATIAS_ROAMING 0` continua lá.)

**Saldo:** 2 flags novas, 1 saindo, 0 vars. A conta de slots livres que a feature consome fica em 2 (`0x881`, já
consumido, e `0x264`), porque `FLAG_RECEIVED_EON_FLUTE` só reaproveita o `0xFF`. Flags são um bitfield de tamanho fixo no
save, então nada disso altera o layout. Folga: ~243 slots `FLAG_UNUSED_0x0xx`-`0x4xx` e ~23 `VAR_UNUSED`.

### Blocos de script que já existem (verificados)
- Escolha de cor: `multichoice 22, 8, MULTI_TV_LATI, TRUE` (rótulos `RED` / `BLUE`, em `src/data/script_menu.h`).
- Iniciar o roamer (padrão da cena original, `players_house.inc:470-473`):
  ```
  multichoice 22, 8, MULTI_TV_LATI, TRUE
  copyvar VAR_0x8004, VAR_RESULT
  special InitRoamer          @ 0 = Red = Latias vaga; senão Latios
  copyvar VAR_ROAMER_POKEMON, VAR_RESULT   @ 0 = Latias, 1 = Latios (o que VAGA)
  ```
- Checar a party: `checkspecies SPECIES_X` (varre a party, sem tela) e `checkspecies_choose SPECIES_X`
  (abre a tela de party). Ver `asm/macros/event.inc:2630-2644` e `src/scrcmd.c:3144-3170`.
  **Ler o `scrcmd.c` antes de usar**, para saber exatamente o que `Scrcmd_checkspecies_choose` grava em `VAR_RESULT`.

### Coisas que ainda não existem
- Um menu `LATIAS / LATIOS / CANCEL` para a troca de montaria (novo `MULTI_*` em
  `include/constants/script_menu.h` e `src/data/script_menu.h`), a menos que já haja um aproveitável.
- ~~Um `special` de "roamer ativo"~~: **descartado** (não haverá fala de roamer perdido).

---

## 5. Fatos verificados sobre o Emerald (para não reinvestigar)

- **A cena da TV hoje** (`players_house.inc`): é disparada ao entrar em casa depois do Hall da Fama
  (`hall_of_fame.inc:43-45` coloca o estado das casas em 3). Faz três coisas: pai entrega o SS Ticket, TV
  mostra o noticiário, mãe pergunta a cor e inicia o roamer.
- **Roamer**: nasce no nível 40 (`roamer.c:158-161`). `InitRoamer` usa `gSpecialVar_0x8004` (0 = Red = Latias,
  1 = Blue = Latios). A escolha fica em `VAR_ROAMER_POKEMON` (0 = Latias, 1 = Latios).
- **`ROAMER_COUNT` é 1** (`include/constants/global.h`): só um roamer ativo por vez, e `TryAddRoamer` falha em
  silêncio se já houver um.
- **Captura do roamer não deixa nenhuma flag.** `FLAG_CAUGHT_LATIAS_OR_LATIOS` é da **ilha** (não reusar).
- **O que remove o roamer** (`battle_main.c:5462-5476`, com `BUGFIX` ativo em `include/config/general.h:43`):
  vitória (nocaute), captura e empate. **Não removem**: blackout, fuga do jogador, fuga do roamer, teleport.
  O roamer **guarda o HP e o status** entre encontros e não se cura (`roamer.c:279-288`).
  O risco real é o nocaute acidental (veneno, clima, golpe forte demais).
- **Eon Ticket**: só chega por Mystery Gift (`cable_club.inc:33`, protegido por `VAR_DISTRIBUTE_EON_TICKET`,
  que é "always FALSE" em `field_specials.c:3677`) ou Record Mixing. Num jogo solo, a ilha não abre.
- **Southern Island** (`SouthernIsland_Interior/scripts.inc`): `VAR_ROAMER_POKEMON == 0` (Latias vaga) → a ilha
  tem o **Latios**, e vice-versa. Capturar seta `FLAG_CAUGHT_LATIAS_OR_LATIOS`; vencer ou fugir seta
  `FLAG_DEFEATED_...`, que o Hall da Fama limpa.
- **Saída da ilha**: marinheiro da praia (`SouthernIsland_Exterior/scripts.inc:9-25`), Fly e a flauta
  (os dois mapas são `MAP_TYPE_ROUTE`).
- **Níveis dos treinadores no repo** (E4 46-55, Wallace 55-58, Steven 75-78) estão defasados: o usuário está
  reajustando e o cap nesse trecho é 61. **Não** sugerir segurar a cena do repórter por progressão.

---

## 6. Falas (v2, aprovada pelo usuário em 2026-10-14; em inglês, que é a língua do jogo)

Linha em branco = nova caixa (`\p`). ♻ = reaproveitado da TV original. Linhas com até ~38 caracteres.
Nomes de lugar e espécie em maiúsculas, como no vanilla.

Direção da v2 (do usuário):
- A repórter faz a reportagem **junto com o enthusiast** (ele é o entrevistado).
- O microfone chia, ela pergunta ao jogador se ouviu e reage com "se você ouviu, então deve estar saindo na transmissão".
- O enthusiast é **pouco expositivo**: fala pouco e deixa pistas. Mas fica **animado e emocionado** quando vê o Lati@s.
- A flauta dele **já existe, mas "quebrada"**: só reage quando há um Lati@s por perto. É por isso que ele a entrega.

Piada central: no vanilla a TV diz um Pokémon de cor "BZZT" por falha de áudio e a mãe pergunta a cor.
Aqui é o **microfone da repórter** que chia, e o jogador faz o papel da audiência.

### 6.1 Reportagem (cena opcional, ao falar com a repórter; o enthusiast está ao lado dela)

Transmissão (♻):
```
We bring you this emergency
news flash!

In various HOENN locales, there have
been reports of a BZZT…colored
POKéMON in flight.

The identity of this POKéMON is
currently unknown.
```

O microfone chia:
```
Hm? Cut, cut! Did the mic crackle
again?!

You there, TRAINER! Did you hear
what I just said?

What color was that POKéMON?
```
Escolha: **RED / BLUE** (♻).

Reação (`{STR_VAR_1}` = cor escolhida; ideia do usuário: "se foi ouvido, deve dar para ouvir na reportagem"):
```
{STR_VAR_1}, you say? So you heard it
loud and clear!

Then it must be coming through fine
on the broadcast, too. Phew!
```

Fecho (♻ a segunda parte vem da fala da mãe e o "regular program" vem da TV):
```
Well, isn't that something!
There are still unknown POKéMON.

Thank you for your time, sir!
We now return you to the regular
program!
```
Aqui o roamer começa a vagar e a repórter sai de cena. O enthusiast fica.

### 6.2 Enthusiast

Antes da cena da repórter (ele está sendo entrevistado):
```
Shh! I'm being interviewed!
```
(decidido: é só esta fala. Quem dispara a cena é a repórter. O enthusiast só passa ao diálogo normal depois que a cena acontece.)

Depois da cena, sem Lati@s na party (**texto do usuário**; correções de gramática marcadas abaixo):
```
Did you hear the report? A glowing
POKéMON streaking across HOENN like
a neon jet!

Those are the EON POKéMON,
LATIAS and LATIOS!

They move from place to place
in the blink of an eye!

I would love to see them up close,
at least once.

I even got a weird flute that's said
to be able to call them, but it
seems broken...
```
Correções: "ina" → "in a"; "that were said" → "that's said"; "in a blink" → "in the blink" (idiom).
Se preferir o original, é só voltar.

Com um Lati@s na party (a flauta reage e abre a tela para mostrar):
```
Hm? What's that sound?
The flute... it's glowing!

It's reacting to something! One of
your POKéMON, maybe? Show me!
```
- Cancelou: `Oh... Maybe next time, then.`
- Mostrou outro Pokémon:
```
Hm... The flute doesn't react to that
one. It must be another.
```
- Mostrou LATIAS/LATIOS (`{STR_VAR_1}` = espécie):
```
...!

The flute is singing! It's a
{STR_VAR_1}!

It was never broken. It just needed
to be close to one of them!

I can't believe I'm this close to a
{STR_VAR_1}! My hands are shaking!

You must be a truly skilled TRAINER
to have earned such a magnificent
POKéMON's trust!

Take it, {PLAYER}. It belongs with
someone who can actually fly with it.
```
(Versão animada, a partir do rascunho do usuário. Ajustes: "pokemon" → POKéMON, a frase longa foi quebrada
em linhas de até ~35 caracteres, e "bestow its magnificency" virou "earned such a magnificent POKéMON's trust".)
  *(Obtained the EON FLUTE!)*
```
I heard there's an island far to the
south that no ferry reaches.

Maybe the flute knows the way.
```
(Só uma pista, sem explicar como usar. O "como usar" fica por conta da descrição do item.)

Depois de entregar, conversa normal:
- Ilha ainda não visitada:
```
How's the flute treating you?

I still wonder about that island far
to the south...
```
- Ilha já visitada (`FLAG_LANDMARK_SOUTHERN_ISLAND`):
```
You've been to the island, haven't
you? I can see it in your eyes.

Nothing beats a story like that!
```

Com os dois na party (troca de montaria):
```
Both a LATIAS and a LATIOS?!
The flute is going crazy!

It can only call one at a time.
Which one should answer?
```
Escolha: **LATIAS / LATIOS / CANCEL**.
- Confirmação: `Understood! From now on, the flute will call {STR_VAR_1}.`
- Cancelar: `Take your time. Decide when you're ready.`

Roamer perdido: **sem fala própria** (decidido). O enthusiast não reage ao nocaute; segue no diálogo de "sem Lati@s na party".

### 6.2b Cameraman (antes da cena; sai junto com a repórter depois)
```
Don't look at me. I only point
the camera.
```

### 6.3 Casa pós-Liga
`IsThatABreakingStory`, `WhatColorDidTheySay` e `StillUnknownPokemon` (mãe) deixam de ser usadas.
`DadShouldStayLonger` **fica**, como fecho da cena do pai (decidido). Ela não depende de nenhum evento da TV.

---

## 7. Pendências e decisões abertas

- [x] NPCs no Porymap: criados (ids 5 enthusiast, 6 cameraman, 7 repórter; commit `35c11647b2`). Scripts ligados na implementação.
- [x] `local_id` nomeados nos três NPCs (ver seção 4).
- [x] Ligar a `FLAG_HIDE_..._REPORTER` (campo `flag` do `map.json`) à repórter e ao cameraman: feito.
- [x] Fala do cameraman antes da cena: "Don't look at me. I only point the camera." (aprovada).
- [x] O cameraman **não fala** na cena (decidido). Só a fala de antes da cena.
- [x] Revisão das falas v2: aprovada, cobre os cenários desejados.
- [x] Falar com o enthusiast **antes** da cena: decidido, ele só diz "Shh! I'm being interviewed!". Só a repórter dispara a cena.
- [x] O aviso "go easy on it, if it faints the chase is over": decidido, **não entra** em lugar nenhum.
      O jogador descobre sozinho que o nocaute perde o roamer.
- [x] A flauta "quebrada" é só narrativa, confirmado. O enthusiast acha que ela está quebrada porque nunca conseguiu
      chamar o Pokémon; com o Lati@s do jogador por perto ela reage. Mecanicamente ela é o `ITEM_EON_FLUTE` normal.
- [x] A mãe mantém `DadShouldStayLonger`: decidido, **mantém**.
- [x] Nomear LATIAS e LATIOS logo de cara: resolvido na v2 (o enthusiast os nomeia, mas pouco explica).
- [x] Fala de "roamer perdido": decidido, **não precisa**. Sem o `special` de roamer ativo.
- [x] Menu `LATIAS / LATIOS / CANCEL` para a troca: feito (`MULTI_EON_FLUTE_LATI`).
- [x] Flags/vars: decidido (ver seção 4): 2 flags novas (`FLAG_RECEIVED_EON_FLUTE` em `0xFF`, no lugar de
      `FLAG_LATIOS_OR_LATIAS_ROAMING`, e `FLAG_HIDE_..._REPORTER` em `0x264`), 0 vars.
- [x] Slots das duas flags: `0xFF` (flauta) e `0x264` (repórter). O usuário trocou em relação à minha primeira versão.
- [x] `FLAG_EON_LATI 0` em `flags_frlg.h`: **descartado**, o FRLG não precisa ser alterado (decisão do usuário).
- [x] Controles do soar: aprovados como estão (avanço contínuo incluído).
- [x] Testar o soar no emulador e a flauta registrada no SELECT: funcionam bem.
- [x] Alvo de pouso da ilha 1x1: **manter 1x1 por enquanto**. Rever só depois de implementar o pouso na ilha,
      se na prática for difícil de acertar.
- [x] Nome no popup do soar antes da primeira visita: decidido. **Southern Island: sempre visível** (ajuda a achar
      o alvo 1x1). **Battle Frontier: escondido até `FLAG_LANDMARK_BATTLE_FRONTIER`**, como no mapa de Fly.
      Contexto: o mapa de Fly (`DrawFlyDestTextWindow`) só desenha o nome se `mapSecType != MAPSECTYPE_NONE`, e
      `GetMapsecType` devolve NONE para esses dois locais até as flags `FLAG_LANDMARK_*`; cidades não visitadas
      **mostram** o nome. O popup do soar (`UpdateMapSecPopup`) não checa o tipo, então hoje mostra a Battle Frontier
      antes da hora.
- [x] Fix do popup da Battle Frontier em `src/soar.c`: feito (condição pelo mapsec, a Southern Island continua visível).
- [x] Push do commit do soar: feito (a branch local acompanha `origin/feature/eon-flute`, ambos em `a0cfb514c3`).
      O commit `35c11647b2` (NPCs do telhado, feito pelo usuário) está só local. A implementação e este plano também.

---

## 8. Como testar

Build é do usuário, no WSL: `make -j20`. O `map_event_ids.h` (os `LOCALID_*` novos) é gerado nesse build.

### 8.1 Soar (já testado, repetir só o que mudou)
1. Pegar a **EON FLUTE** pelo menu de debug. Para a Latias, setar a flag `FLAG_EON_LATI` (`0x881`); sem ela aparece o Latios.
2. A sobre a **Southern Island** (alvo 1x1, ao sul) abre "Would you like to land here?" e "sim" pousa na praia da ilha.
3. Sobrevoar a **Battle Frontier** sem `FLAG_LANDMARK_BATTLE_FRONTIER`: o popup **não** deve mostrar o nome. Com a flag, mostra e deixa pousar.
4. Na ilha: entrar no interior e conferir o encontro (**nível 60**, Soul Dew, Lati@s **oposto** ao que vaga). Sem a flauta na bolsa, só a placa.
5. Voltar da ilha: marinheiro da praia (leva a Lilycove), Fly (HM) ou a própria flauta.

### 8.2 Evento do telhado (Lilycove Department Store, `MAP_LILYCOVE_CITY_DEPARTMENT_STORE_ROOFTOP`)
1. **Antes da cena**: falar com o enthusiast → "Shh! I'm being interviewed!". Falar com o cameraman → "Don't look at me...".
2. **Cena**: falar com a repórter. Ela só é alcançável **pelo norte**, de (11,4). Conferir: transmissão virada para a câmera,
   depois vira para o jogador, pergunta a cor (RED/BLUE, sem cancelar), reação com a cor escolhida, fecho.
3. **Saída**: o cameraman sobe, vai à direita e sai pela escada (13,3); a repórter espera um passo e vai pelo mesmo caminho
   sem atravessar o jogador nem o cameraman. Os dois somem e **não voltam** ao reentrar no mapa.
4. **Roamer**: conferir que ele existe e está nível 60 (RED = Latias vaga, BLUE = Latios vaga).
5. **Enthusiast sem Lati@s na party**: texto da reportagem e da flauta "quebrada".
6. **Enthusiast com um Lati@s** (dar um pelo debug): "a flauta está brilhando", abre a tela de party.
   - cancelar → "Maybe next time"; escolher outro Pokémon → "doesn't react to that one";
   - escolher o Lati@s → texto animado, **recebe a EON FLUTE**, `FLAG_RECEIVED_EON_FLUTE` fica setada e `FLAG_EON_LATI`
     fica setada (Latias) ou limpa (Latios) conforme o mostrado, e ele deixa a pista da ilha.
7. **Depois da entrega**: conversa normal; antes da primeira visita à ilha uma fala, depois outra (`FLAG_LANDMARK_SOUTHERN_ISLAND`).
8. **Com os dois na party**: menu LATIAS / LATIOS / CANCEL; escolher alterna `FLAG_EON_LATI`; cancelar e B só dão "Take your time".
9. **Atalho de teste**: setar `FLAG_RECEIVED_EON_FLUTE` pelo debug pula a entrega; limpá-la permite repetir. Limpar
   `FLAG_HIDE_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_REPORTER` (`0x264`) refaz a cena (reentre no mapa para a repórter e o cameraman reaparecerem).

### 8.3 Casa pós-Liga
1. Depois do Hall da Fama, entrar em casa: o pai entrega o **SS Ticket**, a mãe fala do pai e **acaba**, sem TV nem pergunta de cor.
2. Conferir que o estado das casas passa para 4 e depois 5 (a mãe fala "don't push yourself too hard" e Littleroot segue normal).
3. A TV de casa não deve iniciar nenhum roamer.

## 9. Convenções do projeto (não esquecer)

- O build e os testes são do usuário; eu entrego o comando pronto. `make` sempre com `-j20`.
- SSH pede passphrase de propósito; falha de auth no push/pull não é para consertar.
- Não há saves de jogadores ainda: layout do SaveBlock pode crescer sem preocupação.
- Arquivos soltos no repo (`pokemon_stats.csv`, `trainer_classes.csv`, `trainer_names_classes.csv`,
  `trainers.txt`) não são desta feature; ficam fora dos commits.
