# String Manipulation in C

String manipulation is essential in C.
We need to understand data types and pointers, which is also required for system programming.
Please also refer to the [slide](3.pdf).

## 1. Integer and Character

Open `3a.c` (e.g., using `vim`) and the slide page 3.
Also, compile (`gcc ./3a.c`) and execute (`./a.out`) to see the output.
The first `printf()` prints the address (location in the virtual memory) of `i`,
the value (content) of `i`, the address of `c` and the value (content) of `c`.
Since the data type of `i` is an integer, we use `%d` to output, and since that
of `c` is a character, we use `%c`. Addresses (`&i` and `&c`)are usually printed using `%p`.

In the next line, `char *cp` defines a pointer, which is a variable to store an
address, thereby substituting `&c` in the next line.
The subsequent `printf()` prints the address of `cp`, the content of `cp` (i.e.,
address of `c`) and the content of `c`.  You can see the illustration of memory
layout in the slide.

The final `printf()` is slightly unusual; it prints the character (i.e., `c`) as an integer.  When executing, it prints 97. The trick is that a character is represented an integer between 0 and 127 (you can see that by `man ascii` command). The `printf()` outputs the corresponding character when `%c` is specified.

Next, open `3b.c` and look at the slide page 4.
`char foo[4];` defines an array, which stores four characters.
String in C is an array of characters, which must be terminated by the special
character `'\0'`. In this file we store `'a'`, `'b'`, `'z'` and `'\0'`.
The first `printf()` prints the array; it recognizes `%s` which indicates a
string, and outputs until `'\0'`.
The second `printf()` prints individual characters of `foo`.

After that, we substitute `foo` to the pointer to a character.
By definition, the name of an array without index (e.g., `foo` as opposed to
`foo[0]`, `foo[1]` and etc.) means the pointer to the first element. In this
case, it is an array of characters, so `foo` is a pointer to a character, which
can be substituted to `char *p`. Check the output of the third `printf()` in the
file.

Finally , to further understand the relationship between value, pointer and array, we substitute the address of `foo[1]` (i.e., `&foo[1]`) to `p`, and print the content of that, which is 'b'.

Congratulations, now we understood string manipulation in C.

## 2. Hash Table

Okay, now open `3c.c` and look at the page 5 of the slides.
The goal is to find the price of a given item by searching for the table shown
in the slide.  Each row is 10 characters long; the last slot always stores the price, meaning that each row can accommodate up to 8 characters of name, taking into account the terminating `'\0'`.
The simplest algorithm to look up the price of a given item would scan the rows from the top in turn.
For example, if "kiwi" is given, the algorithm first takes the first row, 
realizes "apple" does not match "kiwi", goes to the next row and repeats until
it finds the target row.

The code look like `3c.c`. You can try this code by compiling the file and
execute `./a.out kiwi` (you can change "kiwi" to another item available in
the table).
You will realize the output is wrong; you need to implement a
function `scmp()`, which currently always returns 0, to return 0 only if two
strings are identical and otherwise a non-zero value. The correct implementation
can be found in `3c2.c`.
In `3c.c`, the first `for` loop builds the table in the slide, and the second
one looks up the key provided on execution (e.g., "kiwi").  Take your time to
understand the code.

We now realize this algorithm would be slow if the table is large.
This is where a *hash table* comes in to achieve constant-time lookup
irrespective of the table size.
Take a look at the page 6 of the slide.
The idea is to convert the key to an integer within a fixed range of value, say,
0 to 127. The simplest one is the reminder of the sum of the string characters in integer format (e.g., 'kiwi' is (107+105+119+105) % 128 = 52).

In `3d.c`, the first `for` loop populates the hash table based on this principle.
The inner `for` loop adds up the characters in integer format to `hash`, and
store the pointer to the target row (`key[i]`) in the hashtable slot whose index
is the hash value.
Take your time to understand these code.

It is trivial to look up the hash table. Like `3c.c`, this program takes a
key to find the price as argument (e.g., if you execute the program with
`./a.out kiwi`, kiwi is the key, and stored in `char *test`). We just obtain the hash value for
the key (this part is missing in `3d.c`, so implement it by yourself; if you
cannot, look at `3d2.c`), and look up the hash table array using the hash value as the index.
Since the content is a pointer to the target row in the main table, we can
obtain the price straight away by looking at the particular part of the row
(i.e., 9th byte).

This hash table clearly has a flaw. If multiple keys have the same hash value,
the first one will be overwritten. To avoid this, in practice, many cases use a
linked list for each hash table entry (hash bucket), assuming the list does not
become too long.
