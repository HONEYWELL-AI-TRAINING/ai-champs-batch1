# Exercise 1: What a Quality Gate Checks, Ground the Rubric

**Duration:** 20 minutes | **Format:** Individual, hands-on, `sample-repo/`

## Objective

Slide 5's rule: *"nothing new here — every category a PR quality gate checks is a discipline from an earlier module; this gate just checks that the discipline actually happened."* Before configuring anything, map those six categories onto `sample-repo`'s real, existing constraints — not a generic checklist.

---

## Step 1 — Name the Six Categories Against Real Files (8 minutes)

For each category below, name the specific `sample-repo` file or artifact that already defines what "pass" means. Do this yourself, don't skip to the answer:

| Category | What checks this in `sample-repo`? |
|---|---|
| Specification Compliance | ? |
| Coding & Design Standards | ? |
| Test Evidence | ? |
| Compiler / Build Results | ? |
| Regression Risk | ? |
| Security & Safety Considerations | ? |

Compare against this (don't peek until you've tried):

- **Specification Compliance** → `specs/*/spec.md`'s acceptance criteria (Module 04's REQ/CON/AC chain)
- **Coding & Design Standards** → `docs/CODING_STANDARDS.md` and `docs/ARCHITECTURE.md`'s layering rules
- **Test Evidence** → `make test` output plus Module 06's traceability discipline
- **Compiler / Build Results** → a clean build with `-Wall -Wextra`, zero warnings
- **Regression Risk** → the *full* `make test` suite, not just new tests (Module 06's rule)
- **Security & Safety Considerations** → the sanitizer build (`-fsanitize=address,undefined`) plus `docs/CODE_REVIEW_REPORT.md`'s existing findings about memory safety and bounds checks

## Step 2 — Build the Rubric (10 minutes)

Slide 6's calibration data ranks what embedded reviewers check closest: timing behavior, memory safety, error handling, hardware/driver boundaries, concurrency, resource constraints. `sample-repo` is host-simulated with no real timing hardware and no concurrency — so your rubric has to be honest about what's actually checkable here, not copy the generic list unchanged.

Write `sample-repo/specs/003-diag-null-safe-formatting/pr-rubric.md` (same feature folder Module 07 used — if your checkout has a different next-available number, use that):

```markdown
# PR Quality Rubric — sample-repo

| Category | Applies Here? | Check | Auto-Fail Threshold |
|---|---|---|---|
| Memory Safety | Yes | No malloc/free anywhere (CODING_STANDARDS.md); sanitizer clean | Any dynamic allocation |
| Error Handling & Boundaries | Yes | NULL/zero-length guards on every public function taking a pointer | Missing guard on a new public function |
| Coding & Design Standards | Yes | snake_case, module prefix, layering rules (ARCHITECTURE.md) | Any layering violation |
| Timing Behaviour | Partial | No unbounded loops; host stub has no real timing budget | N/A — flag as "not verifiable on host," don't fake a PASS |
| Hardware/Driver Boundaries | Yes | No non-HAL code calls hal_gpio_*/hal_adc_* directly | Any boundary violation |
| Concurrency | No | sample-repo is single-threaded, host-simulated | N/A |
| Resource Constraints | Yes | Stack-only, no static buffers added without justification | New static/global state |
```

Notice the two `N/A` rows — a rubric that fakes a PASS on something it can't actually check is worse than one that says "not verifiable here."

---

## Checkpoint

- [ ] You mapped all six PR-quality categories to a specific, real `sample-repo` file or command, not a generic description
- [ ] `pr-rubric.md` exists, with honest `N/A` rows where `sample-repo`'s host-simulated nature makes a category genuinely inapplicable
- [ ] The "Memory Safety" row explicitly names the no-dynamic-allocation rule — you'll need this exact rule in Exercise 3

## Where This Goes Next

Exercise 2 turns this rubric into an actual automated gate and an LLM-as-Judge procedure that scores real evidence against it.
