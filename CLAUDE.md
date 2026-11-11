# Project entry point

Before acting, read the shared workflow from GitHub:

```bash
gh api repos/luisfpires18/ai-workflow/contents/WORKFLOW.md -H "Accept: application/vnd.github.raw"
gh api repos/luisfpires18/ai-workflow/contents/REVIEW.md -H "Accept: application/vnd.github.raw"
gh api repos/luisfpires18/ai-workflow/contents/OWNER.md -H "Accept: application/vnd.github.raw"
```

Also read each module listed in `.ai/PROJECT.md` (`modules/<name>.md` in luisfpires18/ai-workflow, fetched the same way). Then read `.ai/PROJECT.md` and `.ai/STATE.md`, and retrieve the active Issue/PR.

Rules in this repository override the shared workflow where they differ. If the shared workflow cannot be fetched, say so and stop; do not guess its rules.

## Skills and plugins
Use only the skills, plugins and MCP servers listed under "Skills" in `.ai/PROJECT.md`. Do not invoke others, even if they are installed and look related. If a task needs one that is not listed, name it and ask the owner. The full inventory, with safety notes, is `SKILLS.md` in luisfpires18/ai-workflow; read it only when choosing or reviewing skills.

## Repository rules
- Verify any Unreal automation technique (C++, editor Python, commandlets, `RunUAT`) on this UE 5.8.4 installation before relying on it.
- Report the five verification levels separately (`.ai/PROJECT.md`, "Verification levels"); anything not run is reported as **not run**, with the reason.
- The repository is public: committing a third-party asset publishes it. Check its licence allows redistribution in source form first. Bulk asset imports need their own Issue. Account-gated Fab acquisitions are the owner's.
- Engine paths are machine-specific (LP-VICTUS and UFO-LP); re-check them on each machine, never copy them blindly.
- woo-rts shares only the workflow; its milestones, tests, tools, paths and evidence do not apply here.
