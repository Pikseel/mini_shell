#!/bin/bash

# Test script to simulate the exact scenario described
echo "Testing heredoc Ctrl+C behavior:"
echo "Expected output:"
echo "minishell> cat << asad"
echo "> eqwewq"  
echo "> ew"
echo "> w"
echo "> e"
echo "> ^C"
echo "minishell> "
echo ""
echo "Starting test..."

# Create input that mimics the user's scenario
(
    echo "cat << asad"
    echo "eqwewq"
    echo "ew" 
    echo "w"
    echo "e"
    # Send interrupt after some lines
    sleep 0.5
    # Simulate Ctrl+C by sending SIGINT to the process group
    kill -INT 0 2>/dev/null &
    sleep 0.1
    echo "echo 'test after interrupt'"
    echo "exit"
) | timeout 10s ./minishell

echo "Test completed."
