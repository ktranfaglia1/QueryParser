CC=mpicc


OBJS=QPEMPI.o parse.o QueryParsing.o searchUtility.o theDatabaser.o stringStack.o structStack.o

MPIDataBase: $(OBJS)
	$(CC) -o $@ $^

%.o: %.c %.h
	$(CC) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(OBJS) MPIDataBase