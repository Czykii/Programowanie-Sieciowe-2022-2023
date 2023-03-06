# Zadanie 5
W dokumentacji POSIX API znajdź opisy czterech podstawowych funkcji plikowego wejścia-wyjścia, tzn. open, read, write i close. Czy zgadzają się one z tym, co pamiętasz z przedmiotu „Systemy operacyjne”? Jakie znaczenie ma wartość 0 zwrócona jako wynik funkcji read?

## Open

The open() function shall establish the connection between a file and a file descriptor. It shall create an open file description that refers to a file and a file descriptor that refers to that open file description. The file descriptor is used by other I/O functions to refer to that file. The path argument points to a pathname naming the file.

The open() function shall return a file descriptor for the named file, allocated as described in File Descriptor Allocation. The open file description is new, and therefore the file descriptor shall not share it with any other process in the system. The FD_CLOEXEC file descriptor flag associated with the new file descriptor shall be cleared unless the O_CLOEXEC flag is set in oflag.

## Close

The close() function shall deallocate the file descriptor indicated by fildes. To deallocate means to make the file descriptor available for return by subsequent calls to open() or other functions that allocate file descriptors. All outstanding record locks owned by the process on the file associated with the file descriptor shall be removed (that is, unlocked).

If close() is interrupted by a signal that is to be caught, it shall return -1 with errno set to [EINTR] and the state of fildes is unspecified. If an I/O error occurred while reading from or writing to the file system during close(), it may return -1 with errno set to [EIO]; if this error is returned, the state of fildes is unspecified.

## Read

The read() function shall attempt to read nbyte bytes from the file associated with the open file descriptor, fildes, into the buffer pointed to by buf. The behavior of multiple concurrent reads on the same pipe, FIFO, or terminal device is unspecified.

Before any action described below is taken, and if nbyte is zero, the read() function may detect and return errors as described below. In the absence of errors, or if error detection is not performed, the read() function shall return zero and have no other results.

**If no process has the pipe open for writing, read() shall return 0 to indicate end-of-file.**

## Write

The write() function shall attempt to write nbyte bytes from the buffer pointed to by buf to the file associated with the open file descriptor, fildes.

Before any action described below is taken, and if nbyte is zero and the file is a regular file, the write() function may detect and return errors as described below. In the absence of errors, or if error detection is not performed, the write() function shall return zero and have no other results. If nbyte is zero and the file is not a regular file, the results are unspecified.

On a regular file or other file capable of seeking, the actual writing of data shall proceed from the position in the file indicated by the file offset associated with fildes. Before successful return from write(), the file offset shall be incremented by the number of bytes actually written. On a regular file, if the position of the last byte written is greater than or equal to the length of the file, the length of the file shall be set to this position plus one.