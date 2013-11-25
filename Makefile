#
# Build solution codes and unit test codes by convention:
# * 100(.*).cc are solutions to problem 100.
# * 100(.*)_unittest.cc are unit tests to solutions.
#
# Unit test codes depend on googletest.
#


ifndef GTEST_DIR
$(error GTEST_DIR is not defined)
endif


SOLUTIONS := $(subst .cc,,$(filter-out %_unittest.cc,$(wildcard *.cc)))
UNITTESTS := $(subst .cc,,$(filter     %_unittest.cc,$(wildcard *.cc)))


CXX := g++
CXXFLAGS := -lm -lcrypt -O2 -pipe -Wall -Werror
GTEST_CXXFLAGS := -isystem ${GTEST_DIR}/include ${GTEST_DIR}/src/gtest_main.cc
UNITTEST_CXXFLAGS := $(CXXFLAGS) $(GTEST_CXXFLAGS) -I$(UVA_ROOT)
GTEST_LDFLAGS := -pthread -L${GTEST_DIR} -lgtest


all: $(SOLUTIONS) $(UNITTESTS)


clean:
	rm -f $(SOLUTIONS) $(UNITTESTS)


$(UNITTESTS) : %_unittest : %_unittest.cc %.cc
	$(CXX) $(UNITTEST_CXXFLAGS) -o $@ $< $(GTEST_LDFLAGS)


$(SOLUTIONS) : % : %.cc
	$(CXX) $(CXXFLAGS) -DONLINE_JUDGE -o $@ $<


.PHONY: all clean
