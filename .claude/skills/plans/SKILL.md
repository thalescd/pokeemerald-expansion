---
name: plans
description: List every plan in plans/ with its status and branch, without switching branches. Read-only. Only runs when the user types /plans.
disable-model-invocation: true
allowed-tools: Bash(git branch *), Bash(git ls-tree *), Bash(git show *), Bash(git diff *), Bash(git log *), Bash(git rev-list *), Bash(git status *), Bash(git rev-parse *), Read, Glob
---

# /plans

Show all plans at a glance. This is read-only: never switch branches, never create, edit or delete a file.

## Where plans come from

1. **Active plans** (on a branch): for each branch from `git branch --list 'plan/*' --format='%(refname:short)'`, find the plan files added on it with `git diff --name-only --diff-filter=A master...<branch> -- plans/`. Read the header with `git show <branch>:"<file>"` (the first 15 lines or so) and take the `Status:` value.
2. **Done plans** (in master): files tracked under `plans/` on master (`git ls-tree --name-only master plans/`). Take `Status:` from the header; if the file has no header, show `concluído (em master)`. A plan that is already in master is listed once, even if its branch still exists (then note the branch as merged).
3. **Loose plans** (no branch yet): `.md` files in the working directory's `plans/` that are untracked (`git status --short -- plans/`, lines starting with `??`). Show them as `solto, sem branch`.

## Output

A table in Portuguese:

| Plano | Status | Branch | À frente da master | Último commit |
|---|---|---|---|---|

- Ahead of master: `git rev-list --count master..<branch>`. Leave it empty for plans without a branch.
- Last commit: `git log -1 --format=%cs <branch>`.
- Under the table, say which branch is checked out now (`git branch --show-current`) and which loose plans could be adopted with `/new-plan <name>`.
- If there are no plans at all, say so in one line.
