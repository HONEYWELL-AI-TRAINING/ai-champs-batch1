# Module 08 — Guided Lab: Studio-Style Workflows in Atlassian Rovo Studio

**Duration:** 2 hours | **Format:** Studio Authoring Workshop | **Audience:** Program, Product & Engineering Roles

Builds one governed, no-code AI agent in Atlassian Rovo Studio — an **Issue Triage Agent** for the same Jira project Module 07 connected an engineering agent to — then proves it works against real, still-open findings from `sample-repo`'s own code review. No code is written today; the workflow is authored visually, grounded on real data, and handed off to a technical owner at the end.

---

## Learning Objectives

By the end of this lab, you will have:

1. Explained the governance gap this module closes and where Rovo Studio sits relative to Modules 02 (Copilot) and 07 (MCP).
2. Authored a Studio workflow's four building blocks — Trigger, Actions & Connectors, Approval Flow, Human-in-the-Loop Checkpoint.
3. Grounded that workflow on real data: the same Jira project from Module 07, plus a Confluence page mirroring `sample-repo`'s actual architecture and coding-standards docs.
4. Tested the agent against real, unfiled findings from `sample-repo/docs/CODE_REVIEW_REPORT.md`, using the report's own severities as your answer key.
5. Reviewed permission scope, named an approver, versioned the workflow, and handed it off to a technical owner — without ever letting it touch code.
6. Stated, precisely, the one boundary this module never crosses: a Studio workflow can read and write Jira/Confluence; it cannot change `sample-repo`'s source, run its build, or open a PR. That's Module 07's agent, under Module 07's permission model.

---

## Before You Start

**Complete [setup.md](setup.md) first.** This lab reuses the Jira connection from Module 07 — if you haven't done that module, or don't have Rovo Studio access, setup.md tells you exactly what to arrange first.

---

## Agenda

| # | Block | Duration | Exercise File |
|---|-------|----------|----------------|
| 1 | Meet Rovo Studio, Ground on Real Data | 20 min | [01-ground-on-real-data.md](01-ground-on-real-data.md) |
| 2 | Author the Trigger and Data Sources | 20 min | [02-author-trigger-and-data-sources.md](02-author-trigger-and-data-sources.md) |
| 3 | Configure Actions, Approval Flow & Human-in-the-Loop | 30 min | [03-actions-approval-and-checkpoint.md](03-actions-approval-and-checkpoint.md) |
| 4 | Test Against Real Sample-Repo Findings | 30 min | [04-test-against-real-findings.md](04-test-against-real-findings.md) |
| 5 | Governance Review & Hand-Off | 20 min | [05-governance-review-and-handoff.md](05-governance-review-and-handoff.md) |

Exercises build one workflow end to end. The Jira project and issue you touch are the same ones Module 07 used — this module doesn't invent a new scenario, it adds a second, non-code way of working the first one's data.

---

## The Real Data Grounding This Lab

This lab is not built on a mockup. `sample-repo/docs/CODE_REVIEW_REPORT.md` (produced earlier in the programme) still has **four open findings** nobody has filed as individual Jira issues yet:

| Finding | File | Severity |
|---|---|---|
| Stale contract comment says module is not implemented | `include/fault_monitor.h:4` | Medium |
| Public API declarations missing behavior comments | `include/can_driver.h`, `include/fault_monitor.h`, `include/state_machine.h` | Medium |
| State machine event handling lacks defensive bounds checks | `src/state_machine.c:30` | Medium |
| Unused include in fault monitor implementation | `src/fault_monitor.c:3` | Low |

(The report's **High** finding — the `diag_formatter.c` NULL-`detail` gap — is the one Module 07 already turned into a Jira story and fixed by hand. Today's agent never touches that issue; it triages the four still-open ones.)

Exercise 4 files these four as new Jira issues and runs your agent against them — the report's own severities and file paths are the answer key you check your agent's suggestions against, the same evidence discipline every earlier module has used.

---

## Deliverables Checklist

Walk out of Module 08 with:

- [ ] A Confluence page grounding your workflow in `sample-repo`'s real module table and naming convention (Exercise 1)
- [ ] A published Rovo Studio agent named **Sample-Repo Issue Triage Agent**, with a defined trigger and connected data sources (Exercise 2)
- [ ] Actions that suggest a component label and priority, gated by a named approval step and a human-in-the-loop checkpoint before anything is applied to a real issue (Exercise 3)
- [ ] Four real Jira issues (from the table above) triaged by your agent, with your own accuracy check against `CODE_REVIEW_REPORT.md`'s actual severities (Exercise 4)
- [ ] A completed governance checklist and a hand-off note addressed to a technical owner, stating explicitly what the workflow can never do (Exercise 5)

---

## What's Next

**Module 09 — PR Process Automation, Quality Gates and LLM-as-Judge** (1.5 hours). Back to engineering: AI-assisted PR review and specification-compliance quality gates for `sample-repo`'s C/C++ code — the review discipline this module's triage agent fed with real, sourced findings, engineering now automates at the merge gate.

For facilitators: see [facilitator-guide.md](facilitator-guide.md) for the run sheet and the verified expected triage results for all four test issues.
