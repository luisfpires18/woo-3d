# Shared interactive workflow (Weapons of Order)

Authoritative lifecycle for this repository. Project facts live in [PROJECT.md](PROJECT.md); live pointers in [STATE.md](STATE.md); decisions in [DECISIONS.md](DECISIONS.md). Adapted from `luisfpires18/woo-rts` `.ai/WORKFLOW.md` (branch `main`, read 7 Nov 2026); RTS/Unity specifics were not carried over.

## Roles

| Role | Who | Does | Does not |
|---|---|---|---|
| Planner | ChatGPT | Refines requirements and acceptance criteria; publishes owner-approved Issues | Authorize development by itself |
| Implementer | Claude Code | Investigates, implements, tests, commits, pushes the feature branch, opens a draft PR, publishes evidence | Merge, deploy, start other Issues, change scope |
| Reviewer | A separate ChatGPT session | Retrieves Issue, PR, diffs, reports and evidence; returns one verdict | Defer the verdict to the owner; add `CLAUDE NEXT ACTION` |
| Owner | luisfpires18 | Approves plans and scope; final acceptance; merges; deployments; starting tasks | — |

No autonomous Claude runs, unrequested extra agents, or automatic correction loops. Optional Codex use is limited to owner-directed read-only review commands.

## LorEvolve is optional

LorEvolve is a bonus lore cross-check. Never treat it as required, test whether it is connected, or block on it. Consult it only when already available and relevant; otherwise continue from the Issue and repository documents and record material uncertainty.

## 1. Start of session

Follow [SESSION_START.md](SESSION_START.md). Summarize current state and recommend a next step. A queued next task is not permission to start it.

## 2. Plan

The planner discusses requirements, scope, acceptance criteria, tests and evidence scenarios. Explicit owner approval of the finished plan authorizes publishing it as a GitHub Issue. A draft or an assistant recommendation is not approval.

Issue types:
- **Implementation Issue** — authorizes in-scope implementation once the owner starts it.
- **Plan-only / audit-only Issue** — authorizes investigation and a report only. No code implementation until the owner approves the plan and directs implementation.

## 3. Issue-start authorization

A clear owner instruction to start a named, approved Issue authorizes normal in-scope work without repeated approval for routine steps:
- investigation and implementation inside the Issue's scope;
- builds and tests;
- scoped commits on a feature branch;
- pushing that feature branch;
- opening and updating a **draft** PR targeting `master`;
- publishing results, evidence and blocker reports to the Issue/PR.

This authorization persists across Claude and ChatGPT conversations and is not revoked by a review verdict. It does **not** cover: another Issue, changed scope, merging, deployment, repository settings (visibility, default branch), or destructive cleanup (deleting branches, releases, evidence, history rewrites). Those need explicit owner direction.

## 4. Implement

1. Check the current branch and `git status` before touching files. Preserve unrelated work: do not stash, reset, delete, or commit it.
2. Create one feature branch per Issue from `master` (suggested name `woo-<id>-<slug>`). Never change the branch strategy; there is no `main` or `dev` here.
3. Claude may use C++, Unreal Python/editor scripting, commandlets and other supported automation. **Verify each technique works in this UE 5.8.4 installation before relying on it.** Do not claim every Blueprint or map change can be automated; when a step needs the editor GUI, say so and describe what the owner must do.
4. Follow the asset rules in [PROJECT.md](PROJECT.md#assets-and-licensing).

## 5. Verify — five separate levels

Report each level separately. Passing one never proves another.

| Level | Example | Proves |
|---|---|---|
| Compilation | UBT build of the editor target | Code compiles and links |
| Automated tests | `UnrealEditor-Cmd ... -ExecCmds="Automation RunTests ..."` | Tested logic only |
| Runtime check | PIE or `-game` run with logged assertions | Behaviour in a running instance |
| Visual inspection | Screenshots/video per [MEDIA.md](MEDIA.md) | What was seen in the captured scenario |
| Packaged performance | Packaged Windows build, measured FPS/frametime | Performance on the stated hardware only |

Record exact commands, exit codes, test counts, engine version, machine and tested commit. Anything not run is reported as **not run**, with the reason.

## 6. Evidence and handoff

Before stopping, Claude publishes one of these to the active PR (or the Issue if no PR exists):
- implementation report: summary, changed files, test commands and results per level, known limitations, evidence index;
- investigation report; or
- **blocker report**: approved scope, findings, references, options, work done/not done, and the exact owner decision needed.

Do not ask whether to post it. Do not leave the only copy in a chat transcript. If publication fails, say so, keep the report locally, and check for duplicates before retrying.

Evidence rules are in [MEDIA.md](MEDIA.md) and storage in [STORAGE.md](STORAGE.md). Distinguish the tested commit from later documentation-only commits. Repeated manual screenshot transfers by the owner are not the default route.

Then update [STATE.md](STATE.md) pointers in the same branch, return the links, and wait.

## 7. Independent review

The owner asks a separate ChatGPT session to review. The reviewer retrieves the Issue, PR, actual diff, reports and evidence, and states what it actually inspected (code, reported tests, personally executed checks, images, sampled video). It never claims an inspection it did not perform.

The reviewer returns a short evidence-based explanation, then exactly one self-contained block:

```text
REVIEW STATUS: <ACCEPTED | NEED CHANGES | DECLINED>   (choose exactly one)
SCOPE: <Issue/PR and approved scope reviewed>
FINDINGS: <specific evidence; what was and was not verified>
REQUIRED CHANGES: <enumerated in-scope corrections, or "None">
```

No `CLAUDE NEXT ACTION` field in the block.

- **ACCEPTED** — meets approved scope. A reviewer verdict only; the owner decides final acceptance.
- **NEED CHANGES** — the enumerated, in-scope corrections are authorized for Claude to make, verify, commit, push and report without new owner approval. Corrections that would change scope, merge or deploy are excluded and named as owner decisions.
- **DECLINED** — unacceptable as presented; states why and what must happen next. In-scope corrections are not disguised as owner decisions.

After the block, the reviewer separately recommends exact next actions using live references, e.g. *"If you accept, tell Claude: merge PR #X, close Issue #Y, start #Z."* This is guidance for the owner, not execution.

When the owner says "Claude finished" (or similar), the reviewer retrieves the active Issue/PR and its new comments itself instead of asking for pasted text. If no report is published, it says so.

## 8. Close-out

After the owner accepts the result and directs the merge: merge as directed, confirm it, close the Issue, tick it in its tracker Issue, update STATE.md pointers, and list evidence cleanup candidates per [STORAGE.md](STORAGE.md) without deleting anything. A passing review is not the owner's acceptance.

## Record locations

| Record | Location |
|---|---|
| Roadmap | Roadmap Issue [#2](https://github.com/luisfpires18/woo-3d/issues/2) |
| Milestone trackers (M0–M6) | Tracker Issues #3–#9 (e.g. M0: [#3](https://github.com/luisfpires18/woo-3d/issues/3)) |
| Phase plans | Phase Issues #10–#33 |
| Task plans | Task Issues (e.g. M0 tasks #34–#41) |
| Reports, evidence index, review verdicts | PRs (or the Issue when no PR exists) |
| Current pointers and blockers | [STATE.md](STATE.md) |
| Lasting decisions | [DECISIONS.md](DECISIONS.md) |
| Media capability record | [MEDIA.md](MEDIA.md) |
| Evidence storage and retention | [STORAGE.md](STORAGE.md) |

Milestones, phases and tasks are ordinary Issues with Markdown checklists linking each other. They are **not** native GitHub Milestones or native sub-issues; do not assume those features are in use. At close-out, tick the completed task in its tracker Issue.

Local assistant memory is a convenience, never the authoritative record.
