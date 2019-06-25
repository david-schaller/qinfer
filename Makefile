CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -Wall
LDFLAGS=-g

TARGET=qinfer

SRC=src
INC=include
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
	$(CXX) -I $(INC) -c $< -o $@

clean:
	rm -rf $(OBJ)
	rm -rf $(BIN)
