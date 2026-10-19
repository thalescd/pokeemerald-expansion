# Plano: BUG CATCHING CONTEST

- **Branch:** `plan/bug-catching-contest`
- **Status:** planejado
- **Criado em:** 2026-10-17
- **Escopo (pode alterar):** sistema do concurso (novo `bug_catching_contest.c/.h`, scripts e textos do concurso), pontos
  de ligação em batalha, menu de início, passos do jogador, flags/vars e specials (ver Levantamento)
- **Fora de escopo:** a Safari Zone atual, mapas existentes, e o mapa/portão do concurso (criados pelo usuário)

Este plano só pode ser trabalhado na branch acima. As regras de trabalho (levantamento antes de executar, anotar o
progresso aqui, uma decisão por vez) estão no `CLAUDE.md`.

Valores de **Status**: `planejado`, `em andamento`, `concluído`, `abandonado`.

---

## Objetivo

Criar um Bug Catching Contest (Concurso de Captura de Insetos) logo abaixo de Verdanturf, usando como base as regras
dos jogos de GBC (Gold e Silver). O projeto descompilado do pokegold serve de referência: dele vamos tirar o
funcionamento do sistema, o que precisa ser portado para cá e o que o usuário quer modificar.

## Decisões

- 2026-10-17: base das regras = Pokémon Gold/Silver (GBC). Referência de código = projeto descompilado do pokegold
  (link ainda a ser passado pelo usuário).
- 2026-10-17: local = abaixo de Verdanturf. O mapa ainda não existe e será criado pelo usuário; por ora o trabalho é
  só o sistema do concurso.
- 2026-10-17: agenda = sempre aberto (sem dia da semana e sem limite diário), com taxa de entrada de 5000, como na
  Safari Zone.
- 2026-10-17: equipe = igual ao original (só o 1º Pokémon fica com o jogador; o resto fica com o oficial e é devolvido
  no fim).
- 2026-10-17: Pokémon do concurso = mesma lista do original por enquanto (Caterpie, Weedle, Metapod, Kakuna,
  Butterfree, Beedrill, Venonat, Paras, Scyther, Pinsir); o usuário edita depois.
- 2026-10-17: prêmios = equivalentes do original (1º Sun Stone, 2º Everstone, 3º equivalente da Gold Berry, os demais
  uma berry de consolação).
- 2026-10-17: (substitui a decisão anterior sobre a lista de Pokémon) encounters do concurso, na ordem dos 12 slots do
  Emerald (20/20/10/10/10/10/5/5/4/4/1/1): Wurmple 9-19, Grubbin 9-19, Surskit 9-19, Cutiefly 9-19, Sizzlipede 13-18,
  Skorupi 13-18, Scyther 14-16, Pinsir 14-16, Heracross 14-16, Durant 14-16, Heracross 14-16, Durant 14-16. Níveis
  podem mudar depois de calibrar a pontuação. Ideia: como no Gold, os mais fortes vêm em nível menor e com pouca chance,
  para os fracos em nível alto chegarem perto na pontuação.
- 2026-10-17: ajuste de níveis: Grubbin 9-17, Surskit 9-18, Cutiefly 9-18, Heracross 14-15 (nos dois slots; confirmar).
  Os demais ficam como na lista acima. Encounters considerados finais pelo usuário (2026-10-17).
- 2026-10-17: pontuação adaptada aos IVs do Emerald no lugar dos DVs (usuário escreveu "IC"; entendido como IV,
  confirmar).
- 2026-10-17: prêmios definidos: 3º lugar = Sitrus Berry; consolação = Oran Berry.
- 2026-10-17: os 10 rivais são treinadores de Hoenn já existentes (como o Gold reaproveita treinadores de Johto); são
  só NPCs de conversa no concurso, a classe e o nome vêm da tabela de treinadores. IV médio de referência = 15.
  Rivais e resultados (espécie + pontuação, +0 a +7 aleatório no sorteio), 1º/2º/3º resultado:
  1. Bug Catcher Davis: Pinsir 340, Grubbin 285, Wurmple 240
  2. Bug Catcher Lyle: Skorupi 290, Grubbin 270, Wurmple 255
  3. Bug Catcher Rick: Surskit 285, Wurmple 250, Wurmple 215
  4. Bug Catcher Greg: Surskit 300, Surskit 270, Wurmple 230
  5. Bug Maniac Brent: Heracross 360, Pinsir 340, Skorupi 305
  6. Bug Maniac Derek: Cutiefly 312, Surskit 288, Wurmple 262
  7. Bug Maniac Donald: Sizzlipede 318, Skorupi 312, Grubbin 285
  8. Camper Shane: Cutiefly 305, Surskit 280, Wurmple 245
  9. School Kid Georgia: Cutiefly 295, Wurmple 278, Surskit 260
  10. Cooltrainer Carolina: Scyther 357, Heracross 343, Durant 330 (15 a menos que a proposta inicial, pedido do
      usuário). Valores a recalibrar quando a fórmula de pontuação estiver fechada.

- 2026-10-17: IVs entram só pelos stats (sem termo extra na pontuação). Timer = passos, como na Safari Zone (menos que
  os 500 dela, fechado em 250 passos; substitui a ideia do contador de tempo de jogo). Park Ball: taxa normal da espécie x1,5 como no Gold (acrescentar `case BALL_PARK`).
  Troca do pokémon capturado: manter a pergunta; plano B é escolher quem sai da equipe depois da batalha.

- 2026-10-18: o usuário criou os dois mapas (dois mapas, como recomendado): `MAP_VERDANTURF_TOWN_MOTH_GARDEN_ENTRANCE`
  (portão: entrada, juiz, prêmios, rivais) e `MAP_MOTH_GARDEN` (a grama do concurso). Só os warps estão feitos; eventos,
  NPCs, encounters e scripts ainda não. Warps: Verdanturf (7,24) -> portão warp 2; portão (3,21) e (4,21) -> Moth Garden
  warp 0; Moth Garden (3,5) -> portão warp 0; portão (15,13) e (16,13) -> Verdanturf warp 7. Conferido em 2026-10-18:
  os warps são consistentes entre si.

- 2026-10-18: correção a pedido do usuário: o Moth Garden usava `MAPSEC_ABANDONED_SHIP` renomeado, o que mudava o
  nome da Abandoned Ship real. Nome da Abandoned Ship restaurado e criado `MAPSEC_MOTH_GARDEN` (sem x/y no mapa
  regional, como a Navel Rock) em `region_map_sections.json`; `MothGarden/map.json` aponta para ele. Sem tema de popup
  próprio por enquanto (usa o padrão do índice, como a Devon Corp); não testado, depende do build.

- 2026-10-19: batalha do concurso = opção A, fiel ao Gold (batalha normal com menu FIGHT / PKMN / PARK BALL / RUN),
  por causa de um erro do levantamento anterior (que dizia "sem luta"). Mantém EXP do Pokémon do jogador (sim) e o
  concurso termina, em vez de whiteout, se o Pokémon do jogador desmaia (sim).

## Levantamento

**Obrigatório antes de executar. Aprovado pelo usuário em:** 2026-10-19 (levantamento reescrito depois da correção
abaixo; o usuário aprovou a versão para a opção A)

**Correção (2026-10-19):** a primeira versão deste levantamento partia de uma premissa errada do Gold. A batalha do
concurso do Gold NÃO é do tipo Safari: é uma batalha selvagem normal (`BATTLETYPE_CONTEST`), menu FIGHT / PKMN /
PARKBALL×N / RUN (`pokegold/engine/battle/menu.asm`). O jogador luta com o 1º Pokémon para enfraquecer o inseto, joga
a Park Ball direto (sem abrir a mochila, `engine/battle/core.asm` ~4703), pode fugir sempre, e a batalha acaba quando as
bolas zeram (`CheckContestBattleOver`). Decisão do usuário: opção A, fiel ao Gold.

### Regras (Gold) e como ficam aqui

- Entrada: oficial pergunta, cobra 5000, deixa o resto da equipe com ele (só o 1º Pokémon, vivo e não ovo, fica),
  dá 20 Park Balls e inicia o contador (aqui 250 passos, não 20 min).
- Batalha: selvagem normal, 1 Pokémon do jogador, ganha EXP; menu com a Park Ball no lugar da mochila.
- Só 1 capturado (`BugContest_SetCaughtContestMon`): a 1ª captura fica guardada. Se já houver uma guardada, o jogo
  mostra "You already caught a X", compara (nome, nível, HP máx dos dois) e pergunta "Switch #MON?": Sim = o novo
  substitui o antigo, Não = o novo é solto. A bola é gasta de qualquer jeito. Aqui: a mesma pergunta, dentro da
  batalha, com `yesnobox` (mecanismo que a batalha já tem); texto simples com os dois lado a lado no lugar da tela de
  comparação. Plano B, se travar: perguntar depois da batalha, num script de campo.
- Fim: por passos, por bolas ou se o Pokémon do jogador desmaia (decisão do usuário: em vez de whiteout, vai para os
  resultados). Volta ao portão, o juiz anuncia 3º, 2º e 1º e o resultado do jogador, o oficial dá o prêmio; o
  Pokémon capturado vai para a equipe (ou caixa se cheia); a equipe volta.

### O que muda (arquivos, símbolos, flags e vars)

- Novo `src/bug_catching_contest.c` + `include/bug_catching_contest.h` (estado em EWRAM, nada salvo): contest ativo,
  Park Balls restantes, passos restantes, Pokémon capturado (`struct Pokemon`), backup da equipe, rivais sorteados;
  início e fim do concurso, passo (`BugContestTakeStep`), pontuação, tabela dos 10 rivais, sorteio dos 5,
  ranking com desempate aleatório, specials de anúncio (nomes/espécies/pontos nas strings) e de entrega.
  Pontuação: HP máx ×4 + Atk + Def + Spe + SpA + SpD + HP atual/8 + 1 se segura item (IVs só via stats).
- `src/battle_controller_player.c` (`HandleInputChooseAction`, caso do botão de cima à direita): no concurso, emitir
  `B_ACTION_THROW_BALL` no lugar de `B_ACTION_USE_ITEM`. Já existe o caminho de jogar bola direto (usado pelo "último
  item usado"), então não preciso abrir a mochila.
- `src/battle_util.c` (`HandleAction_ThrowBall`): no concurso, a bola é `ITEM_PARK_BALL` e desconta o contador do
  concurso em vez de `RemoveBagItem`.
- Texto do menu de ação ("BAG" -> "PARK BALL ×N") em `src/battle_controller_player.c` / `battle_message.c`.
- `src/battle_main.c`: fim da batalha por bolas zeradas (reaproveitar `B_OUTCOME_NO_SAFARI_BALLS` ou nova condição, na
  checagem de fim de turno), e Fugir sempre permitido.
- `src/battle_script_commands.c`: `Cmd_givecaughtmon` (e `Cmd_trygivecaughtmonnick` sem apelido) com ramo do concurso,
  que guarda o Pokémon no slot do concurso com a pergunta de trocar; `ComputeBallData` com `case BALL_PARK` x1,5 como
  no Gold. Dados em `data/battle_scripts_*.s` e strings novas em `include/constants/battle_string_ids.h` /
  `src/battle_message.c`.
- `src/battle_setup.c`: `CB2_EndWildBattle` (e o caminho de derrota) com ramo do concurso: bolas zeradas ou lead
  desmaiado -> script de fim; captura normal -> volta ao campo.
- Whiteout: `src/overworld.c` (`DoWhiteOut`, `CB2_WhiteOut`) e `src/field_poison.c` (envenenamento no campo) com ramo
  do concurso -> resultados, em vez de whiteout.
- `src/field_control_avatar.c`: um passo do concurso ao lado de `SafariZoneTakeStep`.
- `src/start_menu.c`: menu de início do concurso (Pokémon, Bolsa, Retire; sem Salvar) + janela com bolas e passos,
  como a da Safari (`BuildSafariZoneStartMenu`, `ShowSafariBallsWindow`).
- `src/data/wild_encounters.json`: entrada de `MAP_MOTH_GARDEN` com os 12 slots finais.
- `data/specials.inc` e novo `data/scripts/bug_catching_contest.inc`: entrada (taxa de 5000, deixar a equipe, 20 bolas),
  fim por tempo/bolas/desmaio, juiz, prêmios (Sun Stone, Everstone, Sitrus Berry, Oran Berry) e fala de cada rival;
  textos em inglês. Scripts do portão em `data/maps/VerdanturfTown_MothGarden_Entrance/scripts.inc` e guarda em
  `data/maps/MothGarden/scripts.inc`.
- `include/constants/flags.h`: 10 flags de "rival escondido" (uma por rival; só os 5 sorteados aparecem no portão
  depois do concurso). Sem flag de modo: o estado do concurso fica em EWRAM e some no reset.

### Reaproveitamento do Pokémon Contest do Emerald (`src/contest.c`, `src/contest_util.c`)

Só como padrão, nada é chamável (tudo `static` ou preso a `gContestOpponents`/4 competidores):
- Sorteio sem repetição (`SetContestants`, contest.c ~2962): para sortear 5 dos 10 rivais.
- Desempate aleatório (`DetermineFinalStandings`, contest.c ~3648): sorteia uma ordem por competidor antes de ordenar.
- Specials que põem nomes nas strings (`BufferContestantTrainerName`, contest_util.c ~1953): modelo para o juiz.
- Não reaproveitar: rodadas/apelos, IA, tela de resultados e `struct ContestPokemon`.

### Mapas e NPCs

Os dois mapas já existem (ver Decisões de 2026-10-18). NPCs que o usuário coloca no Porymap, ligando aos scripts que eu
escrevo: no portão, 1 oficial + 10 rivais (todos escondidos pelas flags novas; só os sorteados aparecem); no
Moth Garden, nenhum obrigatório. Não vou criar objetos nos mapas (posição e sprite são escolhas dele). Os 2 warps
do portão para o Moth Garden hoje levam direto para a grama: o ideal é trocá-los por gatilhos (coord event) que chamam
o script de entrada, e eu ponho uma guarda no Moth Garden (se o concurso não estiver ativo, volta ao portão).

### O que é compartilhado e pode quebrar

- Menu de ação, lançamento de bola, fim de batalha, whiteout e passos valem para TODAS as batalhas e para o jogo
  todo; cada mudança fica atrás de "concurso ativo" (EWRAM). Fora do concurso o comportamento precisa continuar
  idêntico. Risco maior: `HandleAction_ThrowBall` e `Cmd_givecaughtmon` (captura normal) e `DoWhiteOut`.
- A equipe fica mascarada durante o concurso (backup em EWRAM, restauro no fim). Sem salvar no meio (menu do concurso
  não tem Salvar); um reset volta ao save antigo com a equipe intacta.
- `ITEM_PARK_BALL` já existe (Pal Park); o `case BALL_PARK` só vale quando alguém joga uma Park Ball, que hoje só o
  concurso faz.
- A tabela de encounters é por mapa: o Moth Garden só dá os encounters do concurso, nenhum outro mapa muda.
- A Safari Zone não é tocada (o concurso não usa o modo nem o contador dela).

### O que não será tocado

- A Safari Zone (mapas, scripts, tabelas), os outros mapas e encounters, a tabela de treinadores (os 10 rivais já
  existem lá) e o sistema de captura de batalhas normais fora do concurso.
- Layout, objetos e eventos dos dois mapas (do usuário).

### Riscos e dúvidas

- Pergunta de trocar dentro da batalha: o `yesnobox` existe, mas preciso de strings novas com nomes e níveis nas
  duas espécies; se ficar frágil, plano B (perguntar depois da batalha).
- Desmaio do lead durante o concurso: terminar o concurso exige cortar o caminho de derrota do `CB2_EndWildBattle`
  antes do whiteout (e igual no veneno do campo); é o ponto mais delicado.
- Catch rate: taxa normal da espécie x1,5 (Park Ball). A pontuação e os rivais ficam como definidos acima.
- Nada será compilado por mim (regra do repo): você roda `make -j20`.

## Pontos de código

- `src/bug_catching_contest.c` + `include/bug_catching_contest.h` + `include/constants/bug_catching_contest.h` (novos):
  estado em EWRAM, tabela dos 10 rivais, sorteio, pontuação, ranking, specials, `BugContest_DoWhiteOut`.
- `data/scripts/bug_catching_contest.inc` (novo): fim por tempo/bolas/desistência, resultados, prêmio, textos.
- `data/maps/VerdanturfTown_MothGarden_Entrance/scripts.inc`: oficial, cena de resultados, saída antecipada, 10 rivais,
  esconder rivais. `data/maps/MothGarden/scripts.inc`: guarda (volta ao portão se o concurso não está rodando).
- `include/constants/flags.h` (10 flags `FLAG_HIDE_BUG_CONTEST_RIVAL_N`, eram `FLAG_UNUSED_0x02B`-`0x034`),
  `include/constants/vars.h` (`VAR_BUG_CONTEST_STATE` = antigo `VAR_UNUSED_0x40DB`, `VAR_BUG_CONTEST_PRIZE` = antigo
  `VAR_UNUSED_0x40DC`), `include/constants/battle.h` (`BATTLE_TYPE_BUG_CONTEST` = antigo `BATTLE_TYPE_14`, sem uso).
- `src/battle_controller_player.c`: botão da mochila vira jogar Park Ball; texto do menu "Ball NN".
- `src/battle_util.c`: `HandleAction_ThrowBall` (Park Ball + contador do concurso), `TryRunFromBattle` (fugir sempre).
- `src/battle_interface.c`: `CanThrowLastUsedBall` desligado no concurso.
- `src/battle_script_commands.c`: `case BALL_PARK` x1,5; `Cmd_givecaughtmon` com ramo do concurso e a pergunta de
  trocar (`yesnobox` no mesmo estilo da pergunta de enviar o Pokémon para a equipe).
- `data/battle_scripts_2.s`: sem apelido no concurso; fim por bolas zeradas. `data/battle_scripts_1.s`: derrota do
  concurso sem perder dinheiro.
- `src/battle_message.c` + `include/constants/battle_string_ids.h`: 3 strings novas.
- `src/battle_setup.c`: tipo de batalha do concurso e fim por bolas zeradas. `src/overworld.c`: `DoWhiteOut` /
  `CB2_WhiteOut` terminam o concurso. `src/field_control_avatar.c`: passo. `src/start_menu.c`: menu do concurso.
  `src/wild_encounter.c`: sem batalha dupla no concurso. `src/party_menu.c`: sem Fly no concurso.
- `src/data/wild_encounters.json`: `MAP_MOTH_GARDEN` com os 12 slots, taxa 30.
- `data/specials.inc`, `data/event_scripts.s`: specials e include do novo `.inc`.

## Progresso

- [x] Levantamento aprovado (2026-10-19)
- [x] Implementação (2026-10-19)
- [x] Build e `make check` (2026-10-19: o usuário rodou `make check`, sem novas falhas)
- [ ] Teste no emulador (parcial: captura, troca e avaliação ok; ver o Progresso)
- [ ] Commits
- [ ] Merge na `master`

2026-10-19: implementação escrita seguindo o levantamento. Verificado só por leitura do código (não compilei, regra do
repositório; o usuário roda `make -j20`). Nada foi testado no jogo.

2026-10-19: primeiro teste do usuário no emulador, falhou: as capturas iam para a party e o 6º para o PC, e a equipe
guardada apagava os 5 ao voltar. Causa: `displaydexinfo` deixa `gBattleCommunication[0]` alto e o caminho do concurso
pulava o passo do apelido, que o zerava; `givecaughtmon` começava em "entregar à party". Corrigido com um
`setbyte gBattleCommunication, 0` antes do `givecaughtmon` (`BattleScript_BugContestGiveCaughtMon`).

2026-10-19: testes do usuário no emulador, OK: várias capturas seguidas, pergunta de trocar com Sim e com Não, Pokémon
mantido e avaliado no fim, capturado com a party cheia indo para a caixa, gatilho do balcão (recusar entrar e recusar
deixar a equipe), prêmios Oran Berry e Sun Stone, e batalhas normais fora do concurso. Falhou: Fly/Teleport/Eon Flute
(ver abaixo, corrigido, aguardando teste). NÃO testado ainda: acabar por passos, por bolas, Retire, sair do jardim e
desmaio (o usuário acredita que acabam do mesmo jeito), prêmios do 2º e 3º lugar, mochila cheia, gatilho sem dinheiro,
ovo ou Pokémon desmaiado, e a Safari Zone.

2026-10-19: ajustes de texto a pedido do usuário. (1) Anúncios do 1º, 2º e 3º lugar quebravam a caixa de texto: o nome
(classe + nome do rival) e a espécie agora ficam em linhas separadas; mesma quebra nas falas dos rivais e em alguns
textos longos. (2) Padrão de maiúsculas dos scripts do jogo: nos scripts de mapa o jogo usa POKéMON, nomes de lugar e
de evento em maiúsculas, BAG, BOX, EGG e CONTEST (2484 POKéMON contra 24 Pokémon). Os textos dos scripts do concurso
foram ajustados (POKéMON, MOTH GARDEN, PARK BALLS, BUG CATCHING CONTEST, BAG, BOX, EGG, CONTEST). As strings de batalha
em `src/battle_message.c` seguem o padrão misto da expansão ("Safari Balls", "Pokémon"), então ficaram como estão. O
nome do mapa no mapa regional já era MOTH GARDEN, só o texto dos diálogos estava em minúsculas. Não testado no jogo.

2026-10-19: melhorias do portão pedidas pelo usuário (objetos novos no mapa: uma placa em (4,1) e uma recepcionista
`OBJ_EVENT_GFX_CABLE_CLUB_RECEPTIONIST` em (6,9)). Decisões: (1) a placa mostra só os 3 primeiros do último concurso e
fica só em memória (não vai para o save); sem resultado em memória, ela diz que ainda não houve concurso. (2) Depois de
qualquer fim de concurso o jogador chega no warp (4,21) e é levado andando até (4,9), em frente à recepcionista, que
anuncia os resultados e entrega o prêmio. Implementado: script `..._EventScript_WalkToReceptionist` (movimento de 12
passos para cima e virar à direita, conferido na grade de colisão do layout), `..._EventScript_Receptionist` (entrega o
prêmio guardado ou explica onde está o ranking), `..._EventScript_RankingBoard`, special `BugContest_CheckBoard` e texto
"Welcome back!" nos resultados; os dois scripts foram ligados no `map.json`. Não testado no jogo.

2026-10-19: testado pelo usuário, OK: prêmio do 2º lugar, placa de ranking e recepcionista (o jogador anda até ela e ela
anuncia os resultados). Também testado pelo usuário, OK: gatilho do balcão com Pokémon desmaiado, e todos os fins do concurso (passos, bolas,
Retire, saída do jardim e desmaio) com o mesmo resultado. Ainda sem teste: prêmio do 3º lugar, mochila cheia, gatilho
sem dinheiro e com ovo, Fly/Teleport/Eon Flute bloqueados e a Safari Zone.

Diferenças em relação ao levantamento:
- Sem `ResetBugContest` e sem flag de modo: o estado é só EWRAM e some no reset, como previsto. Viagem rápida:
  2026-10-19, o usuário viu que Fly/Teleport/Eon Flute ainda funcionavam. Todos perguntam
  `Overworld_MapTypeAllowsTeleportAndFly` (Fly, Teleport, Eon Flute, mapa regional e Pokénav), então o bloqueio agora
  está nessa função (`src/overworld.c`), e o do `SetUpFieldMove_Fly` foi retirado. Escape Rope e Dig dependem de
  `allow_escaping`, que está `false` nos dois mapas. Não usei `map_type` do mapa porque ele também muda iluminação e
  clima, e a função cobre qualquer mapa. Aguardando teste do usuário.
- A pergunta de trocar mostra só os nomes das duas espécies (os buffers de texto da batalha são pequenos demais para
  nível e HP). A tela comparativa do Gold ficou para depois.
- O concurso usa `gNumBugContestBalls` próprio (não o da Safari) e `B_OUTCOME_NO_SAFARI_BALLS` só como "sem bolas".
- Os objetos (oficial, 10 rivais e o gatilho do balcão) não foram criados por mim: o usuário os colocou no Porymap
  (conferido em 2026-10-19: scripts e flags batem).

## Pendências

- Colocar no portão o oficial e os 10 rivais, com os scripts e flags indicados no topo do `scripts.inc` do portão.
- Gatilho (coord event) do portão: decidido em 2026-10-19 pôr na tile em frente ao balcão, (9,4), por onde passa quem
  vai das portas para o lado do jardim (as jardineiras fecham a fileira de baixo). Script
  `VerdanturfTown_MothGarden_Entrance_EventScript_EntranceTrigger` (só age andando para oeste; recusar empurra o
  jogador de volta). Os warps (3,21) e (4,21) do portão ficam só para a chegada do jardim e deixam de ser alcançáveis a
  pé. Falta criar o coord event no Porymap.
- Posição de chegada no portão depois do concurso: hoje usa o warp 0 do portão, ou seja, a tile (4,21).
- Grama do Moth Garden com os encounters (já cadastrados), e a taxa 30 pode ser ajustada depois do teste.
- Recalibrar as pontuações dos rivais depois de ver as pontuações reais dos Pokémon capturados.
- Tela de comparação ao trocar o Pokémon (com nível e HP), se o usuário quiser.

## Como testar

1. Compile: `make -j20`. Se der erro de compilação, me mande a saída.
2. Fale com o oficial do portão (depois de colocá-lo no Porymap): entrar custa ¥5.000. Com mais de um Pokémon na equipe
   ele pergunta se pode ficar com o resto.
3. Dentro do Moth Garden, ande na grama: o menu de início mostra os passos restantes (250) e as bolas (20); o menu de
   batalha tem "Ball 20" no lugar de "Bag" e joga a Park Ball direto.
4. Capture um Pokémon, depois outro: deve perguntar se quer trocar. Sim troca, Não solta o novo.
5. Teste os fins: acabar os passos, gastar as 20 bolas, desistir (Retire no menu de início), sair do jardim pela
   saída, e deixar o Pokémon desmaiar. Em todos o jogador volta ao portão e ouve os resultados (3º, 2º, 1º e o seu).
6. Confira o prêmio (1º Sun Stone, 2º Everstone, 3º Sitrus Berry, sem lugar Oran Berry), a equipe de volta, e o
   Pokémon capturado na equipe (ou na caixa se a equipe estiver cheia).
7. Confira que fora do concurso nada mudou: batalha selvagem normal, jogar bola da mochila, fugir, capturar, e a
   Safari Zone.
