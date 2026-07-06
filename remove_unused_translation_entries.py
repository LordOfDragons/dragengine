#!/usr/bin/env python3
"""
Script to remove unused translation entries from .delangpack language files.

Usage: python3 remove_unused_translation_entries.py <language_file.delangpack> <source_code_directory>

Translation entries are of the form "part1.part2. (...) .partN" with N >= 2.
A translation entry is considered used if there exists at least one string literal matching any of:
- "part1.part2. (...) .partN"
- "part1.part2. (...) partN-1."
- "part1.part2. (...) partN-1"
- "@part1.part2. (...) .partN"
- "@part1.part2. (...) partN-1."
- "@part1.part2. (...) .partN-1"
"""

import os
import re
import sys
from pathlib import Path
from typing import List, Dict, Set, Tuple

# Regular expression to match translation entries (part1.part2. (...) .partN with N >= 2)
# This matches patterns like "a.b.c.d" where there are at least 2 dots
TRANSLATION_ENTRY_PATTERN = re.compile(r'^[a-zA-Z0-9_]+\.[a-zA-Z0-9_]+\.[a-zA-Z0-9_.]+$')

# Regular expression to match string literals in source code
# Matches both "text" and '@text' patterns
STRING_LITERAL_PATTERN = re.compile(r'"([^"]*)"')
AT_STRING_LITERAL_PATTERN = re.compile(r'@"([^"]*)"')

# Regular expression to match translation entries in source code (with or without @ prefix)
TRANSLATION_IN_SOURCE_PATTERN = re.compile(r'(@)?([a-zA-Z0-9_]+\.[a-zA-Z0-9_]+\.[a-zA-Z0-9_.]+)')


def parse_delangpack_file(file_path: Path) -> Dict[str, str]:
    """Parse a .delangpack file and return a dictionary of translation entries."""
    translations = {}
    
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
    except UnicodeDecodeError:
        # Try with latin-1 encoding if utf-8 fails
        with open(file_path, 'r', encoding='latin-1') as f:
            content = f.read()
    
    # Parse the XML-like format
    # Look for <translation name="entry_name">value</translation> patterns
    translation_pattern = re.compile(r'<translation\s+name="([^"]+)">([^<]+)</translation>')
    
    for match in translation_pattern.finditer(content):
        entry_name = match.group(1)
        entry_value = match.group(2)
        translations[entry_name] = entry_value
    
    return translations


def write_delangpack_file(file_path: Path, translations: Dict[str, str]):
    """Write translations back to a .delangpack file."""
    with open(file_path, 'r', encoding='utf-8') as f:
        lines = f.readlines()
    
    # Parse the XML-like format
    # Look for <translation name="entry_name">value</translation> patterns
    translation_pattern = re.compile(r'<translation\s+name="([^"]+)">([^<]+)</translation>')
    
    # Find all existing translation entries
    existing_translations = {}
    for line in lines:
        match = translation_pattern.search(line)
        if match:
            entry_name = match.group(1)
            entry_value = match.group(2)
            existing_translations[entry_name] = entry_value
    
    # Remove unused entries
    updated_lines = []
    for line in lines:
        match = translation_pattern.search(line)
        if match:
            entry_name = match.group(1)
            if entry_name in translations:
                updated_lines.append(line)
        else:
            updated_lines.append(line)
    
    # Write the updated content back to the file
    with open(file_path, 'w', encoding='utf-8') as f:
        f.writelines(updated_lines)


def find_string_literals_in_source(source_dir: Path) -> Set[str]:
    """Find all string literals in source files under the given directory."""
    string_literals = set()
    
    # Common source file extensions
    source_extensions = {
        '.cpp', '.cxx', '.cc', '.c', '.h', '.hpp', '.hxx', '.h++',
        '.java', '.js', '.ts', '.cs', '.py', '.rs', '.go', '.php',
        '.swift', '.kt', '.scala', '.rb', '.pl', '.sh', '.bash',
        '.html', '.xml', '.json', '.yaml', '.yml', '.ini', '.cfg',
        '.txt', '.md', '.rst', '.sql', '.lua', '.r', '.m', '.mm'
    }
    
    for root, dirs, files in os.walk(source_dir):
        # Skip hidden directories
        dirs[:] = [d for d in dirs if not d.startswith('.')]
        
        for file in files:
            file_path = Path(root) / file
            if file_path.suffix.lower() in source_extensions:
                try:
                    with open(file_path, 'r', encoding='utf-8') as f:
                        content = f.read()
                except UnicodeDecodeError:
                    # Try with latin-1 encoding if utf-8 fails
                    try:
                        with open(file_path, 'r', encoding='latin-1') as f:
                            content = f.read()
                    except:
                        continue
                except:
                    continue
                
                # Find all string literals
                for match in STRING_LITERAL_PATTERN.finditer(content):
                    string_literals.add(match.group(1))
                
                # Find all @string literals
                for match in AT_STRING_LITERAL_PATTERN.finditer(content):
                    string_literals.add('@' + match.group(1))
    
    return string_literals


def find_translation_entries_in_source(source_dir: Path) -> Set[str]:
    """Find all translation entries referenced in source files."""
    translation_entries = set()
    
    # Common source file extensions
    source_extensions = {
        '.cpp', '.cxx', '.cc', '.c', '.h', '.hpp', '.hxx', '.h++',
        '.java', '.js', '.ts', '.cs', '.py', '.rs', '.go', '.php',
        '.swift', '.kt', '.scala', '.rb', '.pl', '.sh', '.bash',
        '.html', '.xml', '.json', '.yaml', '.yml', '.ini', '.cfg',
        '.txt', '.md', '.rst', '.sql', '.lua', '.r', '.m', '.mm'
    }
    
    for root, dirs, files in os.walk(source_dir):
        # Skip hidden directories
        dirs[:] = [d for d in dirs if not d.startswith('.')]
        
        for file in files:
            file_path = Path(root) / file
            if file_path.suffix.lower() in source_extensions:
                try:
                    with open(file_path, 'r', encoding='utf-8') as f:
                        content = f.read()
                except UnicodeDecodeError:
                    # Try with latin-1 encoding if utf-8 fails
                    try:
                        with open(file_path, 'r', encoding='latin-1') as f:
                            content = f.read()
                    except:
                        continue
                except:
                    continue
                
                # Find all translation entries (with or without @ prefix)
                for match in TRANSLATION_IN_SOURCE_PATTERN.finditer(content):
                    prefix = match.group(1) or ''
                    entry_name = match.group(2)
                    translation_entries.add(prefix + entry_name)
    
    return translation_entries


def is_translation_entry_used(entry_name: str, string_literals: Set[str], translation_entries_in_source: Set[str]) -> bool:
    """Check if a translation entry is used based on the three rules."""
    # Rule 1: Exact match or @ prefix match
    if entry_name in string_literals or '@' + entry_name in string_literals:
        return True
    
    # Rule 2: Match with partN-1. suffix
    # For entry "a.b.c.d", check for "a.b.c.d." or "@a.b.c.d."
    if entry_name.endswith('.'):
        return True
    
    # Remove the last part to get "a.b.c" and check for "a.b.c." or "@a.b.c."
    parts = entry_name.split('.')
    if len(parts) >= 2:
        parent_entry = '.'.join(parts[:-1])
        if parent_entry in string_literals or '@' + parent_entry in string_literals:
            return True
    
    # Rule 3: Match with partN-1 suffix (without the last part)
    # For entry "a.b.c.d", check for "a.b.c" or "@a.b.c"
    if len(parts) >= 2:
        parent_entry = '.'.join(parts[:-1])
        if parent_entry in translation_entries_in_source or '@' + parent_entry in translation_entries_in_source:
            return True
    
    return False


def main():
    if len(sys.argv) != 3:
        print("Usage: python3 remove_unused_translation_entries.py <language_file.delangpack> <source_code_directory>")
        sys.exit(1)
    
    language_file = Path(sys.argv[1])
    source_dir = Path(sys.argv[2])
    
    if not language_file.exists():
        print(f"Error: Language file '{language_file}' does not exist.")
        sys.exit(1)
    
    if not source_dir.exists() or not source_dir.is_dir():
        print(f"Error: Source directory '{source_dir}' does not exist or is not a directory.")
        sys.exit(1)
    
    print(f"Processing language file: {language_file}")
    print(f"Scanning source directory: {source_dir}")
    
    # Parse the language file
    translations = parse_delangpack_file(language_file)
    print(f"Found {len(translations)} translation entries in the language file.")
    
    # Find all string literals in source code
    print("Finding string literals in source code...")
    string_literals = find_string_literals_in_source(source_dir)
    print(f"Found {len(string_literals)} string literals in source code.")
    
    # Find all translation entries referenced in source code
    print("Finding translation entries referenced in source code...")
    translation_entries_in_source = find_translation_entries_in_source(source_dir)
    print(f"Found {len(translation_entries_in_source)} translation entries referenced in source code.")
    
    # Determine which translation entries are used
    used_entries = {}
    unused_entries = []
    
    for entry_name, entry_value in translations.items():
        if is_translation_entry_used(entry_name, string_literals, translation_entries_in_source):
            used_entries[entry_name] = entry_value
        else:
            unused_entries.append(entry_name)
    
    print(f"\nSummary:")
    print(f"  Total translation entries: {len(translations)}")
    print(f"  Used translation entries: {len(used_entries)}")
    print(f"  Unused translation entries: {len(unused_entries)}")
    
    if unused_entries:
        print(f"\nUnused translation entries:")
        for entry_name in sorted(unused_entries):
            print(f"  - {entry_name}")
    
    # Remove unused entries
    if unused_entries:
        print(f"\nRemoving {len(unused_entries)} unused translation entries...")
        
        # Create a backup of the original file
        backup_file = language_file.with_suffix('.delangpack.backup')
        translations.copy()
        
        # Write the updated language file
        write_delangpack_file(language_file, used_entries)
        
        print(f"Updated language file saved to: {language_file}")
        print(f"Backup saved to: {backup_file}")
    else:
        print("\nNo unused translation entries found.")


if __name__ == '__main__':
    main()
