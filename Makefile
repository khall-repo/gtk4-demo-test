CXX		  := gcc
CXX_FLAGS := -Wall -Wextra -std=c11 -g $(shell pkg-config --cflags gtk4 glib-2.0)

SRC_MAIN_DIR       := src
SRC_RESOURCE_DIR   := src/resource

INC_MAIN_DIR       := src/inc
INC_RESOURCE_DIR    := src/resource

BIN		:= bin
SRC		:= $(wildcard $(SRC_RESOURCE_DIR)/*.c) \
           $(wildcard $(SRC_MAIN_DIR)/*.c)
INCLUDE :=  $(INC_RESOURCE_DIR) \
			$(INC_MAIN_DIR)
INC_PARAMS = $(foreach d, $(INCLUDE), -I$d)
LIB		:= lib

LIBRARIES	:= $(shell pkg-config --libs gtk4 glib-2.0)
EXECUTABLE	:= main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)
	$(CXX) $(CXX_FLAGS) $(INC_PARAMS) $^ -o $@ $(LIBRARIES)
#	$(CXX) $(CXX_FLAGS) $(INC_PARAMS) $^ -o $@ $(LIBRARIES)
#	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	install -d $(BIN)
	-rm -f $(BIN)/*
