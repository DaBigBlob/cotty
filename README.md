# COTTY

Control inputs to another TTY from one TTY.

### NAME
	cotty - COntrol TTY

### SYNOPSIS
	cotty -T path/to/tty [-v] [-d] [-c] [-b backspace-count] [-s string-to-send] [-n] [-h]

### DESCRIPTION
	Control input to a tty.

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

### ORDER OF OPERATIONS
#### The following is the order of operation in 1(one) run of fatty in (non-interactive mode) irrespective of order of options.
	1	(-C)	Send Ctrl+c.
	2	(-b)	Send backspaces.
	3	(-c)	Clear Screen.
	4	(-s)	Send string.
	5	(-n)	Send newline character.
	6	_       Enter interactive mode.

### BUGS
	Does not work on MacOS due to restrictions on input buffer of another terminal.

## Installation

```bash
git clone https://github.com/DaBigBlob/cotty.git
cd cotty
make
```

## Run
```bash
./cotty -T path/to/slave/tty
```