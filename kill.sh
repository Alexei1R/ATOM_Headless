#!/bin/bash

# Check if any process with the name "executable_headless" is running
if pgrep -f "executable_headless" > /dev/null
then
    # Kill all processes with the name "executable_headless"
    pkill -f "executable_headless"
    echo "Processes with the name 'executable_headless' have been killed."

    # Send a notification
    notify-send "Processes Killed" "All processes with the name 'executable_headless' have been terminated."
else
    echo "No processes with the name 'executable_headless' are currently running."

    # Send a notification
    notify-send "No Processes Found" "No processes with the name 'executable_headless' are currently running."
fi
