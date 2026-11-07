# Session start

Run at the start of every new session, and again if the host or tools change. Goal: pick up state from GitHub, not from copied transcripts.

## Both roles

1. Read [WORKFLOW.md](WORKFLOW.md), [PROJECT.md](PROJECT.md), [STATE.md](STATE.md) on `master` (or the branch under review).
2. Retrieve live GitHub state — STATE.md can lag:
   ```bash
   gh issue list --repo luisfpires18/woo-3d --state open
   gh pr list --repo luisfpires18/woo-3d --state open
   gh issue view <n> --repo luisfpires18/woo-3d --comments
   gh pr view <n> --repo luisfpires18/woo-3d --comments
   ```
3. Discover which tools this session actually has (GitHub connector or CLI, code execution, image viewing, local file access). Do not infer absence from one failed route, and do not assume tools carry over from another conversation.
4. Summarize state and recommend a next step. Do not start work that the owner has not directed.

## ChatGPT (planner / reviewer)

- Planning: publish an Issue only after the owner approves the finished plan. If this session cannot write to GitHub, say so.
- Review: retrieve the Issue, PR, full diff (`gh pr diff <n>`; paginate if truncated), comments and evidence yourself. When the owner says "Claude finished", fetch the new report instead of asking for it.
- Visual review: before promising it, retrieve one actual evidence image and inspect its pixels. Metadata or a link is not inspection. For video, follow [MEDIA.md](MEDIA.md); report sampled frames as sampled.
- If a capability is missing after discovery, name it once (byte retrieval, execution, image rendering, video decoding). Do not ask the owner to attach media as the normal route, and do not propose making private assets public.

## Claude Code (implementer)

1. Entry point is [`../CLAUDE.md`](../CLAUDE.md).
2. Run `git status` and `git branch --show-current` before touching files.
3. Confirm machine and engine: `hostname`; engine path from `HKCU\SOFTWARE\Epic Games\Unreal Engine\Builds`; version from `<engine>/Engine/Build/Build.version`. Record any mismatch with [PROJECT.md](PROJECT.md).
4. Check the Issue the owner named is approved and read its comments for later owner decisions.
5. Only the named Issue is in scope. Follow WORKFLOW.md §3–6.

## Media capability check

Only needed when a task involves screenshots/video. Follow [MEDIA.md](MEDIA.md). Until a route is verified there, report it as unverified rather than claiming it works.
