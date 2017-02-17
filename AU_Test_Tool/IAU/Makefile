#source file
SOURCE  := $(wildcard *.c) $(wildcard *.cpp)
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))
  
  
#compile and lib parameter
LDFLAGS :=
INCLUDE := -I. -I$(MAIN_DIR) -I$(SIMPLEINI_DIR) -I$(TINYXML_DIR) -I$(LOG_DIR) -I$(IAU_HEAD_PATH)
CFLAGS  += $(INCLUDE)
CXXFLAGS+= $(INCLUDE)
  
  
#i think you should do anything here
.PHONY : objs clean
  
objs : $(OBJS)
                
clean :
	rm -fr *.o
  
