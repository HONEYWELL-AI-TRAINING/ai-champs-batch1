# Setup — Module 08 Lab

Do this before the session starts. Takes about 10 minutes if Module 07 is already done; longer if you're starting fresh.

## 1. What You Need

- [ ] Access to **Atlassian Rovo Studio** on the same Jira/Confluence site used in Module 07 (or a training tenant your facilitator has provisioned) — ask your Atlassian site admin to enable Rovo Studio and grant you agent-authoring permission if you don't see it in the product navigation
- [ ] The same Jira **project key** you used in [Module 07](../module-07/setup.md) — today's agent triages issues in that project, on purpose. If you skipped Module 07, create one small project now and note its key.
- [ ] Confluence space access in that same site, with permission to create at least one page
- [ ] `sample-repo/` open as a reference — you won't edit any code today, but you'll copy real content from `docs/ARCHITECTURE.md`, `docs/CODING_STANDARDS.md`, and `docs/CODE_REVIEW_REPORT.md`

## 2. Confirm What You're Building On

This module does **not** need a new MCP connection — Rovo Studio talks to Jira and Confluence natively, inside the same Atlassian product, no separate agent client to configure. If you're doing this module independently of Module 07, skim its [README.md](../module-07/README.md) "The Real Gap Driving This Lab" section now — today's agent triages the same category of finding Module 07 fixed one instance of by hand.

- [ ] You can open Rovo Studio's agent builder from your Jira/Confluence site
- [ ] You know your Jira project key
- [ ] You have a Confluence space to publish one grounding page into

## 3. Read the Source Documents Once

Before Exercise 1, read these three files in `sample-repo/docs/` — today's whole workflow is grounded in them, not invented:

- `docs/ARCHITECTURE.md` — the module layering table (which file owns which responsibility)
- `docs/CODING_STANDARDS.md` — the naming convention (`can_*`, `hal_*`, `diag_*`, `sensor_*`, `device_sm_*`, `fault_monitor_*`)
- `docs/CODE_REVIEW_REPORT.md` — the four still-open findings this lab's Exercise 4 files as test issues

## 4. This Module's Own Artifacts

New things land in your Atlassian site, not in `sample-repo/`:

```
Confluence space:
└── "Sample-Repo Module Reference" page      (Exercise 1)

Rovo Studio:
└── "Sample-Repo Issue Triage Agent" (v1)     (Exercises 2–3)

Jira project (same key as Module 07):
├── 4 new test issues                          (Exercise 4)
└── Triage comments from your agent on each   (Exercise 4)
```

Nothing in `sample-repo/`'s files changes today — this module's whole point is a governed layer that touches Jira and Confluence only, never code.

---

Once you can open Rovo Studio's agent builder, start with [Exercise 1](01-ground-on-real-data.md).
