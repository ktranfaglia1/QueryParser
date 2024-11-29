CC=mpicc

finalFile=MPIDataBase
OBJS=MPIMain.o parse.o QueryParsing.o searchUtilityMPI.o theDatabaser.o stringStack.o structStack.o

$(finalFile): $(OBJS)
	$(CC) -o $@ $^

%.o: %.c %.h
	$(CC) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(OBJS) $(finalFile)