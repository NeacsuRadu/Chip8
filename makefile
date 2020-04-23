CC := g++
OBJS := main.o gpu.o cpu.o emulator.o mmu.o

all: ${OBJS}
	${CC} ${OBJS} -o main

%.o: %.cpp %.h
	${CC} -c $<

clean:
	rm -f *.o main
