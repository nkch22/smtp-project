import subprocess
import os

# Set the core.hooksPath configuration for Git
subprocess.run(["git", "config", "core.hooksPath", ".githooks"], check=True)

# Make the pre-commit hook executable
pre_commit_hook_path = ".githooks/pre-commit"
if os.path.exists(pre_commit_hook_path):
    os.chmod(pre_commit_hook_path, 0o755)  # chmod +x
else:
    print(f"Error: The file {pre_commit_hook_path} does not exist.")
    exit(1)