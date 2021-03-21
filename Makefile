CC = gcc
IDIR = include
CFLAGS = -I$(IDIR)

SOURCES = src/main.c include/cliargs.c include/write.c
OBJECTS = main.o cliargs.o write.o

all : eepromIHEX

install :  
	mkdir /opt/eepromIHEX
	cp eepromIHEX /opt/eepromIHEX
	ln -f -s /opt/eepromIHEX/eepromIHEX /usr/local/bin

uninstall :
	rm /usr/local/bin/eepromIHEX
	rm -r /opt/eepromIHEX

eepromIHEX: $(OBJECTS)
	$(CC) -o eepromIHEX $(CFLAGS) $^
	mkdir obj
	mv $(OBJECTS) obj 

$(OBJECTS) : $(SOURCES)
	$(CC) -c $^

.PHONY: clean

clean :
	rm -r obj 
