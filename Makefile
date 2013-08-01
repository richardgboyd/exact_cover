

# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = ../gtest-1.6.0
USER_DIR = .

CXXFLAGS += -I$(GTEST_DIR)/include
CPPFLAGS += -g -Wall -Wextra -DUNIT_TEST

TESTS = testDList testExactCover

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

all : $(TESTS)

clean :
	rm -f $(TESTS) *.o

veryclean :
	rm -f $(TESTS) gtest.a gtest_main.a *.o

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
testDList : testDList.o gtest.a gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ -lpthread 

testExactCover : testExactCover.o exact_cover.o gtest.a gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ -lpthread 

TriominoCover : TriominoCover.o exact_cover.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

TetrominoCover : TetrominoCover.o exact_cover.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

Tetromino5x8Cover : Tetromino5x8Cover.o exact_cover.o Tetromino5x8.data
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

Tetromino5x8.data : 
	./polyimno/Tetromino5x8.exe > $@

