---
name: new-plan
description: Start a new plan. Creates the branch plan/<name> from master and a plans/ file from the template, and commits that file as the first commit of the branch. Only runs when the user types /new-plan.
argument-hint: <plan name, e.g. "follow sprite">
disable-model-invocation: true
---

# /new-plan

Create the branch and the plan file for a new piece of work. The plan name is: $ARGUMENTS

## Names

- Title: the name in UPPERCASE with spaces, for example `FOLLOW SPRITE`. The file is `plans/<TITLE>.md`.
- Slug: the name in lowercase with hyphens, for example `follow-sprite`. The branch is `plan/<slug>`.
- If no name was given, ask for one and stop.

## Before creating anything

1. `git branch --show-current` must be `master`. If it is not, tell the user and stop: new plans always branch from master.
2. `git status --short`: there must be no modified or staged tracked files. Untracked files are fine, they follow the new branch.
3. The branch `plan/<slug>` must not exist (`git branch --list plan/<slug>`). If it does, tell the user and stop.
4. `plans/<TITLE>.md` must not be tracked on master. If it is, tell the user and stop.

## Create

1. `git switch -c plan/<slug>`.
2. If `plans/<TITLE>.md` already exists as an untracked file, **adopt it** and keep all of its content. If it has no `Branch:` line, insert only the header fields from [plan-template.md](plan-template.md) (from the `- **Branch:**` line through the `Valores de **Status**` line) right below the file's existing `# ` title line. Do not add a second title and do not delete or reword anything else.
   Otherwise copy [plan-template.md](plan-template.md) to `plans/<TITLE>.md`.
3. Fill the header: `Branch: plan/<slug>`, `Status: planejado` and today's date (and the title too, when you copied the template). Leave `Escopo` and `Fora de escopo` as placeholders unless the user already gave them. For an adopted plan whose body already states a status or a date, leave that text as it is and mention it to the user so they can tidy it.
4. Commit only that file: `git add -- "plans/<TITLE>.md"`, then commit with the message `docs: add the <name> plan`, following the repo's commit conventions and attribution lines. Never use `git add -A` or `git add .`.

## After

- Do not push. Do not start working on the plan.
- Tell the user: the branch, the plan file, that it is committed, and that the next step is the survey (levantamento). If the header still has placeholders, ask what the scope is.
- Answer in Portuguese.
