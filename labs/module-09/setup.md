# Setup — Module 09 Lab

Do this before the session starts. Takes about 10 minutes.

## 1. What You Need

- [ ] Push access to a personal branch on this training repository, or your own fork — confirm which your facilitator wants this cohort to use
- [ ] `git` and the GitHub CLI (`gh`) installed and authenticated:
  ```sh
  gh auth status
  ```
  If not logged in: `gh auth login`
- [ ] `sample-repo/` on a clean `make test` (see [Module 06's setup.md](../module-06/setup.md) if you haven't run it recently)
- [ ] GitHub Copilot Chat available in your IDE — today's LLM-as-Judge step runs through Copilot Chat, the same tool every earlier module has used, not a separate hosted judge service

## 2. Pick Your Branch Naming

If this cohort shares one repository, **use a personal branch suffix** to avoid collisions — for example:

```sh
git checkout -b fix/diag-null-safe-formatting-<your-initials>
```

Confirm with your facilitator whether your PR should actually be merged at the end of Exercise 4, or left open for comparison — in a shared cohort repo, usually only one participant's PR actually merges `sample-repo`'s fix; everyone else's gate and judge results are still real and still graded, they just don't all land on `main`.

## 3. Confirm Module 07's Reference Fix Is Still Unapplied

```sh
cd sample-repo
grep -n "no detail" src/diag_formatter.c
```

This should return nothing — if it already exists, someone already landed today's fix on this branch; check with your facilitator before continuing, since Exercise 3 depends on the fix not existing yet.

## 4. This Module's Own Artifacts

New things land in a feature branch and a real GitHub PR — not silently in `main`:

```
.github/workflows/pr-quality-gate.yml   (Exercise 2 — your branch only, until merged)
src/diag_formatter.c                     (Exercise 3 flawed version, Exercise 4 real fix)
tests/test_diag_formatter.c              (Exercise 3/4 — new NULL-detail test)
```

Plus one real PR against this repository, with real gate output and real review comments.

---

Once `gh auth status` succeeds and your branch is created, start with [Exercise 1](01-quality-gate-and-rubric.md).
