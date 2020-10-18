# File I/O in C

File I/O in C is based on the file abstraction in the OS kernel.
Please also refer to the [slide](4.pdf).

## 1. Read/Write from/to a File

Open `4a.c`.
This code simply reads the content from the file given by the command line
argument (e.g., 4a.c if you execute the compiled code with `./a.out 4a.c`).
`open()` returns a file descriptor, which is a unique integer within the process.
File descriptors are allocated from the lowest number. We should get 3 because
0, 1 and 2 are reserved for standard input, standard output and standard error,
respectively.
This means that I/O interfaces for files and devices are the same.
At the end of execution, this code outputs the content to the standard output.
The file descriptor opened automatically goes away when the process dies.

Next, let's open `4b.c`.
This code takes two argument: source and destination file names to copy the
content.
You will notice that the code opens the destination file with `O_WRONLY|O_CREAT` and 0644 options. The first option indicates the file to be opened for write and to be created if it does not exist. The second option specifies the file permission.
This code copies data at most 1924 bytes, as specified in the buffer size.


## 2. On-Disk Data Structures

In the [week3](../week3/README.md) we created a simple in-memory key-value
store (see the page 2 of [slide](4.pdf)), which creates a database (table of
fruit and price) on the fly, which goes away when the process exits (slide page
3 for illustration).
The goal of this week is to save the database, then load the saved database next time the
program runs.

`4c.c` stores the data table and hash table created into the file `db` and
`hashtable`, respectively.
The first important point is the hash table contents.
In [week3/3d2.c](../week3/3d2.c), we stored the pointers or addresses of the table
rows. But when we store data in a file and use the file in
different code, we cannot save the pointers or addresses of variables, which are
local to the code.
We therefore store the table row index in the hash table.

The second important point is that we transform the two dimensional array (`keys`) to a
single dimensional one (`db`) in the second `for` loop.  In this code we actually
don't need this, but we are doing this to emphasize that we need the byte stream
representation of data in the on-disk file.
We then write the flattened data to the file called db (also to the standard
output, or the file descriptor 1).
Finally, we store the hashtable to the file `hashtable`, which we have created
without storing addresses.
Compile and execute the code; you will see `db` and `hashtable` created in your
directory (please remove these files before re-executing this program).

`4d.c` restores the data and hash tables, and looks up the price of the fruit
given by the argument (e.g., kiwi if you execute the program with `./a.out
kiwi`).  In the first `open()` and `read()`, it reads the data table in `buf`, and restores the content in `keys`.
In the second pair of the these calls, it reads the hash table.
Compare `4c.c` and `4d.c` with [week3/3d2.c](../week3/3d2.c).
