#source file
SOURCE  := $(wildcard *.c) $(wildcard *.cpp)
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

## 
.PHONY : objs clean

objs : $(OBJS)
 
clean :
	rm -fr *.o
