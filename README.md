# minimon
reduced monitor for small mcu

This monitor provides the minimal interface to interact with a computer:

* you can read memory contents,
* you can change memory contens,
* you can execute functions that reside in memory.

Granted that is not very much. No assembly language is supported yet.
But the interface is not very big and I hope that I can migrate the monitor to different technologies.

Currently only the Unix version is provided as `minimon.cpp`.

## Usage

The monitor starts by printing an address:

```
000109e4: 
```

To display the content at this address you can simply press return.
The byte at the current address is printed and the next address is displayed:

```
000109e4: 00
000109e5: 
```

### Changing Memory

To change the memory at the displayed address simply enter the two hex digits of the new byte value and press return.
Then also the next address is displayed.

### Changing the current address

If you enter more than two digits, the value is treated as a new memory address.
That way you can change the current memory address.

### Executing a function

With `j` you can jump to the machine programm at the current memory address.

## Building

You can build `minimon` by running `make` on your terminal.

