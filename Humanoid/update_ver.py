import re
import sys

def update_version(new_version):
	with open("data/version.txt", "w") as f:
		f.write(f"v{new_version}")


def extract_version():
	version_pattern = r"//\s*Version\s*(\d+\.\d+)"
	with open("main.cpp", "r") as f:
		content = f.read()

	match = re.search(version_pattern, content)
	if match:
		return match.group(1)
	else:
		raise ValueError("Version number not found.")


if __name__ == "__main__":
	try:
		version = extract_version()
		print(f"Extracted version {version}")
		update_version(version)
		print(f"Version number updated.")
	except ValueError as e:
		print(f"Error: {e}")

