#!/bin/bash

echo "Testing heredoc with Ctrl+C behavior"
echo "This will start minishell and send Ctrl+C during heredoc"

# Create a test script to send to minishell
cat << 'EOF' > test_input.txt
cat << END
first line
second line

EOF

echo "Starting minishell..."
echo "You should manually test:"
echo "1. Type: cat << END"
echo "2. Type some content"
echo "3. Press Ctrl+C - should show '^C' on the heredoc line"
echo "4. Should return to single minishell> prompt"

./minishell
