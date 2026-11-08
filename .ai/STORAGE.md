# Evidence storage and cleanup

## Where things live

| Content | Location |
|---|---|
| Source, workflow docs, small durable references | Git (`master` via PRs) |
| Unreal binary assets required by the project (`.uasset`, `.umap`, source art) | Git LFS (see [`../.gitattributes`](../.gitattributes)) |
| Screenshot/video evidence | **Draft GitHub release assets** (confirmed; C-12 in [DECISIONS.md](DECISIONS.md)) |
| Evidence manifest | Release asset `evidence-manifest.json` plus a copy of its table in the PR report |

Evidence media is **not** committed to Git or Git LFS by default. LFS objects stay in history and count against LFS storage/bandwidth; release assets do neither.

### Draft releases in a public repository

Draft releases are visible only to users with write access, even in a public repository, so draft evidence is not public by default. Publishing a draft makes it public. Never publish a release or change visibility to work around a reviewer's access limit. Tested in this public repository on 8 Nov 2026 with draft release ID 438382190 (#41): without authentication, the release by ID, the asset by ID, the release by tag, both `releases/download` asset URLs, both release pages returned 404, the anonymous release list omitted the draft, and no Git tag exists on the remote. Authenticated `gh release download` as the owner retrieved every file with matching hashes. This shows a reviewer without write access cannot retrieve drafts.

Limits (GitHub docs): up to 1,000 assets per release, each under 2 GiB. https://docs.github.com/en/repositories/releasing-projects-on-github/about-releases#storage-and-bandwidth-quotas

## Naming

- Tag: `evidence-<issue>-<slug>-<yyyymmdd>`, e.g. `evidence-issue12-jungle-walk-20261120`.
- Draft releases need a tag name for CLI retrieval; record both the tag and the release API ID in the PR. Draft pages use an `untagged-...` URL (verified #41), and the Git tag does not exist until publication.

## Publish and retrieve

```bash
gh release create <tag> --repo luisfpires18/woo-3d --draft --target <tested-sha> --title "<tag>" --notes "<scenario summary>" <files...>
gh release download <tag> --repo luisfpires18/woo-3d --dir <task-dir>
sha256sum <task-dir>/*
```

Verify downloaded hashes against the manifest before claiming retrieval works.

## Manifest (`evidence-manifest.json`)

One entry per file: `name`, `sha256`, `bytes`, `kind` (original/derived), `derived_from`, `transformation`, `scenario`, `tested_commit`, `machine`, `engine`, `resolution`, `fps`, `captured_at`, `duration_s` (video), `limitations`. Fields come from [MEDIA.md](MEDIA.md).

## Retention (proposed — no automatic deletion)

1. Keep evidence while its Issue/PR is open or under review.
2. After owner acceptance and confirmed merge, classify each item: keep (approved references, milestone baselines, regression recordings) or temporary (drafts, debug clips).
3. Suggested grace period for temporary items: 30 days after merge. Not scheduled; nothing deletes automatically.
4. Before deletion, list exact release/asset IDs, sizes and related tasks; obtain owner approval for that exact batch.
5. Keep the manifest and a deletion tombstone (date, what was removed) in the PR record. Mark removed links as expired.

Merging a PR or deleting a branch does not remove release assets. Removing a file from the Git tree does not remove its history; never rewrite history as routine cleanup.
