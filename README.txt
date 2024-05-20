Assignment 1

The program is looking if the commands - '/etc/ld.so.preload' and '/etc/ld.so.cache' - exists,
if them both exists, the program wants to organize the same libraries that they share.
(libc.so.6) - that means the program loads the C standard library.
Then, the program creates a directory named 'welcome'.
Inside the directory, it creates the files - "Welcome", "To", "OS-2024".
In file "Welcome" the program writes "maya_hod\nIf you haven't read the course guidelines yet
 --- do it right now!\n1".
In file 'To' the program writes "Start exercises early!".
In file 'OS-2024' the program writes "Good luck!".
unlink deletes all the files we created which are- "Welcome", "To", "OS-2024".
rmdir removes the "Welcome" directory.
Then it exits, terminates the program with a status of 0- which means - successful execution.