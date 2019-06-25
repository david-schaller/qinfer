CC=gcc-9
CXX=g++-9
RM=rm -f
CPPFLAGS=-g -Wall -std=c++17
LDFLAGS=-g -lstdc++fs

TARGET=qinfer

SRC=src
INC=-I include
OBJ=obj
BIN=bin

SRCS=src/main.cpp
OBJS=$(subst $(SRC)/,$(OBJ)/,$(subst .cpp,.o,$(SRCS)))

all: $(TARGET)

qinfer: $(OBJS)
	mkdir -p $(BIN)
	$(CXX) $(LDFLAGS) -o $(BIN)/$(TARGET) $(OBJS)

$(OBJ)/%.o: $(SRC)/%.cpp
	mkdir -p $(OBJ)
	$(CXX) $(CPPFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(OBJ)
	rm -rf $(BIN)
