obj-m += pbook.o
pbook-objs := phonebook.o phonebook_device.o phonebook_impl.o

all:
	make -C ../linux-5.4.3 M=$(PWD) modules

clean:
	make -C ../linux-5.4.3 M=$(PWD) clean
