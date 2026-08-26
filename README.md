# rori.sh

A Unix shell built from scratch in C, no external shell libraries, just raw system calls. Built to actually understand what a terminal is doing every time you type a command, instead of just using bash without knowing how it works underneath.

## Features

- **REPL loop**: reads input, evaluates it, prints output, and loops forever until you exit
- **Command execution**: runs any external program via `fork()` and `execvp()`
- **Variable-length arguments**: handles any number of arguments, not just a fixed count (`ls -l -a -h` works, not just `ls -la`)
- **Built-in commands**: `cd` and `exit`, handled natively in the shell process (not forked out) since they have to modify the shell's own state
- **Piping**: `cmd1 | cmd2` chains two commands together, connecting one process's output directly to another's input via a real Unix pipe
- **Output redirection**: `cmd > file.txt` writes a command's output to a file instead of the screen
- **Signal handling**: `Ctrl+C` kills the running command, not the shell itself
- **Crash-safe on empty input**: hitting Enter with nothing typed no longer segfaults

## Build & Run

```bash
clang main.c -o main
./main
```

## Example usage

```
◆ ls -la
◆ cd ..
◆ ls -la | grep main
◆ ls -la > output.txt
◆ exit
```

## What I learned

This project was my introduction to C and systems programming. Building it meant actually implementing, by hand, the mechanisms every shell relies on.

- **fork/exec**: how one process clones itself, and one copy transforms into an entirely different program
- **Process management**: why a parent process has to explicitly `wait()` for its children, and what breaks when it doesn't
- **Pipes and file descriptors**: how two independent processes pass data to each other without ever touching the screen
- **Signal handling**: how `Ctrl+C` actually works at the OS level, and how to control it
- **Manual memory and string handling**: parsing input character by character with `strtok`, `strchr`, and raw arrays instead of relying on a built in string type

## Why C

I wanted to get comfortable with the things Python normally hides: memory, pointers, manual string handling, since that's the muscle low level and systems adjacent roles actually care about. Slower and more annoying than `.split()`, but that friction was the whole point.

## Roadmap / next steps

- More built-ins (`pwd`, `history`, `alias`)
- Input redirection (`<`)
- Job control and background processes (`&`)
- A terminal emulator to pair with this shell, as a follow up project