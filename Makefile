CC    = clang
CXX   = clang++

EXE = compiler

CDEBUG = -g -Wall

CXXDEBUG = -g -Wall

CSTD = -std=c99
CXXSTD = -std=c++14

CFLAGS = -Wno-deprecated-register -O0  $(CDEBUG) $(CSTD) 
CXXFLAGS = -Wno-deprecated-register -O0  $(CXXDEBUG) $(CXXSTD) -v -I./gtest-1.7.0/include -pthread

MAIN_CPPOBJ = driver Command CommandWhile CommandFor CommandIfElse CommandPut CommandGet CommandDummy CommandAssign Identifier Expression ExpressionNumber ExpressionIdentifier ExpressionOperation InfInt CodeBlock Condition GebalaCompiler
CPPOBJ = main $(MAIN_CPPOBJ)
TESTOBJ = main_test $(MAIN_CPPOBJ)
SOBJ =  parser lexer

FILES = $(addsuffix .cpp, $(CPPOBJ))
TEST_FILES = $(addsuffix .cpp, $(TESTOBJ))

OBJS  = $(addsuffix .o, $(CPPOBJ))
TESTOBJS = $(addsuffix .o, $(TESTOBJ))

CLEANLIST =  $(addsuffix .o, $(OBJ)) $(OBJS) \
				 parser.tab.cc parser.tab.hh \
				 location.hh position.hh \
			    stack.hh parser.output parser.o \
				 lexer.o lexer.yy.cc $(EXE) \
				main_test.o test\

.PHONY: all
all: wc

wc: $(FILES)
	$(MAKE) $(SOBJ)
	$(MAKE) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS) parser.o lexer.o $(LIBS)


parser: parser.yy
	bison -d -v parser.yy
	$(CXX) $(CXXFLAGS) -c -o parser.o parser.tab.cc

lexer: lexer.l
	flex --outfile=lexer.yy.cc  $<
	$(CXX)  $(CXXFLAGS) -c lexer.yy.cc -o lexer.o

test: $(TEST_FILES)
	$(MAKE) $(SOBJ)
	$(MAKE) $(TESTOBJS)
	$(CXX) $(CXXFLAGS) -o test $(TESTOBJS) parser.o lexer.o -I./gtest-1.7.0/include -L./gtest-1.7.0/lib/.libs -lgtest $(LIBS)

.PHONY: clean
clean:
	rm -rf $(CLEANLIST)

