#!/bin/bash

# Setup Parameters
PREP_AI_DIR="/nirvana/.prep_ai"
OUTPUT_FILE="$PREP_AI_DIR/.prep_ai.output.txt"
PROMPT_FILE="$PREP_AI_DIR/.prep_ai.prompt.txt"
PROBLEM_FILE="$PREP_AI_DIR/.prep_ai.problem.txt"
ROOT_FOLDERS="$PREP_AI_DIR/../code"

# DART EXAMPLE
# EXCLUDE_DIRS="build|ios|android|.dart_tool|.git|web|macos|windows|linux|packages|images|temp|test|test_driver"
# INCLUDE_EXTS="dart|yaml"

# LARAVEL EXAMPLE
EXCLUDE_FILES="package-lock.json|composer-lock.json|0001_01_09_000001_create_permission_tables.php"
EXCLUDE_DIRS="vendor|node_modules|cache|tests|framework|logs|console"
INCLUDE_EXTS="cpp|json|h|nv"

# --- Section 0: Create file and add the prompt and problem details
> "$OUTPUT_FILE"
cat "$PROMPT_FILE" >> "$OUTPUT_FILE"
echo "" >> "$OUTPUT_FILE"
echo "The current issue is below." >> "$OUTPUT_FILE"
cat "$PROBLEM_FILE" >> "$OUTPUT_FILE"
echo "" >> "$OUTPUT_FILE"

# --- Section 1: Folder Structure (containing .dart files within lib/) ---
echo "--- Folder Structure ---" >> "$OUTPUT_FILE"
echo "" >> "$OUTPUT_FILE"

# Exclude common build/OS/dependency folders that are not part of the source code logic

# Check if the 'tree' command is available
if command -v tree &> /dev/null; then
  IFS='|' read -ra FOLDERS <<< "$ROOT_FOLDERS"
  for ROOT_FOLDER in "${FOLDERS[@]}"; do
    tree -F -P "*.dart" -I "$EXCLUDE_DIRS" "$ROOT_FOLDER" >> "$OUTPUT_FILE"
  done
else
  echo "Warning: 'tree' command not found. Listing directories containing .dart files instead." >> "$OUTPUT_FILE"
  IFS='|' read -ra FOLDERS <<< "$ROOT_FOLDERS"
  for ROOT_FOLDER in "${FOLDERS[@]}"; do
    find "$ROOT_FOLDER" -type f | grep -E "\.($INCLUDE_EXTS)$" | xargs -I {} dirname {} | sort -u | sed 's|^|./|' >> "$OUTPUT_FILE"
  done
fi
echo "" >> "$OUTPUT_FILE"

# --- Section 2: Concatenated File Contents with Headers ---
echo "--- Baseline File Contents ---" >> "$OUTPUT_FILE"
echo "" >> "$OUTPUT_FILE"

IFS='|' read -ra FOLDERS <<< "$ROOT_FOLDERS"
for ROOT_FOLDER in "${FOLDERS[@]}"; do
  find "$ROOT_FOLDER" -type f | grep -E "\.($INCLUDE_EXTS)$" | grep -Ev "$EXCLUDE_DIRS" | while read -r file; do
    filename=$(basename "$file")
    if ! echo "$filename" | grep -Eq "$EXCLUDE_FILES"; then
      echo "# $file # NOT PART OF CODE DON'T ECHO IT EVER #" >> "$OUTPUT_FILE"
      sed '/\/\*/,/\*\//d' "$file" | sed 's|//.*||' | sed '/^use /{ /Has/!d }' | sed 's/^[ \t]*//' >> "$OUTPUT_FILE"
      echo "" >> "$OUTPUT_FILE"
    fi
  done
done


echo "Generated $OUTPUT_FILE"
