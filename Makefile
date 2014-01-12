#
# Build solution codes and unit test codes by convention:
# * 100(.*).cc are solutions to problem 100.
# * 100(.*)_unittest.cc are unit tests to solutions.
#
# Unit test codes depend on googletest.
#


ifndef GTEST_DIR
$(warning GTEST_DIR is not defined)
endif

ifndef UVA_ROOT
$(error UVA_ROOT is not defined)
endif


SOLUTIONS := $(subst .cc,,$(filter-out %_unittest.cc,$(wildcard *.cc)))
UNITTESTS := $(subst .cc,,$(filter     %_unittest.cc,$(wildcard *.cc)))


CXX := g++
CXXFLAGS := -O2 -pipe -Wall -Wextra -pedantic -Werror -std=c++11 -lm -lcrypt
GTEST_CXXFLAGS := -isystem ${GTEST_DIR}/include ${GTEST_DIR}/src/gtest_main.cc
UNITTEST_CXXFLAGS := $(CXXFLAGS) $(GTEST_CXXFLAGS) -I$(UVA_ROOT)
GTEST_LDFLAGS := -pthread -L${GTEST_DIR} -lgtest


all: $(SOLUTIONS) $(UNITTESTS)


clean:
	rm -f $(SOLUTIONS) $(UNITTESTS)


ifdef GTEST_DIR
$(UNITTESTS) : %_unittest : %_unittest.cc %.cc
	$(CXX) $(UNITTEST_CXXFLAGS) -o $@ $< $(GTEST_LDFLAGS)
endif


$(SOLUTIONS) : % : %.cc
	$(CXX) $(CXXFLAGS) -o $@ $<


.PHONY: all clean
