CC=gcc


OBJS=OMPMain.o parse.o QueryParsing.o searchUtilityOMP.o theDatabaser.o stringStack.o structStack.o
FLAG=-fopenmp

OMPDataBase: $(OBJS) 
	$(CC) -o  $@ $(FLAG) $^

%.o: %.c %.h
	$(CC) -c -o $@ $(FLAG) $<

.PHONY: clean

clean:
	rm -f $(OBJS) OMPDataBase
