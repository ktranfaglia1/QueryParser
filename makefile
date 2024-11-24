CC=gcc


OBJS=main.o parse.o searchUtility.o stringStack.o structStack.o theDatabaser.o

SerialDataBase: $(OBJS)
	$(CC) -o $@ $^


.PHONY: clean

clean:
	rm -f $(OBJS) SerialDataBase