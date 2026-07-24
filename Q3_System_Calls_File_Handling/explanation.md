# Question 3 - File Processing Using Linux System Calls

## Program Behavior
The program creates a binary file of employee records using low-level 
system calls (not fopen/fread/fwrite), writes 3 records, updates one 
record's salary in-place, and demonstrates direct random access to any 
record by index.

## Commands and Explanations

**gcc employee_records.c -o employee_records && ./employee_records**
Compiles and runs the program. Output confirms records are written, then 
read back both before and after an in-place update.

**open(FILENAME, O_CREAT | O_WRONLY | O_TRUNC, 0644)**
Opens (creating if necessary) the file for writing, truncating any 
existing content. Using open() instead of fopen() returns a raw file 
descriptor, giving direct control over byte-level positioning without 
library-level buffering getting in the way.

**write(fd, &emps[i], sizeof(Employee))**
Writes the raw bytes of each Employee struct directly to the file. 
Because every record is written as a fixed-size block (sizeof(Employee)), 
each record occupies a predictable, calculable offset in the file — 
essential for random access later.

**lseek(fd, index * sizeof(Employee), SEEK_SET)**
Moves the file's read/write position directly to the byte offset of any 
record, calculated as index * record_size, relative to the start of the 
file (SEEK_SET). This allows retrieving any record in constant time 
without reading through all preceding records.

**lseek(fd, -sizeof(Employee), SEEK_CUR)**
After read() advances the file pointer past a matching record, this 
moves the pointer backward by exactly one record's size, relative to the 
current position (SEEK_CUR), positioning it back at the start of that 
record so the corrected version can be written in place — avoiding a 
full file rewrite for a single field change.

**read(fd, &e, sizeof(Employee))**
Reads exactly one record's worth of bytes into the struct. Comparing the 
return value to sizeof(Employee) detects incomplete reads or end-of-file.

**close(fd)**
Releases the file descriptor and ensures any pending OS-level buffers are 
flushed, preventing descriptor leaks, especially important since the 
program opens the file multiple times across different functions.

## How They Work Together
open() establishes a raw, low-level channel to the file. write() and 
read() move fixed-size binary records in and out of it. lseek() is the 
key to efficiency here — it lets the program jump to any record 
(SEEK_SET) or step back to overwrite a specific record just read 
(SEEK_CUR), meaning updates only touch the bytes that changed rather 
than reconstructing the entire file. close() ensures each operation 
cleanly releases the file descriptor. Together, these system calls 
implement an efficient, direct-access binary record store, similar in 
principle to how simple database engines manage fixed-length records 
on disk.