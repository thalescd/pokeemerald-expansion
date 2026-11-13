# Plano: MOVE REFORGE

- **Branch:** `plan/move-reforge`
- **Status:** planejado
- **Criado em:** 2026-11-13
- **Escopo (pode alterar):** o novo move Reforge (constante, dados, animação) e a refatoração do Roost para dentro de `EFFECT_RESTORE_HP` (efeito do move, efeito adicional, volátil, script de batalha, IA e testes que citam `EFFECT_ROOST`).
- **Fora de escopo:** os outros moves de cura (Recover, Slack Off, Purify etc.), Burn Up e Double Shock (só leitura, serviram apenas de exemplo de efeito adicional configurável), learnsets/TMs (a decisão de quem aprende o Reforge fica para depois).

Este plano só pode ser trabalhado na branch acima. As regras de trabalho (levantamento antes de executar, anotar o
progresso aqui, uma decisão por vez) estão no `CLAUDE.md`.

Valores de **Status**: `planejado`, `em andamento`, `concluído`, `abandonado`.

---

## Objetivo

Criar o move **Reforge**: igual ao Roost (cura metade do HP máximo), mas do tipo Aço, e em vez de tirar o tipo Voador
até o fim do turno, tira o tipo Aço até o fim do turno.

Aproveitar para generalizar: em vez de ter um efeito só para o Roost (`EFFECT_ROOST`), o Roost e o Reforge viram
`EFFECT_RESTORE_HP` com um efeito adicional configurável que diz qual tipo some até o fim do turno, no mesmo estilo
do Burn Up e do Double Shock (só como exemplo de que efeito adicional pode ter configuração própria; o mecanismo do Roost é outro). Resultado: um efeito de move a menos e a flexibilidade de criar "Roost de outro tipo"
só com dados.

## Decisões

- 2026-11-13: o Reforge é um Roost de tipo Aço (mesma cura, mesmo "perde o tipo até o fim do turno", mas com Aço).
- 2026-11-13: direção de design pedida pelo usuário: remover `EFFECT_ROOST`, usar `EFFECT_RESTORE_HP` e pôr a remoção do tipo como efeito adicional configurável. É "no campo da ideia": vale se o levantamento não achar impedimento. Ver o levantamento abaixo.
- 2026-11-13: PP e descrição do Reforge iguais às do Roost.
- 2026-11-13: o Reforge se comporta como o Roost com mono-tipo (mesma config `B_ROOST_PURE_FLYING`), usa a animação do Recover por enquanto, e o handler/constante de fim de turno e o volátil ganham nomes genéricos.
- 2026-11-13: o campo do tipo se chama `.argument.suppressedType` e o efeito não usa `.self` (sempre atua no usuário).
- 2026-11-13: o tipo a remover fica configurado **no efeito adicional** (`additionalEffects`), não no `.argument` do move. Burn Up e Double Shock foram citados só como exemplo de efeito adicional com configuração, não como mecanismo a reutilizar.

## Levantamento

**Obrigatório antes de executar. Aprovado pelo usuário em:** 2026-11-13 ("vamos começar", com todas as pendências resolvidas)

Feito em 2026-11-13, só leitura de código (nada foi editado fora deste arquivo).

### Como as peças funcionam hoje

- **Roost** (`MOVE_ROOST`, `src/data/moves_info.h:9748`): `EFFECT_ROOST`. O script `BattleScript_EffectRoost` (`data/battle_scripts_1.s:1438`) faz `tryhealhalfhealth`, depois `setroost` e pula para `BattleScript_HealTarget`.
- `setroost` (`Cmd_setroost`, `src/battle_script_commands.c:3007`) só liga o volátil `roostActive` (1 bit). O volátil é desligado no fim do turno por `HandleEndTurnRoost` (`src/battle_end_turn.c:1022`).
- O tipo "sumido" é calculado **na leitura**, em `GetBattlerTypes` (`src/battle_util.c:9865`): se `roostActive`, Voador vira `TYPE_MYSTERY`, e se o mon é Voador puro vira Normal (config `B_ROOST_PURE_FLYING`). Os tipos reais em `gBattleMons[].types` nunca são alterados. Isso é o que faz o efeito ser temporário.
- **Burn Up / Double Shock** (citados só como exemplo de efeito adicional configurável) funcionam de outro jeito: `EFFECT_FAIL_IF_NOT_ARG_TYPE` + `.argument = { .type = TYPE_FIRE }` + efeito adicional `MOVE_EFFECT_REMOVE_ARG_TYPE` com `.self = TRUE`. O handler `HandleSetEffectRemoveArgType` (`src/battle_set_effect.c:249`) chama `RemoveBattlerType`, que **escreve** `TYPE_MYSTERY` nos tipos reais: a remoção é **permanente** (até trocar de mon). O próprio código tem um `TODO use the arg field from move effects` nesse ponto.
- `EFFECT_RESTORE_HP` (Recover, Slack Off e outro) usa `BattleScript_EffectRestoreHp` (`data/battle_scripts_1.s:1686`).

### Conclusão principal

A ideia é viável, mas **não dá para reaproveitar `MOVE_EFFECT_REMOVE_ARG_TYPE`**: ele é permanente e o Roost é temporário. É preciso um efeito adicional novo (nome provisório `MOVE_EFFECT_SUPPRESS_TYPE`) que liga o volátil em vez de escrever nos tipos. O `REMOVE_ARG_TYPE` do Burn Up fica como está. O novo efeito não toma emprestado nada do Burn Up: o tipo vem do próprio efeito adicional (ver Decisões).

### O que muda

**Para o move novo**
- `include/constants/moves.h`: `MOVE_REFORGE` na lista de custom moves, antes de `MOVES_COUNT_CUSTOM` (atualmente a lista termina em `MOVE_ZONE_SHIFT`).
- `src/data/moves_info.h`: entrada `[MOVE_REFORGE]` (nome, descrição, tipo Steel, `EFFECT_RESTORE_HP`, PP, flags copiadas do Roost: `healingMove`, `snatchAffected`, `ignoresProtect`, `mirrorMoveBanned`, `zMove`).
- Animação: o Reforge aponta para `gBattleAnimMove_Recover` por enquanto (nenhum arquivo de animação novo).
- `data/battle_anim_scripts.s`, `gBattleAnimMove_Roost` (linha ~119): acrescentar `call HealingEffect` e `waitforvisualfinish` antes do `end`, para manter o brilho de cura depois que o Roost passar a usar `BattleScript_EffectRestoreHp`.

**Para a generalização**
- `include/constants/battle_set_effect.h`: novo `MOVE_EFFECT_SUPPRESS_TYPE` (antes de `NUM_MOVE_EFFECTS`).
- `src/battle_set_effect.c`: novo handler registrado na tabela (perto da linha 1345); sem texto de tela, só liga o volátil.
- `include/constants/battle.h:309`: o volátil `roostActive` (1 bit) vira um volátil que guarda **qual tipo** está suprimido (ex.: `typeSuppressed`, `(enum Type, ...)`), porque o 1 bit só serve para o Voador.
- `src/battle_util.c:9865` (`GetBattlerTypes`): trocar `== TYPE_FLYING` pelo tipo guardado no volátil.
- `src/battle_main.c:5669-5674` (Revelation Dance): mesma troca; hoje repete a lógica do Roost na mão.
- `src/battle_end_turn.c:1022` e `include/constants/battle_end_turn.h:40`: o nome `ENDTURN_ROOST` e `HandleEndTurnRoost` passam a limpar o volátil novo (renomear ou manter o nome é decisão, ver Pendências).
- `data/battle_scripts_1.s`: `BattleScript_EffectRestoreHp` passa a chamar `setadditionaleffects`; `BattleScript_EffectRoost` e o comando `setroost` (`Cmd_setroost`, `B_SCR_OP_SETROOST`, macro em `asm/macros/battle_script.inc:341`, declaração em `include/battle_scripts.h:652`) somem, por ficarem sem uso.
- `include/constants/battle_move_effects.h:156` e `src/data/battle_move_effects.h:1065`: remoção de `EFFECT_ROOST`.
- `src/data/moves_info.h` (`MOVE_ROOST`): `.effect = EFFECT_RESTORE_HP` + efeito adicional `{ .moveEffect = MOVE_EFFECT_SUPPRESS_TYPE, .argument.suppressedType = TYPE_FLYING }`, sem `.self`. O Reforge igual, com `TYPE_STEEL`.
- `include/move.h:46` (struct `AdditionalEffect`): novo membro `enum Type suppressedType:8` na union `argument` (as entradas da union têm o nome do que guardam: `absorbPercentage`, `wrapped`, `brokeProtect`) (cabe nos 3 bytes do `_Static_assert` da linha 242, igual a `brokeProtect:8`). O handler lê `se->additionalEffect->argument.suppressedType` e age sempre em `cv->battlerAtk` (o usuário), então o `.self` não é necessário. `GetMoveArgType` e o `.argument` do move não são tocados.
- IA: tirar `EFFECT_ROOST` de `src/battle_ai_main.c:2235`, `:4582`, `:6309` e `src/battle_ai_util.c:302` (em 2235 e 4582 ele já vem junto com `EFFECT_RESTORE_HP`; em 6309 também; em `battle_ai_util.c:302` é preciso conferir que `EFFECT_RESTORE_HP` já está coberto na mesma lista).
- Testes que citam `EFFECT_ROOST` e precisam de `EFFECT_RESTORE_HP` nos `ASSUME`: `test/battle/move_effect/roost.c:6`, `revelation_dance.c:11`, `transform.c:181`, `test/battle/ability/dancer.c:239`, `test/battle/ai/values_moves_over_splash.c:272`. Testes novos do Reforge (copiar a forma do `roost.c`).

### O que é compartilhado e pode quebrar

1. **`BattleScript_RestoreHp` é compartilhado com o Purify** (`data/battle_scripts_1.s:808`). Se `setadditionaleffects` entrar no trecho final compartilhado, o Purify também roda esse comando. Hoje o Purify não tem efeito adicional, então seria um no-op, mas é um ponto a conferir no código final.
2. **O Roost hoje usa `BattleScript_HealTarget`, o Recover usa `BattleScript_RestoreHp`.** O `HealTarget` toca `B_ANIM_SIMPLE_HEAL` no script; o `RestoreHp` não, porque o brilho do Recover e do Slack Off já está dentro da animação do move. Ao juntar, o Roost perderia o brilho; resolvido pondo `call HealingEffect` + `waitforvisualfinish` no fim de `gBattleAnimMove_Roost` (ver Pendências, item 2).
3. **Ordem relativa:** hoje o `setroost` roda antes da cura. Com efeito adicional, o tipo sumiria depois da cura. Não muda o resultado (a cura não depende de tipo), mas é uma diferença de ordem.
4. **`Cmd_setadditionaleffects`** (`src/battle_script_commands.c:1796`) pula quando `IsBattlerUnaffectedByMove(gBattlerTarget)`. Como o alvo é o próprio usuário, deve passar, mas precisa ser verificado com Heal Block e com Protect (o Roost tem `ignoresProtect`).
5. **Sheer Force / Shield Dust / Serene Grace:** com `chance` 0 o efeito é "primário" e não é afetado. Mesmo caminho do Haze e do Reflect, que já são status moves com efeito adicional.
6. **Remover `EFFECT_ROOST` renumera** os `EFFECT_*` seguintes. Efeitos não são gravados no save, então não afeta saves; só obriga recompilar tudo.
7. **Tera:** hoje `GetBattlerTypes` ignora o Roost se o mon está Terastalizado e `RemoveBattlerType` ignora Tera. O volátil novo deve manter a mesma regra.

### O que não será tocado

- `MOVE_EFFECT_REMOVE_ARG_TYPE`, `GetMoveArgType` e os moves Burn Up / Double Shock (o comportamento permanente fica).
- Os outros moves que usam `EFFECT_RESTORE_HP` (Recover, Slack Off) e o `EFFECT_SOFTBOILED`, `EFFECT_MORNING_SUN` etc.
- Learnsets, TMs, egg moves e `all_learnables.json` (a lista de quem aprende o Reforge é outra decisão).
- A config `B_ROOST_PURE_FLYING` (fica valendo para o Roost; ver pendência do Aço puro).

### Riscos e dúvidas

- **Mono-tipo Aço (Aço puro, como o Registeel):** o Roost vira Normal (Gen 5+) quando o mon é Voador puro. O Reforge em um Aço puro deveria virar Normal também? Precisa de decisão.
- **Terceiro tipo:** o Roost atual só olha `types[0]` e `types[1]`, não `types[2]` (tipo dado por Trick-or-Treat/Forest's Curse). Manter igual para não mudar comportamento, mas o Reforge em um mon com Aço no terceiro slot não o removeria. Aceitável?
- **Volátil guardando o tipo:** um mon só usa um move por turno, então um único tipo basta. Casos de borda: Instruct/Dancer repetindo um dos dois (mesmo tipo, sem problema).
- **Não compilo e não rodo teste** (regra do CLAUDE.md). A validação é do usuário: `make -j20` e depois `make check TESTS="roost"` etc. no WSL.

## Pontos de código

Ver a lista de arquivos no item de implementação em Progresso.

## Progresso

- [x] Levantamento aprovado (2026-11-13)
- [x] Implementação (2026-11-13, escrita; **não compilada**)
- [ ] Build e `make check`
- [ ] Teste no emulador
- [ ] Commits
- [ ] Scripts 1, 2 e 3 do debug descartados (nunca commitados)
- [ ] Merge na `master`

- 2026-11-13: adicionados **Script 2** (batalha selvagem contra Snorlax nv 20 com **só** Flamethrower; `move2` a `move4` = `MOVE_NONE`) e **Script 3** (idem com Thunderbolt) no menu de debug (`data/scripts/debug.inc`), usando `setwildbattle` + `createmon B_SIDE_OPPONENT` + `dowildbattle`. Temporários, como o Script 1. Não compilado, não testado.
- 2026-11-13: Script 1 do debug ajustado: Snorlax nv 50 agora tem Recover, Slack Off, Roost e Reforge; e dá também um Corviknight nv 50 com Roost, Reforge, Recover e Drill Peck. Não compilado, não testado.
- 2026-11-13: adicionado o **Script 1** do menu de debug (`data/scripts/debug.inc`): dá um Snorlax nv 50 com Recover, Slack Off e Roost, para comparar as animações. É **temporário**: deve ser removido (voltar a `release` + `end`) antes de fechar o plano. Não compilado, não testado.
- 2026-11-13: decisão: o `data/scripts/debug.inc` não entra em nenhum commit; commits planejados: (1) refactor do Roost, (2) Reforge, (3) docs do plano.
- 2026-11-13: implementação escrita, **nada compilado nem testado** (build é do usuário). Feito:
  - `include/move.h`: `suppressedType:8` na union `argument` de `AdditionalEffect`.
  - `include/constants/battle_set_effect.h` + `src/battle_set_effect.c`: `MOVE_EFFECT_SUPPRESS_TYPE` e `HandleSetEffectSuppressType` (liga o volátil no atacante; não mexe nos tipos reais).
  - `include/constants/battle.h`: `roostActive` (1 bit) virou `suppressedType` (`enum Type`).
  - `src/battle_util.c` (`GetBattlerTypes`) e `src/battle_main.c` (Revelation Dance): usam o tipo guardado em vez de `TYPE_FLYING` fixo.
  - Fim de turno: `ENDTURN_SUPPRESSED_TYPE` / `HandleEndTurnSuppressedType` (zera o volátil).
  - Removidos: `EFFECT_ROOST` (constante e entrada em `battle_move_effects.h`), `BattleScript_EffectRoost`, `setroost` (`Cmd_setroost`, `B_SCR_OP_SETROOST`, macro, extern) e os 4 `case EFFECT_ROOST` da IA.
  - `data/battle_scripts_1.s`: `BattleScript_EffectRestoreHp` chama `setadditionaleffects` logo depois de `waitanimation` e **antes** do label `BattleScript_RestoreHp`, para o Purify (que pula para o label) não passar por ele.
  - `src/data/moves_info.h`: Roost em `EFFECT_RESTORE_HP` + efeito adicional (Voador); novo `MOVE_REFORGE` (Aço, mesmos dados do Roost, animação do Recover); `include/constants/moves.h`: `MOVE_REFORGE` depois de `MOVE_ZONE_SHIFT`.
  - `data/battle_anim_scripts.s`: `gBattleAnimMove_Roost` termina com `call HealingEffect` + `waitforvisualfinish`.
  - Testes: 4 `ASSUME` trocados para `MoveHasAdditionalEffect(MOVE_ROOST, MOVE_EFFECT_SUPPRESS_TYPE)` (`roost.c`, `revelation_dance.c`, `transform.c`, `dancer.c`); `case EFFECT_ROOST` removido de `values_moves_over_splash.c`; novo `test/battle/move_effect/reforge.c` (3 testes, nunca rodados).
  - **Mudança de comportamento da IA (decidir):** `ShouldRecordStatusMove` (`src/battle_ai_util.c`) tinha `EFFECT_ROOST` na lista de "Medium odds". Sem ele, o Roost cai nas odds padrão (`ASSUME_ALL_STATUS_ODDS`). Não preservei de propósito para não criar código novo; se quiser manter, dá para checar `MoveHasAdditionalEffect(move, MOVE_EFFECT_SUPPRESS_TYPE)`.
- 2026-11-13: plano criado e levantamento escrito. Nenhum código foi editado. Nada compilado nem testado.

## Pendências

- **Descartar os Scripts 1, 2 e 3 do debug** (`Debug_EventScript_Script_1/2/3` em `data/scripts/debug.inc`) ao terminar o plano. Decisão de 2026-11-13: eles **não são commitados**; descartar = `git restore data/scripts/debug.inc` (o arquivo fica só como alteração local).

Decisões que o usuário precisa tomar antes de eu editar código:

1. ~~Onde fica o tipo configurável~~ — resolvido em 2026-11-13 (ver Decisões): no `.argument` do próprio efeito adicional.
2. ~~Animação de cura simples no Roost~~ — resolvido em 2026-11-13: o brilho de cura já faz parte da animação de move no Recover e no Slack Off (`gBattleAnimMove_Recover` e `gBattleAnimMove_SlackOff` terminam com `call HealingEffect`, confirmado no código e no jogo pelo usuário). Só o Roost não tem isso na animação, porque o brilho vinha do script `BattleScript_HealTarget`. Solução: pôr `call HealingEffect` + `waitforvisualfinish` no fim de `gBattleAnimMove_Roost` (o Reforge usa a do Recover, que já tem). Nenhuma mudança de script de batalha. (O Script 1 do debug serviu para comparar as três animações.)
3. ~~Aço puro vira Normal?~~ — resolvido em 2026-11-13: mesmo comportamento do Roost, governado pela mesma config `B_ROOST_PURE_FLYING` (hoje `GEN_LATEST`: mono-tipo vira Normal; com `GEN_4` ficaria sem tipo, `TYPE_MYSTERY`, nos dois moves). A config não será renomeada.
4. ~~Animação do Reforge~~ — resolvido em 2026-11-13 (provisório): usar a animação do Recover nos primeiros testes (`gBattleAnimMove_Recover`); o usuário decide depois se troca por outra. Com isso o Reforge não precisa de animação própria nem do `call HealingEffect` extra.
5. ~~Renomear `ENDTURN_ROOST` / `HandleEndTurnRoost`~~ — resolvido em 2026-11-13: sim, nomes genéricos, já que o efeito é o mesmo para os dois moves. Nomes propostos (o usuário não escolheu os nomes exatos): `ENDTURN_SUPPRESSED_TYPE`, `HandleEndTurnSuppressedType`, volátil `VOLATILE_SUPPRESSED_TYPE` / `suppressedType`. `TYPE_NONE` (= 0) serve de "nada suprimido" e o volátil cabe em 5 bits (`NUMBER_OF_MON_TYPES` = 21).
6. ~~Descrição e PP do Reforge~~ — resolvido em 2026-11-13: iguais às do Roost (PP `B_UPDATED_MOVE_DATA >= GEN_9 ? 5 : 10`, texto de cura adaptado para Aço).

## Como testar

1. `make -j20`, depois no WSL `make check TESTS="roost"` e `make check TESTS="reforge"` (e `revelation_dance`, `transform`, `dancer`, `values_moves_over_splash`).
2. No debug: Scripts… > Script 2 ou 3 (batalha selvagem contra Snorlax nv 20 com Flamethrower / Thunderbolt, para machucar o seu Pokémon); Scripts… > Script 1 (Snorlax nv 50 com Recover/Slack Off/Roost/Reforge e Corviknight nv 50 com Roost/Reforge/Recover/Drill Peck).
3. Verificar em batalha: Roost em Voador, Reforge em Aço (perde o tipo só naquele turno, volta no seguinte), mono-tipo vira Normal, brilho de cura do Roost e do Reforge, cura falha com HP cheio e com Heal Block, Purify ainda cura.
