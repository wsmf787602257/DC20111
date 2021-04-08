CFLAGS=-c -g -Wall

export CFLAGS

DIRS=main src obj

ALL:$(DIRS)

$(DIRS):ECHO
	make -C $@
ECHO:
	@echo $(DIRS)

.PHONY:clean

clean:
	rm ./obj/*.o ./bin/serverSystem
