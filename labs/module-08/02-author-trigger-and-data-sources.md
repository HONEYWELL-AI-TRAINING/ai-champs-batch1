# Exercise 2: Author the Trigger and Data Sources

**Duration:** 20 minutes | **Format:** Individual, hands-on, Rovo Studio

## Objective

Slide 7's anatomy, first two blocks: **Trigger** (what starts the workflow) and the data-source half of **Actions & Connectors** (what the workflow reads). Build these two pieces before any generation logic — an agent that reads the wrong data can't be fixed by a cleverer prompt.

---

## Step 1 — Create the Agent (5 minutes)

In Rovo Studio, create a new agent named:

```
Sample-Repo Issue Triage Agent
```

Give it a one-sentence description you write yourself:

```
Reads a newly created issue in <YOUR_PROJECT_KEY> and suggests a component
label and priority based on sample-repo's real module ownership and the
severities the team already uses — for human review before anything is applied.
```

Writing this description before configuring anything is deliberate — it's your own scope statement, the same discipline as Module 07 Exercise 2's "draft the story before you file it."

## Step 2 — Define the Trigger (5 minutes)

Set the trigger to **issue created** in your Jira project (the same key from Module 07). Confirm in the builder:

- [ ] The trigger fires on issue creation, not on every field update — a broader trigger (e.g., "any issue change") would fire repeatedly for the same issue as your agent itself comments on it later, which is exactly the kind of scope creep Slide 12's anti-pattern list warns against
- [ ] The trigger is scoped to your one project, not "any project in this site"

## Step 3 — Connect the Data Sources (10 minutes)

Add two data source connections:

1. **Jira** — the triggering issue's summary, description, and (once you create them in Exercise 4) any existing labels
2. **Confluence** — the "Sample-Repo Module Reference" page from Exercise 1

Confirm the connection is scoped correctly:

- [ ] The agent can read the triggering issue's fields
- [ ] The agent can read your Confluence grounding page
- [ ] The agent **cannot** read every Confluence space in the site — check the connector's scope settings; if it defaults to "all spaces I can access," narrow it to just the one space, per Slide 10's "no privilege escalation" principle: a workflow inherits its author's access, but that doesn't mean the workflow should exercise all of it just because it can

---

## Checkpoint

- [ ] Agent exists, named, with a one-sentence scope description you wrote
- [ ] Trigger fires only on new-issue-created in your one project
- [ ] Both data sources are connected and scoped as narrowly as the workflow actually needs

## Where This Goes Next

Exercise 3 turns these two data sources into an actual suggestion — a component label and priority — gated by an approval step before either ever touches the real issue.
