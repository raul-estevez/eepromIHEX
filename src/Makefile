CC = gcc
IDIR = ../include
CFLAGS = -I$(IDIR)
TARGET = main.out
ODIR = obj

# Busca todos los .c en el dir actual y en el include dir
SOURCES = $(wildcard *.c) $(wildcard $(IDIR)/*.c)
# Convierte todo lo encontrado anteriormente en *.o files
O = $(patsubst %.c,%.o,$(SOURCES))
# Quita el ../include/ de las entradas, dejando solo los nombres.o 
OBJECTS = $(patsubst $(IDIR)/%.o,%.o,$(O))

all : $(TARGET).out

$(TARGET).out : $(OBJECTS)
	$(CC) -o $(TARGET) $(CFLAGS) $^
	mv $(OBJECTS) $(ODIR) 

$(OBJECTS) : $(SOURCES)
	$(CC) -c $^

.PHONY: clean

clean :
	rm obj/*.o 
