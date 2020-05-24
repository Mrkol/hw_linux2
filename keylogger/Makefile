obj-m += keylogger.o
pbook-objs := keylogger.o

all:
	make -C ../linux-5.4.3 M=$(PWD) modules

clean:
	make -C ../linux-5.4.3 M=$(PWD) clean
