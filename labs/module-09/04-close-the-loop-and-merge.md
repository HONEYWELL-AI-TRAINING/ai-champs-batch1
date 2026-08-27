# Exercise 4: Close the Loop — Fix, Re-Judge, Merge for Real

**Duration:** 20 minutes | **Format:** Individual, hands-on, `sample-repo/` + GitHub + Copilot Chat

## Objective

Slide 10's rule: *"a judge that never escalates isn't confident, it's miscalibrated"* — and the inverse matters just as much: a fix that never reaches a clean pass isn't done. Land Module 07's actual reference solution, prove it clean, and make the one decision that was never delegated to the judge.

---

## Step 1 — Apply the Real Fix (5 minutes)

Replace the flawed version with Module 07's actual reference fix — the one that was verified but never applied:

```c
size_t diag_format_message(char *buf, size_t buf_len, diag_error_code_t code, const char *detail)
{
    if (buf == NULL || buf_len == 0) {
        return 0;
    }
    if (detail == NULL) {
        detail = "(no detail)";   /* string literal — no allocation, nothing to free */
    }
    int written = snprintf(buf, buf_len, "[0x%04X] %s", (unsigned)code, detail);
    if (written < 0 || (size_t)written >= buf_len) {
        return 0;
    }
    return (size_t)written;
}
```

Remove the now-unnecessary `#include <stdlib.h>` if nothing else in the file needs it. Confirm the test from Exercise 3 doesn't need to change — it was testing the right *behavior* all along; only the *implementation* was wrong.

## Step 2 — Push the Fix, Re-Run Both Checks (7 minutes)

```sh
git add src/diag_formatter.c
git commit -m "Replace malloc-based fallback with a string literal — no allocation"
git push
```

Confirm on the PR:
- [ ] `make test` still passes
- [ ] The dynamic-allocation gate now **passes** — no `malloc`/`calloc`/`realloc`/`free` anywhere in `src/` or `include/`
- [ ] Run the sanitizer build locally one more time and confirm clean:
  ```sh
  gcc -fsanitize=address,undefined -Wall -Wextra -o /tmp/diag_san tests/test_diag_formatter.c src/diag_formatter.c && /tmp/diag_san
  ```

Re-run the Exercise 2 judge prompt with the updated diff. Confirm:
- [ ] Memory Safety now scores **PASS**, citing the literal-string change
- [ ] Overall recommendation reads "Ready for human review," not "Needs fixes"
- [ ] The judge still doesn't state a merge decision — that hasn't changed just because the score improved

Post this clean result as a PR comment too, so the PR's history shows the real before/after — not just the final state.

## Step 3 — Close the Loop: The Human Decision (8 minutes)

This is the one step Slides 7 and 10 both insist can never be automated. Review the PR yourself, one more time, as a human:

- [ ] Read the actual diff — not the judge's summary of it
- [ ] Confirm the fix matches `sample-repo/docs/CODE_REVIEW_REPORT.md`'s original recommendation
- [ ] Confirm nothing outside `diag_formatter.c` and its test changed

If your facilitator designated your PR as the one that actually lands on `main` for this cohort:

```sh
gh pr merge --squash
```

If not — your PR still did everything real; leave it open for comparison and say so in your Exercise 5 notes. Either way, you made the merge decision yourself; nothing in this pipeline made it for you.

---

## Checkpoint

- [ ] The real fix is in your PR, the gate passes clean, and the judge's re-score is on record
- [ ] You personally reviewed the diff before deciding anything — not just read the judge's summary
- [ ] You can state whether your PR merged or stayed open, and why, in one sentence

## Where This Goes Next

Exercise 5 compares what you just measured — review time, rounds, what got caught before a human looked — against what a fully manual pass would have cost.
