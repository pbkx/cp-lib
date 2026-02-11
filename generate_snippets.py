import json
import sys
from pathlib import Path

snippets = {}
root = Path(__file__).resolve().parent

sources = [
    {"dir": root / "io", "pattern": "*.cpp", "name_prefix": "", "use_relative_name": False},
    {"dir": root / "template", "pattern": "*.cpp", "name_prefix": "", "use_relative_name": False},
    {
        "dir": root / "third_party" / "ac-library" / "atcoder",
        "pattern": "*.hpp",
        "name_prefix": "acl_",
        "use_relative_name": True,
    },
]

for source in sources:
    source_dir = source["dir"]
    if not source_dir.is_dir():
        continue

    for path in sorted(source_dir.rglob(source["pattern"])):
        relative = path.relative_to(root)
        if any(part.startswith(".") for part in relative.parts):
            continue

        if source["use_relative_name"]:
            local_name = "_".join(path.relative_to(source_dir).with_suffix("").parts).replace("-", "_")
            name = f'{source["name_prefix"]}{local_name}'
        else:
            name = f'{source["name_prefix"]}{path.stem}'

        if name in snippets:
            print(f"error: duplicate snippet {name}", file=sys.stderr)
            raise SystemExit(1)

        snippet = {
            "prefix": name,
            "body": path.read_text(encoding="utf-8").splitlines(),
            "description": name,
        }
        snippets[name] = snippet

        print(f"generated snippet {name}", file=sys.stderr)

(root / "cpp.json").write_text(json.dumps(snippets, indent=2), encoding="utf-8")

print("done", file=sys.stderr)
