#source file
SOURCE  := $(wildcard *.c) $(wildcard *.cpp)
OBJS    := $(patsubst %.c, %.o, $(patsubst %.cpp, %.o,$(SOURCE)))
 
  
#compile and lib parameter
#LIBS    := -ltmloader
LDFLAGS := -L$(AU_LIB_PATH)
INCLUDE := -I. -I$(MAIN_DIR) -I$(SIMPLEINI_DIR) -I$(LOG_DIR) -I$(AU_HEAD_PATH)
CFLAGS	+= $(INCLUDE) $(LIBS)
CXXFLAGS+= $(INCLUDE) $(LIBS)
 
#i think you should do anything here
.PHONY : objs clean
  
objs : $(OBJS)

clean:
	rm -rf *.o
