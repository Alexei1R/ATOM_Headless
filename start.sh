#!/bin/bash

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd "$script_dir" || exit

repo_owner="Alexei1R"
repo_name="ATOM_Headless"
auth_token="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"
last_commit_file="last_commit.txt"

get_last_commit() {
    local response
    response=$(curl -s -H "Authorization: Token $auth_token" "https://api.github.com/repos/$repo_owner/$repo_name/commits")

    if [ $? -eq 0 ]; then
        # Print the response for debugging
        # echo "GitHub API Response: $response"

        # Check if the response is a JSON array
        if jq -e '.[].sha' <<< "$response" >/dev/null; then
            # Extract SHA of the latest commit from the array
            echo "$response" | jq -r '.[0].sha'
        else
            # Handle other cases or errors
            echo "Error: Invalid GitHub API response."
            exit 1
        fi
    else
        # Handle curl errors
        echo "Error: Unable to fetch GitHub API response."
        exit 1
    fi
}

read_last_commit_from_file() {
    local filename="$1"
    [[ -f "$filename" ]] && cat "$filename"
}

write_last_commit_to_file() {
    local filename="$1"
    local commit_sha="$2"
    echo "$commit_sha" > "$filename"
}

pull_latest_commit() {
    git pull
    echo "Successfully pulled the latest changes."
    notify-send "ATOM" "Successfully pulled the latest changes."
}

build_and_run() {
    if [ ! -d "build" ]; then
        mkdir build
    fi
    cd build || exit
    cmake ..
    make -j"$(nproc)"
    ./app/executable_headless
    cd ..
}

last_fetched_commit=$(get_last_commit)
last_saved_commit=$(read_last_commit_from_file "$last_commit_file")

if [ -n "$last_fetched_commit" ] && [ "$last_saved_commit" != "$last_fetched_commit" ]; then
    echo "New commit found: $last_fetched_commit"
    notify-send "ATOM" "New commit found: $last_fetched_commit"

    pull_latest_commit
    write_last_commit_to_file "$last_commit_file" "$last_fetched_commit"
    
    notify-send "ATOM" "Begin build."
    build_and_run
else
    echo "No new commits."
    notify-send "ATOM" "Begin build."
    build_and_run
fi
