CC=gcc


OBJS=serialMain.o parse.o QueryParsing.o searchUtility.o theDatabaser.o stringStack.o structStack.o

SerialDataBase: $(OBJS)
	$(CC) -o $@ $^

%.o: %.c %.h
	$(CC) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(OBJS) SerialDataBase