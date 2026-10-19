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

## Levantamento

**Obrigatório antes de executar. Aprovado pelo usuário em:** _(ainda não)_

Base no Emerald: a Safari Zone (`src/safari_zone.c`, `src/battle_controller_safari.c`, `BATTLE_TYPE_SAFARI`,
`data/scripts/safari_zone.inc`, `Route121_SafariZoneEntrance`). Já tem: flag de modo, contador de bolas, menu de
batalha sem luta, entrada com taxa (500 lá, 5000 aqui), saída por script e menu de início só com "Retire".

- **O que muda** (arquivos, símbolos, flags e vars):
  - Novo `src/bug_catching_contest.c` + `include/bug_catching_contest.h`: flag de modo, 20 Park Balls
    (`ITEM_PARK_BALL` já existe), timer de 20 min, pokémon capturado do concurso (um só, em EWRAM, não salvo),
    pontuação (HP máx ×4 + Atk + Def + Spe + SpA + SpD + HP atual/8 + 1 se segura item, mais termo de IV a definir),
    tabela dos 10 rivais, sorteio de 5, ranking 1º/2º/3º, guarda e devolução do resto da equipe.
  - `src/battle_setup.c`: `BattleSetup_StartWildBattle` e um `DoBugContestBattle` (como `DoSafariBattle`).
  - Menu da batalha do concurso só com Bola/Fugir (reaproveitar o controlador Safari ou uma variante dele).
  - Captura: com o concurso ativo, o pokémon capturado NÃO vai para a equipe/PC; fica no slot do concurso, com a
    pergunta de trocar se já houver um (pontos em `data/battle_scripts_2.s` e `Cmd_givecaughtmon`).
  - `src/field_control_avatar.c`: checar o timer a cada passo (ao lado de `SafariZoneTakeStep`).
  - `src/start_menu.c`: menu de início do concurso (Retire + janela com bolas e tempo), como o do Safari.
  - `src/data/wild_encounters.json`: entrada de encounters do mapa do concurso (12 slots finais), só quando o mapa
    existir.
  - `data/specials.inc`, `data/scripts/` (novo `bug_catching_contest.inc`): scripts de entrada (taxa de 5000,
    deixar o resto da equipe com o oficial), fim por tempo/bolas, juiz e prêmios (Sun Stone, Everstone, Sitrus Berry,
    Oran Berry), textos em inglês.
  - Flags/vars novas em `include/constants/flags.h` e `vars.h` (modo concurso; estado da entrada/saída).
- **O que é compartilhado e pode quebrar:**
  - `GetSafariZoneFlag()` e `BATTLE_TYPE_SAFARI` aparecem em ~15 pontos (batalha, healthbox, reshow, controllers,
    start menu, overworld, double battle). Reusar o modo Safari muda o comportamento da Safari Zone real se eu errar a
    condição; por isso o contest precisa de flag própria e o Safari deve continuar idêntico.
  - Multa/taxa e retorno de mapa seguem o padrão da Safari; `src/overworld.c` limpa o flag do Safari em várias
    transições (linhas ~409-486 e ~1634) e eu preciso fazer o mesmo para o flag do concurso.
  - A tabela de encounters é compartilhada com o mapa novo; nenhum mapa existente é alterado.
  - A pontuação depende de `CalculateMonStats` e dos campos de IV de `struct Pokemon`; só leio, não altero.
- **Reaproveitamento do Pokémon Contest do Emerald (`src/contest.c`, `src/contest_util.c`):** só como padrão, não
  dá para chamar nada direto (tudo é `static` ou preso a `gContestOpponents`/4 competidores):
  - Sorteio sem repetição (`SetContestants`, contest.c ~2962): lista de candidatos, sorteia, remove da lista. Copiar o
    padrão para sortear 5 dos 10 rivais.
  - Desempate no ranking (`DetermineFinalStandings`, contest.c ~3648): sorteia uma ordem aleatória por competidor
    antes de ordenar. Usar a mesma ideia no ranking 1º/2º/3º (melhor que o empate do Gold, que favorece quem entra
    por último na comparação).
  - Specials que põem nomes nas strings do script (`BufferContestantTrainerName`, `BufferContestWinnerTrainerName`,
    contest_util.c ~1953): mesmo padrão para o juiz anunciar "classe + nome" e a espécie.
  - Não reaproveitar: rodadas/apelos, IA, tela de resultados (4 competidores fixos) e `struct ContestPokemon`
    (carrega golpes e condições que não usamos). O resultado sai em caixas de texto, como no Gold.
- **Mapas e NPCs (feitos pelo usuário; recomendação, a confirmar):** dois mapas, como no Gold e na Safari Zone.
  - Portão (entrada e resultado): oficial (entrada, taxa, juiz, prêmios) + 10 objetos de rival, todos começando
    escondidos; depois do concurso os 5 sorteados aparecem. Mínimo: 11 NPCs. Dá para reduzir a 1 oficial + 5 rivais
    trocando sprite e fala por variável, mas dá mais script; a recomendação é 11 objetos simples.
  - Mapa do concurso (a grama): nenhum NPC obrigatório; só a grama com os encounters finais, a saída e, se quiser,
    itens. Como os encounters são por mapa, a grama do concurso nunca dá encontro fora do concurso.
  - Motivo de dois mapas: se a grama e o oficial ficarem no mesmo mapa, o mapa sempre daria os encounters do concurso
    (ou precisaria de script para bloquear a área), e o fim do concurso teria que reorganizar o mesmo mapa (esconder a
    grama, mostrar os rivais). Com portão separado, o fim é só um warp para o portão.
- **O que não será tocado:**
  - A Safari Zone do Hoenn (mapas, scripts, tabelas), os mapas existentes e os encounters das rotas.
  - O mapa do concurso e o portão (criados pelo usuário); só deixo o espaço (IDs de flags e objetos dos 10 rivais).
  - Sistema de captura de batalhas normais, evolução, EXP, e a tabela de treinadores (os 10 rivais já existem lá).
- **Riscos e dúvidas:**
  - Termo de IV da pontuação: DECIDIDO, sem termo extra; os IVs entram só via stats (a tabela de calibração vale).
  - Timer: DECIDIDO (2026-10-17), contar passos como na Safari Zone, com 250 passos (decidido pelo usuário em
    2026-10-17; constante única, fácil de mudar depois do teste). Mesmo ponto de ligação da Safari
    (`field_control_avatar.c`, ao lado de `SafariZoneTakeStep`), e a janela do menu de início já mostra passos e bolas.
    Isso troca a ideia anterior de usar o contador de tempo de jogo (`play_time.c`), descartada pelo usuário.
  - Guardar o resto da equipe: não pode salvar durante o concurso (menu só com "Retire", como no Safari), mas um reset
    ou desligar no meio devolve o save antigo com a equipe intacta (não salvamos no meio), então o backup só precisa
    existir em EWRAM.
  - Pergunta de trocar o pokémon capturado: DECIDIDO manter. A batalha já tem caixa Sim/Não em script
    (`yesnobox` / `Cmd_yesnobox`, e o mesmo mecanismo de `Cmd_trygivecaughtmonnick`), então a pergunta dentro da
    batalha parece viável. Plano B (se complicar muito): perguntar depois da batalha e, se o jogador tiver mais de
    1 Pokémon, ele escolhe qual sai da equipe.
  - Catch rate / Park Ball: DECIDIDO usar a taxa normal da espécie com a Park Ball como no Gold, onde a Park Ball
    multiplica por 1,5 (mesma conta da Great Ball no Gold). No Emerald, `ITEM_PARK_BALL` existe (`secondaryId =
    BALL_PARK`) mas `ComputeBallData` (`battle_script_commands.c` ~8018) não tem um `case BALL_PARK`, então hoje vale
    1x. Falta adicionar `case BALL_PARK: ball->multiplier = 150`. Atenção: se a batalha do concurso reaproveitar
    `BATTLE_TYPE_SAFARI`, a captura usa `safariCatchFactor` em vez da taxa da espécie; o concurso precisa de tipo de
    batalha próprio (ou desviar essa condição) para usar a taxa normal.
  - Nada será compilado por mim (regra do repo): você roda `make -j20`.

## Pontos de código

<Arquivos e trechos que serão alterados, depois do levantamento aprovado.>

## Progresso

- [ ] Levantamento aprovado
- [ ] Implementação
- [ ] Build e `make check`
- [ ] Teste no emulador
- [ ] Commits
- [ ] Merge na `master`

<Registrar aqui, com data, o que foi feito e o que foi verificado ou ainda não foi testado.>

## Pendências

<O que ficou em aberto e depende de uma decisão ou de um teste.>

## Como testar

<Roteiro passo a passo para o usuário conferir no emulador.>
