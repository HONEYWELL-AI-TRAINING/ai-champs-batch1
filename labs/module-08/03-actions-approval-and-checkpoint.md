# Exercise 3: Configure Actions, Approval Flow & Human-in-the-Loop Checkpoint

**Duration:** 30 minutes | **Format:** Individual, hands-on, Rovo Studio

## Objective

Slide 7's remaining two blocks: **Approval Flow** (who signs off, named not assumed) and **Human-in-the-Loop Checkpoint** (where a person reviews before anything reaches its destination). Slide 7's rule is explicit: *"a trigger with no approval flow is exactly the kind of ungoverned automation this module exists to prevent."* You build the action first, but it does not run unreviewed today or ever.

---

## Step 1 — Configure the Suggestion Actions (12 minutes)

Add an action that, using the two data sources from Exercise 2, does the following:

1. Reads the new issue's summary and description.
2. Matches keywords against the Confluence module table to suggest a **component label** — one of `can_driver`, `hal_gpio`, `hal_adc`, `sensor_poll`, `state_machine`, `diag_formatter`, `fault_monitor`. If the issue plausibly touches more than one module, the agent should say so explicitly rather than picking one arbitrarily.
3. Suggests a **priority** (Critical / High / Medium / Low) using this explicit rule set — write it into the agent's instructions verbatim, don't leave it to the model's judgment alone:
   - Mentions of crash, segfault, undefined behavior, memory corruption, or a security-relevant defect → **High** or above
   - Mentions of a functional gap, missing check, or incorrect behavior with no crash risk → **Medium**
   - Mentions of style, unused code, comments, or documentation → **Low**
4. Drafts a comment (not yet posted) stating the suggested label, priority, and a one-sentence rationale citing the specific Confluence table row it matched.

Notice what this action does **not** do: it doesn't apply the label, doesn't change the priority field, and doesn't post the comment yet. That's Step 2.

## Step 2 — Add the Approval Flow (10 minutes)

Configure an approval step before the draft comment/label/priority is applied to the real issue. Name a real approver — yourself, for today's lab, or a specific "Embedded Engineering Lead" role if your site supports role-based approval. Slide 10 is explicit: *"every approval step names a real person or role — never 'someone will check this eventually.'"*

Confirm:
- [ ] The workflow pauses after drafting and before applying anything
- [ ] The approver is a named person/role, visible in the workflow configuration, not left blank

## Step 3 — Add the Human-in-the-Loop Checkpoint (8 minutes)

This is distinct from the approval flow: the checkpoint is *where* and *how* the reviewer actually sees the draft before deciding. Configure the workflow to surface the draft comment, suggested label, and suggested priority somewhere a human will actually look — a Jira comment marked as pending, a notification, or a Rovo Studio review queue, depending on what your site offers.

Confirm before moving on:
- [ ] You can see exactly what the agent would do, in full, before approving it — not a summary of the action, the actual text/label/priority
- [ ] Rejecting the suggestion doesn't silently retry or escalate — it just stops, leaving the issue exactly as it was

---

## Checkpoint

- [ ] The agent drafts a component label, priority, and rationale — but applies none of it automatically
- [ ] A named approver must act before anything reaches the real issue
- [ ] You've personally seen what the pending-review view looks like, not just configured it

## Where This Goes Next

Exercise 4 finally gives this workflow something real to triage — the four still-open findings from `sample-repo`'s own code review report — and checks its suggestions against that report's actual, published severities.
