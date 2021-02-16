#___________________________________________________________________________________________
#
# project name
#
# author  : archer
# date    :
# to-do   :
# caution :
#
# program : 
#
#___________________________________________________________________________________________
# targets
TARGET = run_ising

all: $(TARGET)

#___________________________________________________________________________________________
# compiler and flags
CXX      := g++
CXXFLAGS := -std=c++11 -O3 -Wno-pointer-arith -Wall #-fsanitize=address
LIBS     := -pthread -lSDL

#___________________________________________________________________________________________
# directory with the source files
SRC := ./src
OBJ := ./obj
RUN := ./run
TST := ./test

#___________________________________________________________________________________________
# includes
INC := -I $(SRC)

#___________________________________________________________________________________________
# clean 
clean:
	rm -f *~ $(OBJ)/*.o $(SRC)/*~ $(RUN)/*~ $(TST)/*~ $(TARGET)

#___________________________________________________________________________________________
#
# compile commands
#___________________________________________________________________________________________
#___________________________________________________________________________________________
# run files
run_ising: $(RUN)/run_ising.cpp $(OBJ)/ising.o $(OBJ)/timing.o $(OBJ)/range.o $(OBJ)/thread_pool.o
	$(CXX) -o $@ $(RUN)/run_ising.cpp $(LIBS) $(INC) $(CXXFLAGS)

#___________________________________________________________________________________________
# c++ object files
$(OBJ)/timing.o: $(SRC)/timing.h
	$(CXX) -o $@ -c $(SRC)/timing.h $(CXXFLAGS) $(INC) $(LIBS)

$(OBJ)/matrix.o: $(SRC)/matrix.h
	$(CXX) -o $@ -c $(SRC)/matrix.h $(CXXFLAGS) $(INC) $(LIBS)

$(OBJ)/grid.o: $(SRC)/grid.h $(OBJ)/matrix.o
	$(CXX) -o $@ -c $(SRC)/grid.h $(CXXFLAGS) $(INC) $(LIBS)

$(OBJ)/spin_config.o: $(SRC)/spin_config.h $(OBJ)/grid.o
	$(CXX) -o $@ -c $(SRC)/spin_config.h $(CXXFLAGS) $(INC) $(LIBS)

$(OBJ)/drawer.o: $(SRC)/drawer.h $(OBJ)/spin_config.o
	$(CXX) -o $@ -c $(SRC)/drawer.h $(CXXFLAGS) $(INC) $(LIBS)

$(OBJ)/ising.o: $(SRC)/ising.h $(OBJ)/drawer.o
	$(CXX) -o $@ -c $(SRC)/ising.h $(CXXFLAGS) $(INC) $(LIBS)

$(OBJ)/range.o: $(SRC)/range.h
	$(CXX) -o $@ -c $(SRC)/range.h $(CXXFLAGS) $(INC) $(LIBS)

$(OBJ)/thread_pool.o: $(SRC)/thread_pool.h
	$(CXX) -o $@ -c $(SRC)/thread_pool.h $(CXXFLAGS) $(INC) $(LIBS)

#___________________________________________________________________________________________
# non file targets
.PHONY : all clean
