# Plano: efeito do Odd Keystone (Spiritomb)

- **Branch:** `plan/odd-keystone`
- **Status:** planejado
- **Criado em:** 2026-10-15
- **Escopo (pode alterar):** <arquivos, pastas ou sistemas que este plano pode tocar>
- **Fora de escopo:** <o que não deve ser tocado>

Este plano só pode ser trabalhado na branch acima. As regras de trabalho (levantamento antes de executar, anotar o
progresso aqui, uma decisão por vez) estão no `CLAUDE.md`.

Valores de **Status**: `planejado`, `em andamento`, `concluído`, `abandonado`.

---

**Status: não implementado.** Este documento só levanta as mudanças. Nada foi editado no código, e eu não compilo:
o build e o `make check` ficam por conta do usuário (`make -j20`, no WSL).

Levantamento feito em 2026-10-15.

## A ideia

Odd Keystone segurado por **Spiritomb**: quando ele entra em campo, o item é **consumido** e Spiritomb ganha **+1 em
Attack, Defense, Sp. Attack e Sp. Defense** (Bulk Up + Calm Mind de uma vez, sem Speed). Como o item some, em contexto
normal isso acontece uma vez por batalha.

## Situação atual do item

- `ITEM_ODD_KEYSTONE` (`include/constants/items.h`, id 155) existe em `src/data/items.h` **sem `holdEffect`**: é só um
  item vendável (preço 2100, `ITEM_TYPE_SELLABLE`, `flingPower = 80`). Pode ser dado a um Pokémon, mas não faz nada.
- Só aparece em dois lugares: o **minigame de mineração** (`MININGID_ODD_KEY_STONE`, tabela `ItemRarityTable_Rare`
  em `src/mining_minigame.c`) e o texto da Pokédex do Spiritomb. Não há nenhum mapa, script, loja ou trainer
  que o entregue. **Nada disso precisa mudar.**
- Spiritomb aqui tem `ABILITY_VESSEL_OF_RUIN` como única habilidade (`gen_4_families.h`). Não interage com o item, mas
  vale lembrar ao escrever os testes (o Sp. Atk dos adversários é reduzido).

## Por que um hold effect novo

O motor (1.17) decide tudo por `enum HoldEffect`. Sem `holdEffect` o item nunca entra em `ItemBattleEffects`. O molde
mais próximo é o **Berserk Gene** (`HOLD_EFFECT_BERSERK_GENE`): dispara na entrada em campo, mexe em stats do próprio
portador e some. A lista de arquivos que o Berserk Gene toca é exatamente o checklist abaixo
(`grep HOLD_EFFECT_BERSERK_GENE`: constantes, tabela, `battle_hold_effects.c`, `battle_debug.c`, `items.h`, teste).

A restrição "só Spiritomb" fica **dentro da função do efeito**, como Thick Club e Light Ball fazem em
`battle_util.c` (checam a espécie). Não existe um campo de espécie no `HoldEffectInfo`.

## Mudanças

### 1. Constante: `include/constants/hold_effects.h`
- [ ] Adicionar `HOLD_EFFECT_ODD_KEYSTONE` antes de `HOLD_EFFECT_COUNT` (junto do `HOLD_EFFECT_BERSERK_GENE`,
      que é o último hoje).
- **Atenção:** o comentário do arquivo diz que `HOLD_EFFECT_COUNT` não pode passar de 127 (cabe em `u8`). O arquivo tem
  127 linhas `HOLD_EFFECT_*` contando `NONE` e `COUNT`, ou seja, `COUNT` vale hoje 126. Com este efeito vira 127:
  **é o último slot livre**. Qualquer hold effect futuro vai exigir mexer nesse limite. (Contagem por grep, confirme
  no build.)

### 2. Tabela de timing: `src/data/hold_effects.h`
- [ ] Nova entrada:
  ```c
  [HOLD_EFFECT_ODD_KEYSTONE] =
  {
      .onSwitchIn = TRUE,
  },
  ```
  `onSwitchIn` faz o bloco `FIRST_EVENT_BLOCK_ITEMS` de `src/battle_switch_in.c` chamar o item, e também entra em
  `IsForceTriggerItemActivation` (ver "Comportamentos que vêm de graça").

### 3. Lógica: `src/battle_hold_effects.c`
- [ ] Nova `static enum ItemEffect TryOddKeystone(enum BattlerId battler)`, perto de `TryBerserkGene`:
  - retorna `ITEM_NO_EFFECT` se `GET_BASE_SPECIES_ID(gBattleMons[battler].species) != SPECIES_SPIRITOMB`
    (item **não** é consumido em outro portador);
  - percorre `STAT_ATK`, `STAT_DEF`, `STAT_SPATK`, `STAT_SPDEF` (lista explícita, sem `SPEED`) e enfileira
    `SetStatChange(battler, stat, 1)` **só nos que `CompareStat(battler, stat, MAX_STAT_STAGE, CMP_LESS_THAN, ability)`
    deixa subir**. Contrary entra aqui de graça, como no Berserk Gene;
  - se nenhum puder subir, retorna `ITEM_NO_EFFECT` (item preservado; só acontece com stages herdados, ex. Baton Pass);
  - senão faz `gEffectBattler = gBattleScripting.battler = battler;`, `BattleScriptCall(BattleScript_ConsumableItemStatRaise);`
    e retorna `ITEM_STATS_CHANGE`.
- [ ] `case HOLD_EFFECT_ODD_KEYSTONE:` no `switch` de `ItemBattleEffects`.
- Não precisa de script novo: `BattleScript_ConsumableItemStatRaise` (`data/battle_scripts_1.s`) já faz pop-up do item,
  animação, `trybattlerstatchange ... STAT_CHANGE_ITEM | STAT_CHANGE_CERTAIN` e **`removeitem`**. É o mesmo caminho
  dos Terrain Seeds (`TryTerrainSeeds`), que também enfileira stats e chama esse script.
- Mensagem resultante, uma por stat: `The Odd Keystone boosted Spiritomb's Attack!` (string
  `STRINGID_USINGITEMSTATOFPKMNROSE`). São **quatro caixas de texto** por ativação. É o padrão do motor
  (Weakness Policy mostra duas); se incomodar no jogo, a alternativa é um script próprio com uma mensagem só, o que
  exige string nova em `battle_message.c`.

### 4. Nome de debug: `src/battle_debug.c`
- [ ] `[HOLD_EFFECT_ODD_KEYSTONE] = COMPOUND_STRING("Odd Keystone"),` na tabela de nomes (perto de
      `[HOLD_EFFECT_BERSERK_GENE]`). Sem isso o menu de debug mostra vazio para o item.

### 5. Item: `src/data/items.h`, entrada `ITEM_ODD_KEYSTONE`
- [ ] Adicionar `.holdEffect = HOLD_EFFECT_ODD_KEYSTONE,`.
- [ ] **Descrição nova** (hoje diz só "A vital item. Voices can be heard from it occasionally."). Proposta no formato
      de 3 linhas do Berserk Gene:
  ```
  "Boosts Spiritomb's\n"
  "Atk, Def, Sp. Atk and\n"
  "Sp. Def once."
  ```
  Conferir no jogo se a segunda linha cabe na caixa; senão encurtar.
- [ ] Decisão pequena: `.sortType` está `ITEM_TYPE_SELLABLE`. Mudar para `ITEM_TYPE_HELD_ITEM` só afeta a ordenação
      automática da mochila. Recomendo trocar, já que agora é item de segurar; `.price`, `.flingPower` e o resto ficam.

### 6. IA (opcional, não recomendo agora)
`src/battle_ai_switch.c` tem um `switch (aiHoldEffect)` ("Item stat changes") que simula, na decisão de troca da IA,
os stats que o item vai dar. O Berserk Gene **não** está lá, então o motor já tolera item de entrada fora dessa
simulação. Um Spiritomb de trainer com Odd Keystone vai ativar o efeito normalmente; a IA só não o prevê.

## Testes

Novo arquivo `test/battle/hold_effect/odd_keystone.c`, modelado em `berserk_gene.c` (mesmo `ASSUMPTIONS` com
`ASSUME(gItemsInfo[ITEM_ODD_KEYSTONE].holdEffect == HOLD_EFFECT_ODD_KEYSTONE)`).
Todos os nomes começam com `Odd Keystone`, porque `TESTS=` casa por **prefixo** (ver memória do harness).

- [ ] Spiritomb entra em batalha: `ANIMATION ... B_ANIM_STATS_CHANGE`, as quatro `MESSAGE`, e em `THEN` os quatro
      `statStages` em `DEFAULT_STAT_STAGE + 1` e **`SPEED` inalterado**.
- [ ] O item some: `EXPECT_EQ(player->item, ITEM_NONE)`.
- [ ] Ativa ao entrar por troca (como o teste "Berserk Gene activates on switch in").
- [ ] Sai e volta: segunda entrada **sem** mensagem e stages em +0 (prova a "uma vez por batalha").
- [ ] Dupla: funciona na posição direita (como o par de testes simples/dupla do Berserk Gene).
- [ ] Outro Pokémon (Wobbuffet) segurando: nenhuma mensagem e `item` continua `ITEM_ODD_KEYSTONE`.
- [ ] Klutz no Spiritomb: não ativa e o item fica. `Ability(ABILITY_KLUTZ)` fora dos slots é trava permanente, o que
      aqui é o desejado.
- [ ] Contrary no Spiritomb (opcional): stats caem −1 em vez de subir, como o Berserk Gene.

Rodar (por conta do usuário): `make -j20 check TESTS="Odd Keystone"`. Conferir se `Tests TOTAL` bate com o número de
testes do arquivo e se `ASSUMPTIONS_FAILED` é zero (um `ASSUME` quebrado desliga o teste em silêncio).

## Comportamentos que vêm de graça (conferir, não implementar)

Por ser `onSwitchIn`, o efeito também roda em `IsForceTriggerItemActivation`, que é chamado quando o item "volta a
poder ser usado" ou é recebido. Na prática, um Spiritomb ganha o boost **no meio da batalha** se:

- receber o item por **Trick / Switcheroo / Bestow** ou **Pickpocket** (`activateitemeffects`). O teste "Berserk Gene
  does not cause an infinite loop" (Bestow) mostra o precedente;
- o Klutz dele sumir (Gastro Acid, Skill Swap...) ou **Embargo / Magic Room** acabarem (`ACTIVATION_ON_USABLE_AGAIN`);
- pegá-lo de volta com **Pickup** ou **Recycle** (`removeitem` guarda o item usado; Recycle devolve e dispara de novo).

É a mesma regra de todo item de entrada (Berserk Gene, Room Service, seeds) e bate com "uma vez por batalha em
contextos normais". Se for indesejado em algum caso, o ponto de corte é o `TryOddKeystone`, não a tabela.

Outras notas:
- **Transform / Imposter:** a checagem usa a espécie atual, como Thick Club e Light Ball. Um Ditto que copiou Spiritomb
  segurando o item ativa. Aceito, a menos que você queira bloquear com `volatiles.transformed` (que `TryBoosterEnergy`
  já usa).
- **Unnerve não bloqueia:** `IsUnnerveBlocked` só vale para itens do bolso de berries.
- **Magic Room / Embargo / Klutz** são tratados por `GetBattlerHoldEffect`, que já alimenta o bloco de entrada.
- **Opportunist / Mirror Herb do adversário** copiam os boosts pelo mesmo caminho de stat change do Weakness Policy;
  não deve precisar de código, mas não escrevi teste para isso.
- **Fling:** continua `flingPower = 80` sem efeito extra.

## Fora do escopo

- Como o jogador obtém um Spiritomb e quantos Odd Keystones aparecem (hoje só mineração, na raridade "rare").
- Preço de venda (2100 → 1050 ao vender).
- Mudança nos trainers: para usar em NPC, basta `Item: Odd Keystone` no `.party` de um Spiritomb.

## Ordem sugerida

1. Constante, tabela e `battle_debug.c` (trio mecânico).
2. `TryOddKeystone` e o `case`.
3. Entrada em `items.h` (`holdEffect`, descrição, `sortType`).
4. Testes; o usuário roda `make -j20 check TESTS="Odd Keystone"`.
5. Teste no emulador: Spiritomb com o item numa batalha de trainer, e uma troca no meio da luta.
