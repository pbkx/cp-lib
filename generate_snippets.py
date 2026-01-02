import json
import sys
from pathlib import Path

snippets = {}
root = Path(__file__).resolve().parent

source_dirs = [root / "io", root / "template"]

for source_dir in source_dirs:
    if not source_dir.is_dir():
        continue
    for path in sorted(source_dir.rglob("*.cpp")):
        relative = path.relative_to(root)
        if any(part.startswith(".") for part in relative.parts):
            continue

        name = path.stem
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
