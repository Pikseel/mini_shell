#!/bin/bash

echo "Testing heredoc Ctrl+C behavior..."

# Start minishell in background and send commands through pipe
(
    sleep 0.5
    echo "cat << END"
    sleep 0.5
    echo "line 1"
    sleep 0.5
    # Send Ctrl+C (SIGINT) to the heredoc child process
    pkill -SIGINT -f "minishell"
    sleep 1
    echo "echo 'after ctrl-c'"
    sleep 0.5
    echo "exit"
) | timeout 10s ./minishell 2>&1 | tee test_output.txt

echo ""
echo "=== Test Output ==="
cat test_output.txt
