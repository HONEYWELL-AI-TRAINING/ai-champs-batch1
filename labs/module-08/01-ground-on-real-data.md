# Exercise 1: Meet Rovo Studio, Ground on Real Data

**Duration:** 20 minutes | **Format:** Individual, hands-on, Confluence + Rovo Studio

## Objective

Slide 6's rule: *"a workflow is only as trustworthy as the data it's grounded on — never a disconnected guess."* Before building anything, put real `sample-repo` content somewhere your Studio workflow can actually read it.

---

## Step 1 — Locate Rovo Studio (5 minutes)

Open your Jira/Confluence site and find Rovo Studio's agent builder (usually under the app switcher or a "Studio"/"Automation" entry — exact placement varies by site configuration). Confirm you can see an option to create a new agent. If you can't, stop here and get access from your site admin before continuing — don't work around this with a personal trial site, since today's grounding step depends on this being the *same* site as Module 07's Jira project.

## Step 2 — Publish the Grounding Page (10 minutes)

Create a Confluence page titled **"Sample-Repo Module Reference"** in your space. Populate it with real content — not a paraphrase — from two files:

From `sample-repo/docs/ARCHITECTURE.md`, copy the **Modules** table (module name → responsibility).

From `sample-repo/docs/CODING_STANDARDS.md`, copy the **Naming** section's module-prefix list:

```
can_*            → CAN driver
hal_*            → Hardware abstraction
diag_*           → Diagnostics
sensor_*         → Sensor polling
device_sm_*      → State machine
fault_monitor_*  → Fault integration
```

This page is today's single source of truth for "which module does a Jira issue belong to" — your agent will read it, not guess from the issue title alone.

## Step 3 — Confirm the Studio/Engineering Boundary (5 minutes)

Before authoring anything, answer these from Slide 8's comparison table, in your own words:

1. What can today's Studio workflow touch? What can it never touch?
2. Who authors a Studio workflow versus who authors an MCP-enabled engineering workflow (Module 07)?
3. If your finished agent tried to open a pull request against `sample-repo`, would that be a bug in your workflow, or a fundamental scope violation? Why?

---

## Checkpoint

- [ ] You can open Rovo Studio's agent builder on the same site as Module 07's Jira project
- [ ] A Confluence page exists with the real module table and naming convention from `sample-repo/docs/`
- [ ] You can state, without looking it up, what a Studio workflow can never do

## Where This Goes Next

Exercise 2 builds your agent's trigger and wires it to this exact Confluence page plus your Jira project — the two data sources everything downstream depends on.
