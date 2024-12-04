CC=mpicc

finalFile=MPIDataBase
OBJS=QPEMPI.o parse.o QueryParsing.o searchUtilityMPI.o theDatabaser.o stringStack.o structStack.o

FLAGS=-g -o

$(finalFile): $(OBJS)
	$(CC) $(FLAGS) $@ $^

%.o: %.c %.h
	$(CC) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(OBJS) $(finalFile)