# Exercise 4: Test Against Real Sample-Repo Findings

**Duration:** 30 minutes | **Format:** Individual, hands-on, Jira + Rovo Studio

## Objective

Every earlier module in this programme insists on real, verified evidence over a self-consistency check. Today's version: file four **real** findings from `sample-repo/docs/CODE_REVIEW_REPORT.md` as Jira issues, run your agent on each, and check its suggestions against that report's own, already-published severities — an answer key nobody can dispute because it predates today's session.

---

## Step 1 — File the Four Test Issues (10 minutes)

Open `sample-repo/docs/CODE_REVIEW_REPORT.md` and file each of these as a separate Jira issue in your project, using the report's own wording for the summary/description. Do this by hand — today's exercise is testing the *triage* agent, not another creation workflow.

| # | Summary (from the report) | File |
|---|---|---|
| 1 | Stale contract comment says module is not implemented | `include/fault_monitor.h:4` |
| 2 | Public API declarations missing behavior comments | `include/can_driver.h`, `include/fault_monitor.h`, `include/state_machine.h` |
| 3 | State machine event handling lacks defensive bounds checks | `src/state_machine.c:30` |
| 4 | Unused include in fault monitor implementation | `src/fault_monitor.c:3` |

## Step 2 — Let the Agent Triage Each One (10 minutes)

Confirm your trigger fires for each new issue and produces a draft (per Exercise 3, nothing is applied yet). For each of the four, record what the agent suggested: component label, priority, and its stated rationale.

## Step 3 — Score Against the Real Answer Key (10 minutes)

Compare your agent's four suggestions against this table — derived directly from the report, not invented for this exercise:

| # | Expected Component | Expected Priority | Why |
|---|---|---|---|
| 1 | `fault_monitor` | Medium | Report labels this Medium; it's a documentation/comment accuracy issue, not a crash risk |
| 2 | Multiple — `can_driver`, `fault_monitor`, `state_machine` | Medium | This one genuinely spans three modules; an agent that forces a single label here is making the exact mistake Exercise 3 warned about |
| 3 | `state_machine` | Medium | Missing bounds checks are a real robustness gap, but the report doesn't rate this as crash-confirmed the way the diag_formatter High finding was |
| 4 | `fault_monitor` | Low | Purely a maintainability nit — unused `#include`, no behavioral impact |

Score your agent honestly:
- [ ] How many of 4 component suggestions matched?
- [ ] How many of 4 priority suggestions matched (exact or one level off is fine; two or more levels off is a real miss)?
- [ ] Did your agent handle issue #2's multi-module case by naming multiple components, or did it force a single wrong answer?

If your agent missed one, don't just accept it — go back to Exercise 3's action configuration and fix the instruction that caused the miss (usually: the keyword rule was too narrow, or the Confluence table wasn't actually being read). Re-run the same issue and confirm the fix.

---

## Checkpoint

- [ ] All four findings exist as real Jira issues with draft triage comments pending approval
- [ ] You have an honest accuracy score against the real answer key, not an assumed "it probably worked"
- [ ] If anything scored wrong, you diagnosed why and fixed the workflow, not just noted the miss

## Where This Goes Next

Exercise 5 reviews this workflow's permission scope one more time, versions it, and hands it to a technical owner — the same "done means proven, not done means it ran once" standard Module 06 set for test evidence.
