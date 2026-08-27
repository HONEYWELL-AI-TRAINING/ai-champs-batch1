---
name: sdd-spec-from-requirements
description: "Converts a raw feature request or ticket into a formal, traceable Spec-Driven Development (SDD) specification document with requirements, constraints, interfaces, and acceptance criteria. Use when: turning a feature request into implementable spec; formalizing requirements before code; creating testable acceptance criteria; ensuring requirements traceability. Produces a single spec.md file ready for planning and implementation."
tags: ["sdd", "specification", "requirements", "embedded-engineering"]
---

# SDD Spec from Requirements

Convert a raw feature request into a formal, traceable specification following Spec-Driven Development principles.

## When to Use This Skill

- You have a feature request, ticket, or informal requirement statement
- You need a structured specification before implementation begins
- You want to establish testable acceptance criteria and traceability
- You're working in an embedded systems or regulated environment where spec clarity matters
- You need to bridge the gap between "what was requested" and "what should be built"

## The Workflow

This skill implements the **Specify** stage of the SDD workflow: **Spec → Plan → Tasks → Implement**

```
Raw Requirement/Ticket
         ↓
    [This Skill]
         ↓
   spec.md with:
   • Functional Requirements
   • Non-Functional Requirements
   • Constraints
   • Interfaces & Contracts
   • Acceptance Criteria (testable, bounded, traceable)
```

## Quick Start

1. **Copy your feature request** (ticket, user story, or email) into the conversation
2. **Ask the skill to extract and structure it** — it will walk you through the four decisions:
   - What exactly should the new feature **do**? (requirements)
   - What are the **limits and rules** it must follow? (constraints)
   - What **existing APIs or data structures** does it interact with? (interfaces)
   - How will you **know it's done?** (acceptance criteria)
3. **Review the draft spec against the quality checklist** — testable, bounded, interface-accurate, traceable
4. **Save the result as `spec.md`** in your project or specs folder

## The Four Sections of a Good Spec

Every spec.md contains exactly four sections:

### 1. **Requirements** (Functional & Non-Functional)

What the feature should **do**.

- **Functional Requirements (REQ-1, REQ-2, ...):** Observable behaviors and outputs.
  - *Example:* "REQ-1: Function returns one diagnostic string per polling tick in `[0xCCCC] detail` format."
  - *Anti-example:* "REQ-1: Works correctly" (not testable)

- **Non-Functional Requirements (REQ-N):** Performance, resource usage, quality attributes.
  - *Example:* "REQ-3: Polling loop completes within 2ms at 100Hz."
  - *Anti-example:* "REQ-3: Should be fast" (not bounded)

**Quality check:** Could a reviewer understand what "done" means without asking you a clarifying question?

### 2. **Constraints** (CON-1, CON-2, ...)

What the feature **cannot violate** or must respect.

- Memory limits, no dynamic allocation
- Platform / toolchain restrictions
- Timing / resource budgets
- Layering rules or architectural boundaries
- Regulatory / safety requirements

*Example:* "CON-1: Must use caller-owned buffer (no `malloc`/`free`)."  
*Anti-example:* "CON-1: Should be efficient" (not specific)

**Quality check:** Are all constraints stated as concrete rules, not adjectives?

### 3. **Interfaces & Contracts** (Interfaces, Reuses, Must NOT)

What your feature **touches and reuses**.

- Function signature(s) — name, parameters, return type
- Which existing functions or modules it calls (reuses)
- Which boundary layers it must **not** cross (isolation rules)
- Data structures or APIs it depends on
- Existing behavior to preserve

*Example:*
```
Interface: sensor_fault_diagnostic_format(const sensor_snapshot_t *snap, 
                                          char *buf, size_t buf_len) → size_t
Reuses: diag_format_message(), diag_format_sensor_fault()
Must NOT: call hal_adc_*, hal_gpio_* (diagnostics don't touch HAL)
```

**Quality check:** Does this match the codebase's actual conventions and layering rules?

### 4. **Acceptance Criteria** (AC-1, AC-2, ...)

How you'll **prove the feature works**.

Each AC is a **testable, observable pass/fail condition** — not a goal, but a checkpoint.

- Every requirement should have at least one AC that proves it
- ACs are specific enough to write a unit test from
- ACs cover edge cases (empty input, buffer too small, no data, etc.)
- ACs state exact thresholds, not adjectives

*Example (strong):*
- "AC-1: On sensor timeout (>50ms), retry once, then log `SENSOR_TIMEOUT` and return last-known value."
- "AC-2: When zero faults are active, return 0 and leave buf untouched."

*Anti-example (weak):*
- "AC-1: Handle timeouts gracefully" (what's "gracefully"?)
- "AC-2: Works correctly" (prove it how?)

**Quality check:** Could someone write a unit test from AC-1 through AC-N without asking you anything?

## The Five-Point Quality Bar

Before you finalize your spec, check it against these five criteria:

1. **Testable** — Every requirement has an AC that resolves to pass/fail. "Works correctly" never appears.
2. **Bounded** — Constraints are explicit: numbers, not adjectives. "50ms timeout," not "respond quickly."
3. **Interface-Accurate** — Function signatures and contracts match this codebase's real conventions, not an ideal version.
4. **Traceable** — Every REQ, CON, and AC has an ID (REQ-1, CON-2, AC-3, etc.).
5. **Reviewed** — A second engineer has read the spec and tried to poke a hole in at least one AC.

**Quick test:** If you can't write a unit test checklist from AC-1 through AC-N alone, the spec isn't done yet.

## Example Walkthrough

**Raw Ticket:**
> "Add a function to surface sensor faults as diagnostic messages. Field support wants to see fault details in the same format already used elsewhere. When multiple faults are active, show the most important one."

**Decisions the spec must make:**

1. **What exactly is "the same format already used elsewhere"?**  
   → Search the codebase for existing diag_format_message calls; match that signature.

2. **Which fault is "most important"?**  
   → Business decision: temperature fault (safety) > pressure fault (data) > door+motion (operational). Write this as the priority rule.

3. **What happens when the caller's buffer is too small?**  
   → Follow the codebase's convention: return how many bytes *would have been* written; let caller resize and retry.

4. **What happens when zero faults are active?**  
   → Return 0, buffer untouched. (Makes it safe to call every tick unconditionally.)

**Result:** A 4-section spec.md with 2–3 reqs, 3–4 constraints, 1–2 interfaces, and 4–6 acceptance criteria, each one answering one of these decisions.

---

## How to Invoke This Skill

### In the Chat

Type `/sdd-spec-from-requirements` and provide:

```
Feature Request:
[Paste your ticket, user story, or requirement here]

Context (optional):
- Existing APIs or modules this should interact with
- Key architectural rules or constraints from your codebase
- Any ambiguous terms you want clarified
```

### What You'll Get

1. **A draft spec.md** with all four sections filled in
2. **Explanation of the four key decisions** embedded in the spec
3. **A quality checklist** — you rate the spec against the five-point bar
4. **Suggestions for refinement** if any AC fails the checklist

### What to Do Next

- **Save the spec.md** to your project (typically in `specs/NNN-feature-name/spec.md`)
- **Use this spec in the next stage:** `/sdd-plan-from-spec` (generates a plan and tasks)
- **Reference AC IDs** during code review and testing — every PR should trace back to a requirement

---

## Common Pitfalls to Avoid

| Pitfall | Why It Matters | Fix |
|---------|---|---|
| Specs that list features instead of requirements | "Add X, Y, Z" is a shopping list, not a spec | Focus on *why*: what problem is each feature solving? What's the observable output? |
| Acceptance criteria that can't be tested | "Must be reliable" — how do you measure it? | Rewrite as a specific, checkable condition: "Return last-known value on timeout, with error code `ERR_SENSOR_TIMEOUT`." |
| Constraints stated as adjectives | "Should be efficient" — too vague | Be specific: "Complete in <2ms," "Use <256 bytes RAM," "No dynamic allocation." |
| Forgetting to check interfaces against real code | "Signature should be `int foo(float x)`" — but the codebase uses `size_t` return values | Read your actual codebase's patterns first, then spec to match them |
| Skipping the review step | Lone author misses gaps, creates ambiguity later | Trade specs with a colleague; ask: "Can you write a unit test for AC-2 without asking me?" |

---

## Related Skills

- **`sdd-plan-from-spec`** — Takes your spec.md and generates a plan.md + tasks.md
- **`sdd-implement-from-tasks`** — Executes tasks and checks each diff against the spec
- **`agent-customization`** — Learn to create and refine skills like this one

## References & Resources

- **SDD Fundamentals** — See Module 04 in the training material
- **Spec Anatomy** — Review the five-point quality bar and acceptance-criteria examples above
- **GitHub Spec Kit** — Official tool for SDD workflows (this skill uses similar patterns with plain markdown)
- **OpenSpec** — Alternative spec-driven development format

---

## Tips for Success

✓ **Start with the raw requirement.** Don't overthink it — ambiguity is the *point* of this step. Resolve it in the spec.

✓ **Make the four decisions explicit.** Your spec.md will have a paragraph addressing each one. Call them out.

✓ **Write ACs like test cases.** If it reads like a unit test (condition → expected outcome), it's probably right.

✓ **Trace everything.** Every task will reference a REQ/CON/AC by ID. If a REQ doesn't appear in any AC, either delete the REQ or add the AC.

✓ **Ask for review.** Trade specs with a teammate for five minutes. Their questions become gaps to close before coding starts.

---

**Let's build better specs, together.**
