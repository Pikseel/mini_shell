#!/bin/bash

# Test script for minishell expansion functionality

cd /home/mecavus/Desktop/son

echo "Testing basic expansion..."
echo 'echo $USER' | timeout 5s ./minishell

echo ""
echo "Testing quotes with expansion..."
echo 'echo "Hello $USER"' | timeout 5s ./minishell

echo ""
echo "Testing single quotes (no expansion)..."
echo "echo 'Hello \$USER'" | timeout 5s ./minishell

echo ""
echo "Testing exit status..."
echo 'echo $?' | timeout 5s ./minishell

echo ""
echo "Testing word splitting..."
echo 'export TEST="one two three"; echo $TEST' | timeout 5s ./minishell

echo ""
echo "Testing complex export and expansion..."
echo 'export AKF="ho -n"' | timeout 5s ./minishell
echo 'ec$AKF "hello claude"' | timeout 5s ./minishell

echo ""
echo "Testing variable assignment from another variable..."
echo 'export CVS=$AKF' | timeout 5s ./minishell
echo 'ec$AKF "hello claude"' | timeout 5s ./minishell

echo ""
echo "Testing single quotes around expanded variable..."
echo "echo \"'\$USER'\"" | timeout 5s ./minishell

echo ""
echo "Testing complex quote mixing..."
echo "echo ''\"'\$USER'\"''" | timeout 5s ./minishell

echo ""
echo "Testing empty variable expansion..."
echo 'export EMPTY=""; echo "[$EMPTY]"' | timeout 5s ./minishell

echo ""
echo "Testing variable expansion in different positions..."
echo 'export PREFIX="test_"; echo ${PREFIX}file' | timeout 5s ./minishell

echo ""
echo "Testing multiple variables in one string..."
echo 'echo "$USER works on $PWD"' | timeout 5s ./minishell

echo ""
echo "Testing variable with special characters..."
echo 'export SPECIAL="hello world!@#"; echo $SPECIAL' | timeout 5s ./minishell

echo ""
echo "Testing nested quotes..."
echo 'echo "He said \"Hello $USER\""' | timeout 5s ./minishell
