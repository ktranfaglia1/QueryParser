CC=gcc 
MPICC=mpicc 

OBJS=parse.o QueryParsing.o theDatabaser.o stringStack.o structStack.o

SER= QPESeq.o searchUtility.o
OMP= QPEOMP.o searchUtilityOMP.o
MPI= QPEMPI.o searchUtility.o


all: SerialDataBase OMPDataBase MPIDataBase

SerialDataBase: $(SER) $(OBJS)
	$(CC) -o $@ $^

OMPDataBase: $(OMP) $(OBJS)
	$(CC) -O0 -fopenmp -o $@ $^

MPIDataBase: $(MPI) $(OBJS)
	mpicc -o $@ $^


QPEMPI.o: QPEMPI.c
	$(MPICC) -c -o $@ $<

%.o: %.c %.h
	$(CC) -c -o $@ $<



.PHONY: clean

clean:
	rm -f $(OBJS) $(SER) $(OMP) $(MPI) SerialDataBase OMPDataBase MPIDataBase