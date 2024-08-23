#!/bin/bash
#
# This script is part of a test suite designed to compile and run tests
# for a Sudoku application. It also handles cleanup after the tests are run.
# The script provides colored output for better readability of the test results.

GREEN="\033[0;32m"
RED="\033[0;31m"
YELLOW="\033[1;33m"
BLUE="\033[1;34m"
RESET="\033[0m"
CHECK_MARK="\xE2\x9C\x94"
CROSS_MARK="\xE2\x9C\x98"
BOLD="\033[1m"

echo -e "${BOLD}Compiling the tests...${RESET}"
make test
if [ $? -ne 0 ]; then
    echo -e "${RED}${CROSS_MARK} Compilation failed!${RESET}"
    exit 1
fi
echo -e "${GREEN}${CHECK_MARK} Compilation successful.${RESET}\n"

PUZZLE_DIR="puzzles"

echo -e "${BOLD}Testing...${RESET}"
for test_file in "${PUZZLE_DIR}"/*.txt; do
    test_name=$(basename "${test_file}")

    ./test_sudoku "${test_file}"

    result=$?
    if [ ${result} -eq 0 ]; then
        echo -e "${GREEN}${CHECK_MARK} ${test_name}${RESET}"
    else
        echo -e "${RED}${CROSS_MARK} ${test_name}${RESET}"
    fi
done

echo -e "\n${YELLOW}Cleaning up...${RESET}"
make clean

echo -e "${BLUE}${BOLD}Test suite completed.${RESET}"