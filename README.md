# COTTY

Control inputs to another TTY from one TTY. \
It automatically enters interactive mode if -d is not passed.

### SYNOPSIS
	cotty -T path/to/tty [-v] [-d] [-c] [-b backspace-count] [-s string-to-send] [-n] [-h]

### OPTIONS
	-T	Path to the (slave) tty you want to control.
	-v	Verbose output.
	-d	Disable interactive mode.
	-c	Clear Screen.
	-b	Number of backspaces to send to the tty. Used as the BACKSPACE/DELETE(Mac) key.
	-s	String to send.
	-n	Send newline. Used as the ENTER/RETURN(Mac) key.
	-C	Send Ctrl+c to slave tty.
	-h	Show this help text block and exit.

## Installation

```bash
curl -s https://raw.githubusercontent.com/DaBigBlob/cotty/main/install.sh | sh
```

## Run
```bash
./cotty -T path/to/slave/tty
```
**or**
```bash
cotty -T path/to/slave/tty
```
if it was added to your $PATH

### BUGS
	Does not work on MacOS due to restrictions on input buffer of another terminal.
