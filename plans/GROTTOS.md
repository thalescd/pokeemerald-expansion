# Plano: GROTTOS

- **Branch:** `plan/grottos`
- **Status:** planejado
- **Criado em:** 2026-11-17
- **Escopo (pode alterar):** <arquivos, pastas ou sistemas que este plano pode tocar>
- **Fora de escopo:** <o que não deve ser tocado>

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

## Levantamento

**Obrigatório antes de executar. Aprovado pelo usuário em:** _(ainda não)_

- **O que muda** (arquivos, símbolos, flags e vars):
- **O que é compartilhado e pode quebrar:**
- **O que não será tocado:**
- **Riscos e dúvidas:**

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

- Onde as tabelas de grotto ficam: campo novo `grotto_mons` no `wild_encounters.json` (editável no Porymap) ou tabela em
  C separada. Depende de o `tools/wild_encounters/wild_encounters_to_header.py` aceitar campo novo.
- Divisão em planos: este plano cobre só as grottos ou também a remoção da secret base, das decorações e o
  encolhimento do save?

## Como testar

<Roteiro passo a passo para o usuário conferir no emulador.>
