# Exercise 2: Configure the Gate and the LLM-as-Judge Workflow

**Duration:** 15 minutes | **Format:** Individual, hands-on, `sample-repo/` + GitHub

## Objective

Slide 7's five stages: PR opened → evidence gathered → rubric scored → confidence checked → human closes the loop. Build the first two stages as real automation; the last three you'll drive through Copilot Chat, the same tool every earlier module used — no new hosted judge service required.

---

## Step 1 — Add the Automated Evidence-Gathering Workflow (8 minutes)

On your branch, create `.github/workflows/pr-quality-gate.yml`:

```yaml
name: PR Quality Gate

on:
  pull_request:
    paths:
      - 'labs/module-02/sample-repo/**'

jobs:
  build-test-evidence:
    runs-on: ubuntu-latest
    defaults:
      run:
        working-directory: labs/module-02/sample-repo
    steps:
      - uses: actions/checkout@v4
      - name: Build and run full test suite
        run: make test
      - name: Sanitizer build
        run: |
          for f in tests/test_*.c; do
            name=$(basename "$f" .c)
            gcc -fsanitize=address,undefined -Wall -Wextra -o /tmp/"$name" "$f" src/*.c 2>&1 | tee /tmp/"$name".build.log
            /tmp/"$name" | tee /tmp/"$name".run.log
          done
      - name: Check for dynamic allocation (standards gate)
        run: |
          if grep -rn "malloc\|calloc\|realloc\|free(" src/ include/; then
            echo "::error::Dynamic allocation found — violates CODING_STANDARDS.md"
            exit 1
          fi
```

This is the **evidence-gathering** stage — deterministic, no LLM involved yet. It also encodes one **auto-fail** gate directly (the dynamic-allocation check from your rubric) — Slide 6's rule that timing/memory violations get an automatic fail with no confidence threshold to save it.

Commit this file to your branch now, before Exercise 3's PR — it needs to already exist to run against what you push next.

```sh
git add .github/workflows/pr-quality-gate.yml
git commit -m "Add PR quality gate workflow for sample-repo changes"
```

## Step 2 — Define the LLM-as-Judge Procedure (7 minutes)

Write this prompt template into `sample-repo/specs/003-diag-null-safe-formatting/judge-prompt.md` — you'll paste it into Copilot Chat, filled in with real values, once a PR exists:

```markdown
You are reviewing a pull request against sample-repo's pr-rubric.md.

INPUT:
- Diff: <paste `git diff main...<branch>` output>
- Rubric: <paste pr-rubric.md>
- Test evidence: <paste the workflow run's test/sanitizer output>
- Spec: specs/003-diag-null-safe-formatting/spec.md (if it exists on this branch)

TASK: Score each rubric row PASS / WARN / FAIL, citing the specific line of
evidence for each score — a line number, a test name, or a rubric rule, not
a general impression. For any FAIL, state exactly what would need to change.
State an overall confidence (High/Low) in your scoring.

OUTPUT: One evidence-based comment per rubric row, plus an overall
recommendation: "Ready for human review" or "Needs fixes before human review."
Never state a merge decision — that's not yours to make.
```

Notice the last line — matching Slide 7's rule exactly: *"the name is precise: 'judge,' not 'approver' — it evaluates and informs; the merge decision is never delegated away."*

---

## Checkpoint

- [ ] `.github/workflows/pr-quality-gate.yml` exists, committed, and includes the dynamic-allocation auto-fail check
- [ ] `judge-prompt.md` exists with a template that demands cited evidence per row, not general impressions
- [ ] You can state, in one sentence, why this procedure never asks the LLM for a merge decision

## Where This Goes Next

Exercise 3 pushes a real PR with a deliberately flawed fix and runs both stages — the automated gate and the Copilot Chat judge — against it for real.
