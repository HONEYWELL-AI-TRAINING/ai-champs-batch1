# Exercise 5: Governance Review & Hand-Off

**Duration:** 20 minutes | **Format:** Individual, hands-on, Rovo Studio + Confluence

## Objective

Slide 10's four pillars govern every workflow this module produces: no privilege escalation, named approvers, versioned and owned, auditable by design. Slide 11's rule: *"a workflow that never gets reviewed by a technical owner isn't done, no matter how well it ran in testing."* This exercise is that review and that hand-off.

---

## Step 1 — Run the Governance Checklist (8 minutes)

Go through Slide 10's four pillars against your actual workflow, not from memory:

- [ ] **No Privilege Escalation** — open the agent's connector settings again; confirm it can see only the one Jira project and the one Confluence space it needs, not "everything I can access"
- [ ] **Named Approvers** — confirm the approval step from Exercise 3 names a real person or role, visible in the configuration, not blank or "TBD"
- [ ] **Versioned & Owned** — set a version number (e.g., `v1.0`) and a named owner on the published agent. If Rovo Studio's version field is separate from a description field, use both — version in the field meant for it, owner named explicitly in the description
- [ ] **Auditable by Design** — confirm you can pull up a log or history showing every trigger fire, every draft, and every approval decision from Exercise 4's four test issues. If you can't find this view, ask your facilitator — an unauditable workflow fails this checklist regardless of how well it performed

## Step 2 — Confirm the Boundary One Last Time (5 minutes)

Answer Slide 8's table for your own workflow, specifically:

1. What does your workflow touch? (Should be: Jira issues, Confluence pages — nothing else.)
2. Can it change code? (Should be: never — verify by checking there is no action anywhere in your configuration that has file-system or repository write capability. If one exists, remove it — it shouldn't have been possible to add in the first place, but confirm.)
3. When your workflow's suggestion is good enough to actually fix something in `sample-repo` (like finding #3's missing bounds check), what happens next? Name the specific module and exercise from Module 07 that would pick it up.

## Step 3 — Write the Hand-Off Note (7 minutes)

Publish a short Confluence page (or add a section to your "Sample-Repo Module Reference" page from Exercise 1) addressed to a technical owner:

```markdown
# Hand-Off: Sample-Repo Issue Triage Agent v1.0

**Owner:** <your name / role>
**Scope:** Reads new issues in <PROJECT_KEY>, drafts a component label and
priority suggestion grounded in sample-repo's real module table. Requires
named-approver sign-off before anything is applied to a live issue.

**What this workflow will never do:** change source code, run a build or
test, or open a pull request against sample-repo. Any fix a triaged issue
implies is picked up separately, by an engineer, through Module 07's
MCP-enabled workflow — this workflow only prepares the ticket, it never
executes the fix.

**Test evidence:** 4/4 real findings from docs/CODE_REVIEW_REPORT.md
triaged; accuracy score and any fixes made are in Exercise 4's notes.

**Known limitations:** <anything you found during testing that a future
maintainer should know — e.g., how it handled the multi-module finding>
```

---

## Checkpoint

- [ ] All four governance pillars checked against your real configuration, not assumed
- [ ] You can state your workflow's boundary in one sentence, matching Slide 8's table exactly
- [ ] A hand-off page exists, naming a real owner, a real scope, and the explicit "never does this" boundary

## Where This Goes Next

You've now built two working pieces of the same engineering loop: Module 07's engineering agent implements a real fix end to end; Module 08's Studio agent prepares the tickets that feed it. Module 09 automates the last mile between them — the PR quality gate an engineer's fix has to pass before either kind of agent's work actually merges.
