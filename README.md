# Gerp

**Gerp** is a C++ command-line utility that mimics the behavior of Unix `grep`. It searches through all text files in a given directory, returning every line in which a specified word appears, along with the corresponding file path and line number.

## Compile & Run

To compile:
```bash
make
```

To run:
```bash
./gerp <inputDirectory> <outputFile>
```

## Features

- Case-sensitive and insensitive search options using separate hash tables
- Efficient performance with average-case constant time lookups using custom-built hash tables
- Output redirection and intuitive command handling via a command loop
- File system traversal using a dynamically constructed directory tree

## Architecture Overview

Upon startup, Gerp builds a directory tree from the specified input directory, parsing all contained text files. Each word is indexed in both a case-sensitive and case-insensitive hash table for fast querying. Search results return file paths, line numbers, and corresponding lines, with commands supporting:

- Case-sensitive search: `<word>`
- Case-insensitive search: `@i <word>`
- Output redirection: `@f <new_output_file>`
- Exit: `@q`

## Data Structures

- **Hash Tables:** Core to indexing and searching words quickly. Uses chaining with linked lists to handle collisions and maintains a second table for case-insensitive matches.
- **Vectors:** Used to store file lines and hash table buckets due to constant-time access and dynamic sizing.
- **Linked Lists:** Employed for chaining in hash table buckets, optimizing space by allocating memory only when needed.

## Testing & Validation

The application was rigorously tested through manual input, edge case handling, and output comparison against expected results. Debugging included unit tests, print trace debugging, and reference behavior comparisons to ensure reliable performance. Post-validation, further optimizations were made to improve runtime and memory efficiency.
