# Unix Utilities

## pwd.c
Prints current working directory. Uses `getcwd()`.

## echo.c
Prints command-line arguments separated by spaces. Uses `write()`.

## cp.c
Copies a file. Uses `open()`, `read()`, `write()`, `close()`.

## mv.c
Copies a file then deletes the original. Uses `open()`, `read()`, `write()`, `close()`, `unlink()`.

## femtoshell.c
a shell that uses 2 commands : echo and exit
## picoshell.c
A lightweight Unix shell written in C featuring dynamic memory management, POSIX process execution (fork/execvp), custom 
tokenization, builtin command handling, and robust status tracking.
