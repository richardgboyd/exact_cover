

# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = ../../gtest-1.6.0
USER_DIR = .

CPPFLAGS += -I$(GTEST_DIR)/include
CXXFLAGS += -g -Wall -Wextra -DUNIT_TEST

TESTS = testCell.exe testShapes.exe testBoard.exe testStateSpace.exe
STATES = Triomino.exe Tetromino.exe

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

all : $(TESTS) $(STATES)

clean :
	rm -f $(TESTS) gtest.a gtest_main.a *.o

tests : $(TESTS)

states : $(STATES)

GTEST_PATH = ../gtest-1.6.0
GTEST_INCLUDE = $(GTEST_PATH)/include
GTEST_LIB = $(GTEST_PATH)/lib
INCLUDES = -I$(TEST_PATH) -I$(GTEST_PATH) -I$(GTEST_INCLUDE)

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Build test files
testCell.exe : testCell.o Shapes.o gtest.a gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ -lpthread 

testShapes.exe : testShapes.o Shapes.o gtest.a gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ -lpthread 

testBoard.exe : testBoard.o Board.o Shapes.o gtest.a gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ -lpthread 

testStateSpace.exe : testStateSpace.o StateSpace.o Board.o Shapes.o gtest.a gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ -lpthread 

Triomino.exe : Triomino.o StateSpace.o Board.o Shapes.o
	$(CXX) $(CXXFLAGS) $^ -o $@

Tetromino.exe : Tetromino.o StateSpace.o Board.o Shapes.o
	$(CXX) $(CXXFLAGS) $^ -o $@
