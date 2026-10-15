# Working on plans

Larger pieces of work in this repo are driven by a plan file in `plans/`.
One plan = one file = one branch (`plan/<name>`) = one session.
Small fixes that are not tied to a plan are not covered by these rules.

Use `/new-plan <name>` to start a plan and `/plans` to list them all.

## At the start of a session

1. Run `git branch --show-current`. If the branch starts with `plan/`, read the plan on that branch before doing anything else: the file in `plans/` whose `Branch:` header matches the current branch.
2. If there is no matching plan, or more than one, ask the user. Do not guess.

## Branch lock

- A plan may only be worked on from its own branch. Before any edit, commit, merge or file move, check that the current branch equals the plan's `Branch:`. If it does not, stop and tell the user.
- Never edit, move, delete or commit another plan's file. Other `.md` files in `plans/` belong to other branches and sessions, even when they show up here as untracked.

## Survey first

- Before executing a plan, do a survey (levantamento): list what will change (files, symbols, flags and vars), what is shared with other code and could break, and what stays untouched.
- Write the survey into the plan's `Levantamento` section and wait for the user's approval. Do not edit code before that.

## While executing

- Record progress in the plan as you go, not at the end: the `Progresso` list near the top, with dates. Say plainly what is verified and what is untested.
- Record every decision the user makes in the plan, with the date.
- If what you are about to write conflicts with the plan, say so and ask instead of deviating silently.
- Do not compile. The build is the user's: give them the command (`make -j20`).

## Git

- Use `git add` with explicit paths. Never `git add -A` or `git add .`: `plans/` and a few csv/txt files are usually untracked and must not be committed by accident.
- Commit the plan file as the first commit of its branch.
- Do not push, and do not merge into master, unless the user asks.

## Language

- The conversation with the user and the plan files are in Portuguese.
- Everything else is in English: code, comments, commit messages, this file and the skills. Game text stays in English.
