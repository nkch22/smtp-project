import subprocess

subprocess.run(["git", "config", "core.hooksPath", ".githooks"], check=True)
