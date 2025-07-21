#!/bin/bash

# Test script to verify heredoc Ctrl+C behavior
echo "Testing heredoc Ctrl+C behavior..."

# Start minishell in background
echo "cat << EOF" | timeout 3s ./minishell &
SHELL_PID=$!

sleep 0.5

# Send SIGINT to the process group to simulate Ctrl+C
kill -INT -$SHELL_PID 2>/dev/null

wait $SHELL_PID 2>/dev/null
EXIT_CODE=$?

if [ $EXIT_CODE -eq 130 ]; then
    echo "✓ Heredoc correctly handled Ctrl+C (exit code 130)"
else
    echo "✗ Unexpected exit code: $EXIT_CODE"
fi

# Test that shell continues after heredoc interruption
echo "Testing shell continuation after heredoc interruption..."
(
    echo "cat << EOF"
    sleep 0.5
    kill -INT $$ 2>/dev/null
    sleep 0.5
    echo "echo 'Shell is still alive'"
    echo "exit"
) | timeout 5s ./minishell

echo "Test completed."
