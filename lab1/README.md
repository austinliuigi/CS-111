## UID: 305785051

## Pipe Up

This program replicates the bash pipe operator, `|`. It takes in one or more
commands and pipes the output of the previous command into the input of the
current command (unless the current command is the first).

## Building

```
make
```

## Running

```
$ ./pipe
usage: ./pipe CMD...

$ echo $?
22
```

```
$ ./pipe ls wc
      8      8     68

$ echo $?
0
```

```
$ ./pipe ls nonexistent_command
pipe: could not execute nonexistent_command

$ echo $?
2
```

## Cleaning up

```
make clean
```
