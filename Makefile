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
PRESUBMIT := $(addsuffix _presubmit,$(SOLUTIONS))


CXX := g++
CXXFLAGS := -O2 -pipe -Wall -Wextra -pedantic -Werror -std=c++11 -lm -lcrypt
GTEST_CXXFLAGS := -isystem ${GTEST_DIR}/include ${GTEST_DIR}/src/gtest_main.cc
UNITTEST_CXXFLAGS := $(CXXFLAGS) $(GTEST_CXXFLAGS) -I$(UVA_ROOT)
GTEST_LDFLAGS := -pthread -L${GTEST_DIR} -lgtest


all: $(SOLUTIONS)


unittests: $(UNITTESTS)


presubmit: $(PRESUBMIT)


clean:
	rm -f $(SOLUTIONS) $(UNITTESTS) $(PRESUBMIT)


ifdef GTEST_DIR
$(UNITTESTS) : %_unittest : %_unittest.cc %.cc
	$(CXX) $(UNITTEST_CXXFLAGS) -o $@ $< $(GTEST_LDFLAGS)
endif


$(SOLUTIONS) : % : %.cc
	$(CXX) $(CXXFLAGS) -g -o $@ $<

$(PRESUBMIT) : %_presubmit : %.cc
	$(CXX) $(CXXFLAGS) -DONLINE_JUDGE -o $@ $<


.PHONY: all unittests presubmit clean
