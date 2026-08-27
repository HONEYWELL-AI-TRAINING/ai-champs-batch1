# Exercise 5: Review-Cycle Debrief

**Duration:** 10 minutes | **Format:** Individual, written

## Objective

Slide 11's four signals — review time, review-cycle count, issues caught pre-human, escalation rate — turned from abstractions into your own numbers from the PR you just ran.

---

## Step 1 — Fill In Your Own Numbers (6 minutes)

```markdown
# Review-Cycle Debrief — <your PR link>

| Signal | Your Number |
|---|---|
| Time from first push to a clean gate pass | ? |
| Review rounds (pushes after the first) | ? |
| Issues the automated gate caught before any human looked | ? (should be ≥1 — the malloc violation) |
| Issues the LLM-as-Judge caught that the automated gate didn't | ? |
| Did anything require human escalation beyond the final merge decision? | ? |
```

For comparison, estimate — honestly, don't guess low to make the point look better — how long a fully manual review (a human reading the diff cold, with no automated gate and no judge) would have taken to catch the same `malloc` issue. Some reviewers would catch it immediately; some wouldn't, especially in a larger diff where it's not the only thing under review.

## Step 2 — Answer, in Your Own Words (4 minutes)

1. Slide 10 says a healthy escalation rate is never zero. If every single PR your gate has ever seen scored "Ready for human review" with High confidence, what would that suggest about your rubric — that it's excellent, or that it's not actually testing anything hard?
2. Point to the exact moment in Exercises 3–4 where a human decision could not have been delegated to either the automated gate or the LLM-as-Judge, no matter how good either one got.
3. This whole module ran against one file and roughly ten lines of real change. What would you need to add to this pipeline — not remove — before trusting it on a 500-line PR touching three modules?

---

## Checkpoint

- [ ] Your review-cycle numbers are filled in with real values from your own PR, not estimates presented as measurements
- [ ] You've honestly estimated the manual-review baseline, not assumed automation "obviously" wins
- [ ] You can answer all three debrief questions without looking anything up

## Where This Goes Next

Module 10 takes these same four signals — the ones you just measured by hand for one PR — and shows how Agent Prism tracks them automatically, across every PR, every agent, and every module this programme has built so far.
