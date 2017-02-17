##
TARGET		:= AU_Test_Tool
##
PWD		:= $(shell pwd)
SIMPLEINI_DIR 	:= $(PWD)/simpleini
TINYXML_DIR	:= $(PWD)/tinyxml
LOG_DIR		:= $(PWD)/TestLog
AU_DIR 		:= $(PWD)/AU
IAU_DIR 	:= $(PWD)/IAU

OBJS_DIR	:= $(PWD)/objs/
MAIN_DIR	:= $(PWD)

## AU
AU_HEAD_PATH	:=$(PWD)/../AU2861039/redhatAS4/include/
AU_LIB_PATH	:=$(PWD)/../AU2861039/redhatAS4/debug/lib/
ifeq ($(shell getconf LONG_BIT), 64)
AU_HEAD_PATH	:=$(PWD)/../AU2861039/ubuntu8/include/
AU_LIB_PATH	:=$(PWD)/../AU2861039/ubuntu8/debug/lib/
endif

## IAU
IAU_HEAD_PATH := $(PWD)/../IAU/redhat32/client/include/
ifeq ($(shell getconf LONG_BIT), 64)
IAU_HEAD_PATH := $(PWD)/../IAU/redhat64/client/include/
endif


##
CC 	:= gcc
CFLAGS 	:= -Wall -g -O3
CXXFLAGS:= $(CFLAGS)

##
export

.PHONY: everything objs clean veryclean rebuild

all: everything transfer

everything: 
	make -C $(SIMPLEINI_DIR)
	make -C $(TINYXML_DIR)
	make -C $(LOG_DIR)
	make -C $(AU_DIR)
	make -C $(IAU_DIR)
	make -f Makefile_main

transfer:	
	cp $(SIMPLEINI_DIR)/*.o	$(OBJS_DIR)
	cp $(TINYXML_DIR)/*.o 	$(OBJS_DIR)
	cp $(LOG_DIR)/*.o 	$(OBJS_DIR)
	cp $(AU_DIR)/*.o 	$(OBJS_DIR)
	cp $(IAU_DIR)/*.o 	$(OBJS_DIR)
	cp $(MAIN_DIR)/*.o 	$(OBJS_DIR)
	make -C $(OBJS_DIR)
	cp $(OBJS_DIR)/$(TARGET) $(MAIN_DIR)/$(TARGET)

rebuild: veryclean everything

clean:
	rm -rf *.o
	rm -rf $(SIMPLEINI_DIR)/*.o
	rm -rf $(TINYXML_DIR)/*.o
	rm -rf $(LOG_DIR)/*.o
	rm -rf $(AU_DIR)/*.o
	rm -rf $(IAU_DIR)/*.o
	rm -rf $(OBJS_DIR)/*.o

veryclean: clean
	rm -rf $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS) -ldl
