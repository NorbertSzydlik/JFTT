CC    = gcc
CXX   = g++

EXE = compiler

CDEBUG = -g -Wall

CXXDEBUG = -g -Wall

CSTD = -std=c99
CXXSTD = -std=c++14

CFLAGS = -Wno-deprecated-register -O0  $(CDEBUG) $(CSTD)
CXXFLAGS = -O0  $(CXXDEBUG) $(CXXSTD) -v -I. -lcln

MAIN_CPPOBJ = driver Command CommandSkip CommandWhile CommandFor CommandIfElse CommandPut CommandGet CommandDummy CommandAssign Identifier Expression ExpressionNumber ExpressionIdentifier ExpressionOperation InfInt CodeBlock Condition GebalaCompiler
CPPOBJ = main $(MAIN_CPPOBJ)
TESTOBJ = main_test tests/interpreter tests/test_Assignment tests/test_OperatorAdd tests/test_While tests/test_IfElse tests/test_For tests/test_OperatorDiv tests/test_OperatorMul $(MAIN_CPPOBJ)
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
				main_test.o \
				tests/test_Assignment.o tests/test_OperatorAdd.o tests/test_While.o tests/test_IfElse.o tests/test_For.o tests/test_OperatorDiv.o tests/test_OperatorMul.o \
				test\

.PHONY: all
all: wc

wc: $(FILES)
	$(MAKE) $(SOBJ)
	$(MAKE) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS) parser.o lexer.o $(LIBS) -pthread

parser: parser.yy
	bison -d -v parser.yy
	$(CXX) $(CXXFLAGS) -c -o parser.o parser.tab.cc

lexer: lexer.l
	flex --outfile=lexer.yy.cc  $<
	$(CXX)  $(CXXFLAGS) -c lexer.yy.cc -o lexer.o

test: $(TEST_FILES)
	$(MAKE) $(SOBJ)
	$(MAKE) $(TESTOBJS)
	$(CXX) $(CXXFLAGS) -o test $(TESTOBJS) parser.o lexer.o  $(LIBS) -pthread

.PHONY: clean
clean:
	rm -rf $(CLEANLIST)
