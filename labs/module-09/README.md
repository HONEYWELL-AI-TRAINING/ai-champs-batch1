# Module 09 — Guided Lab: PR Process Automation, Quality Gates and LLM-as-Judge

**Duration:** 1.5 hours | **Format:** Guided Configure + Judge-a-PR Lab | **Audience:** Embedded/Firmware Developers, Architects, Testers, DevOps/SRE, Engineering Leads

Finally lands the real feature Module 07 only ever kept as a documented reference solution — the `diag_format_message` NULL-`detail` fix — through an actual PR, automated quality gate, and LLM-as-Judge review. You'll first prove the gate catches a real, deliberately planted standards violation, then fix it, get a clean pass, and merge for real.

---

## Learning Objectives

By the end of this lab, you will have:

1. Named the six categories a PR quality gate checks and confirmed each one traces to a discipline an earlier module already taught.
2. Built a scored rubric from real embedded NFR categories (memory, timing, error handling, hardware boundaries), applied to `sample-repo`'s actual constraints.
3. Configured an automated quality-gate workflow and an LLM-as-Judge review step that gathers real evidence before scoring anything.
4. Opened a real PR with a deliberately flawed version of the `diag_formatter` fix, and watched the gate catch it with an evidence-based comment — not a vague warning.
5. Fixed the defect, re-run the gate to a clean pass, and closed the loop with a human merge decision — the fix from Module 07's facilitator guide, landed in `sample-repo` for real, for the first time.
6. Measured your own review cycle (time, rounds, escalation) and can state why a healthy escalation rate is never zero.

---

## Before You Start

**Complete [setup.md](setup.md) first.** This lab opens a real GitHub PR against your own branch — confirm your git and `gh` CLI access before Exercise 1.

---

## Agenda

| # | Block | Duration | Exercise File |
|---|-------|----------|----------------|
| 1 | What a Quality Gate Checks, Ground the Rubric | 20 min | [01-quality-gate-and-rubric.md](01-quality-gate-and-rubric.md) |
| 2 | Configure the Gate and the LLM-as-Judge Workflow | 15 min | [02-configure-the-quality-gate.md](02-configure-the-quality-gate.md) |
| 3 | Flag: Open a Deliberately Flawed PR, Judge It | 25 min | [03-flag-and-judge-a-flawed-pr.md](03-flag-and-judge-a-flawed-pr.md) |
| 4 | Close the Loop: Fix, Re-Judge, Merge for Real | 20 min | [04-close-the-loop-and-merge.md](04-close-the-loop-and-merge.md) |
| 5 | Review-Cycle Debrief | 10 min | [05-review-cycle-debrief.md](05-review-cycle-debrief.md) |

---

## The Real Feature This Lab Finally Ships

Module 07 fixed `sample-repo`'s real, documented gap — `diag_format_message` passing an unchecked `NULL` `detail` into `snprintf`'s `%s` — but by that module's own convention (matching Module 06), the verified fix stayed in [Module 07's facilitator guide](../module-07/facilitator-guide.md), never applied to the actual files, so the exercise stayed open for the next learner. Today it finally lands:

- `src/diag_formatter.c` gets the one-line guard
- `tests/test_diag_formatter.c` gets the new `detail == NULL` test

But not on the first try. Exercise 3 has you open a PR with a **deliberately flawed** version of this exact fix — one that violates `sample-repo/docs/CODING_STANDARDS.md`'s explicit "no dynamic allocation" rule — and proves your quality gate catches it before a human ever needs to. Exercise 4 fixes it and merges the real version.

---

## Deliverables Checklist

Walk out of Module 09 with:

- [ ] A written rubric mapping `sample-repo`'s real constraints to the six quality-gate categories (Exercise 1)
- [ ] A working PR quality-gate configuration and an LLM-as-Judge review procedure, both tested (Exercise 2)
- [ ] A real, pushed PR containing a deliberately flawed fix, with a gate/judge result that caught the flaw with cited evidence (Exercise 3)
- [ ] The same PR, fixed, re-judged clean, and merged — `sample-repo` now actually contains Module 07's fix (Exercise 4)
- [ ] Your own review-cycle numbers: time, rounds, and what the gate caught before you looked (Exercise 5)

---

## What's Next

**Module 10 — Agent Prism for Monitoring, Observability and Enterprise Control** (2 hours). Traces, failure patterns, and token/cost leakage — connecting everything built across Modules 02–09 to numbers a program can actually track, including the review-cycle metrics this module just taught you to measure by hand.

For facilitators: see [facilitator-guide.md](facilitator-guide.md) for the run sheet and the verified expected gate/judge output for both the flawed and fixed PR.
