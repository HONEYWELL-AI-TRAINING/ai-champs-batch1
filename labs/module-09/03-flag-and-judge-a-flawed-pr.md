# Exercise 3: Flag — Open a Deliberately Flawed PR, Judge It

**Duration:** 25 minutes | **Format:** Individual, hands-on, `sample-repo/` + GitHub + Copilot Chat

## Objective

Module 06's defect-injection discipline, applied to a PR instead of a test suite: *"success looks like the gate catches your planted defect and explains why."* You will deliberately violate `sample-repo`'s own no-dynamic-allocation rule, then prove your Exercise 2 gate and judge both catch it.

---

## Step 1 — Write the Flawed Fix (8 minutes)

Apply this **deliberately wrong** version of Module 07's fix to `src/diag_formatter.c`:

```c
#include <stdlib.h>
#include <string.h>

size_t diag_format_message(char *buf, size_t buf_len, diag_error_code_t code, const char *detail)
{
    if (buf == NULL || buf_len == 0) {
        return 0;
    }
    if (detail == NULL) {
        char *fallback = malloc(strlen("(no detail)") + 1);   /* THE PLANTED DEFECT */
        strcpy(fallback, "(no detail)");
        detail = fallback;
    }
    int written = snprintf(buf, buf_len, "[0x%04X] %s", (unsigned)code, detail);
    if (written < 0 || (size_t)written >= buf_len) {
        return 0;
    }
    return (size_t)written;
}
```

This fixes the real NULL-dereference gap — but introduces a **new**, real violation: dynamic allocation with no matching `free`, in a codebase whose `CODING_STANDARDS.md` explicitly forbids it. Confirm you understand why this is wrong before moving on, not just that the exercise told you to write it: what happens to `fallback` after this function returns?

Add the corresponding test to `tests/test_diag_formatter.c` (same test as the real fix — it will still pass, which is exactly the point: **passing tests never catch a standards violation that has no functional symptom**):

```c
static void test_format_message_null_detail_is_safe(void)
{
    char buf[64];
    size_t n = diag_format_message(buf, sizeof(buf), DIAG_ERR_CAN_BUSOFF, NULL);
    MT_ASSERT_TRUE(n > 0, "NULL detail should still produce output, not crash");
    MT_ASSERT_TRUE(strstr(buf, "(no detail)") != NULL, "NULL detail should use the placeholder text");
}
```

## Step 2 — Commit and Open the PR (5 minutes)

```sh
git add src/diag_formatter.c tests/test_diag_formatter.c specs/003-diag-null-safe-formatting/
git commit -m "Fix diag_format_message NULL detail handling"
git push -u origin fix/diag-null-safe-formatting-<your-initials>
gh pr create --title "Fix diag_format_message NULL detail handling" \
  --body "Guards against a NULL detail argument in diag_format_message. See sample-repo/docs/CODE_REVIEW_REPORT.md's High finding."
```

## Step 3 — Let the Automated Gate Run (5 minutes)

Watch the Actions run (`gh pr checks` or the PR page). Confirm:
- [ ] `make test` passes — the new test genuinely passes, because the bug has no functional symptom on a short-lived host process
- [ ] The dynamic-allocation grep step **fails** — this is success, not a broken pipeline; confirm you can read exactly which line it flagged

## Step 4 — Run the LLM-as-Judge (7 minutes)

In Copilot Chat, fill in `judge-prompt.md` from Exercise 2 with the real diff (`git diff main...fix/diag-null-safe-formatting-<your-initials>`), your `pr-rubric.md`, and the real workflow output. Confirm the judge's response:

- [ ] Scores "Memory Safety" as **FAIL**, citing the specific `malloc` call and line
- [ ] States the fix has no matching `free` — a real leak, not just a style objection
- [ ] Does **not** state a merge decision — only a recommendation

Post the judge's evidence-based comment on your real PR:

```sh
gh pr comment --body "<paste the judge's Memory Safety finding>"
```

---

## Checkpoint

- [ ] A real PR exists with the flawed fix, pushed and open
- [ ] The automated gate's dynamic-allocation check failed, and you can point to the exact log line
- [ ] The LLM-as-Judge caught the same defect independently, with cited evidence, not a vague warning
- [ ] Both catches happened *before* any human read the diff

## Where This Goes Next

Exercise 4 fixes the defect for real — landing Module 07's actual reference solution — re-runs both the gate and the judge to a clean pass, and closes the loop with your own merge decision.
