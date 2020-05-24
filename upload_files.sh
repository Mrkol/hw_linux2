#!/usr/bin/expect -f

spawn scp -P2222 ./phonebook/pbook.ko tc@localhost:~
expect "*assword:*"
send "test123test\r"
expect eof

spawn scp -P2222 ./keylogger/keylogger.ko tc@localhost:~
expect "*assword:*"
send "test123test\r"
expect eof

spawn scp -P2222 ./syscall_test.c tc@localhost:~
expect "*assword:*"
send "test123test\r"
expect eof

# spawn scp -r -P2222 ./headers/include tc@localhost:/usr/
# expect "*assword:*"
# send "test123test\r"
# expect eof

exit
