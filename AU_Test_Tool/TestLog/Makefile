#source file
SOURCE  := $(wildcard *.c) $(wildcard *.cpp)
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

##
INCLUDE	:= -I$(TINYXML_DIR)
CFLAGS	+= $(INCLUDE)
CXXFLAGS+= $(INCLUDE)
 
##
.PHONY : objs clean
  
objs : $(OBJS)
	
clean :
	rm -fr *.o
