SRC_F := $(shell find src -name "*.c")
OBJ_F := $(patsubst %.c,%.o,$(SRC_F))

TARGET := bin/main.bin

LIBS = -lm -lpthread

FLAGS := -Wall -pedantic -g

all: $(OBJ_F)
	gcc $(FLAGS) $^ -o $(TARGET) $(LIBS)

run_main: all
	$(TARGET)
	eog test.ppm

%.o: %.c
	gcc -c $(FLAGS) $^ -o $@

clean:
	rm -f $(TARGET) $(OBJ_F)