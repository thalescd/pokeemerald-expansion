# Plano: <TÍTULO>

- **Branch:** `plan/<slug>`
- **Status:** planejado
- **Criado em:** <AAAA-MM-DD>
- **Escopo (pode alterar):** <arquivos, pastas ou sistemas que este plano pode tocar>
- **Fora de escopo:** <o que não deve ser tocado>

Este plano só pode ser trabalhado na branch acima. As regras de trabalho (levantamento antes de executar, anotar o
progresso aqui, uma decisão por vez) estão no `CLAUDE.md`.

Valores de **Status**: `planejado`, `em andamento`, `concluído`, `abandonado`.

---

## Objetivo

<O que queremos e por quê, em poucas linhas, sem termos técnicos demais.>

## Decisões

<Uma linha por decisão: data, o que foi decidido e por quê. Só entra aqui o que o usuário decidiu.>

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

<O que ficou em aberto e depende de uma decisão ou de um teste.>

## Como testar

<Roteiro passo a passo para o usuário conferir no emulador.>
