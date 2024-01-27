import subprocess
import requests
import json

def get_last_commit(repo_owner, repo_name, auth_token=None):
    url = f'https://api.github.com/repos/{repo_owner}/{repo_name}/commits'

    headers = {}
    if auth_token:
        headers['Authorization'] = f'Token {auth_token}'

    response = requests.get(url, headers=headers)

    if response.status_code == 200:
        commits = response.json()
        if commits:
            return commits[0]['sha']  # Return the SHA of the latest commit
    else:
        print(f"Error: {response.status_code}, {response.text}")

    return None

def read_last_commit_from_file(filename):
    try:
        with open(filename, 'r') as file:
            return file.read().strip()
    except FileNotFoundError:
        return None

def write_last_commit_to_file(filename, commit_sha):
    with open(filename, 'w') as file:
        file.write(commit_sha)

def pull_latest_commit():
    try:
        subprocess.run(['git', 'pull'], check=True)
        print("Successfully pulled the latest changes.")
    except subprocess.CalledProcessError as e:
        print(f"Error pulling changes: {e}")

if __name__ == "__main__":
    repo_owner = "Alexei1R"
    repo_name = "ATOM_Headless"
    auth_token = "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"

    last_commit_file = "last_commit.txt"

    last_fetched_commit = get_last_commit(repo_owner, repo_name, auth_token)
    last_saved_commit = read_last_commit_from_file(last_commit_file)

    if last_fetched_commit and last_saved_commit != last_fetched_commit:
        print(f"New commit found: {last_fetched_commit}")

        # Pull the latest changes
        pull_latest_commit()

        # Update the last fetched commit in the file
        write_last_commit_to_file(last_commit_file, last_fetched_commit)
    else:
        print("No new commits.")
