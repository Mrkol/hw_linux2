#!/usr/bin/expect -f

spawn scp -P2222 ./phonebook/pbook.ko tc@localhost:~
expect "*assword:*"
send "penispenis228\r"
expect eof

spawn scp -P2222 ./syscall_test.c tc@localhost:~
expect "*assword:*"
send "penispenis228\r"
expect eof

spawn scp -r -P2222 ./headers/include tc@localhost:/usr/
expect "*assword:*"
send "penispenis228\r"
expect eof

exit
