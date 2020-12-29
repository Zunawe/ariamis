CXX ?= g++

SRCDIR = src
BUILDDIR = build
TARGET = lib/libariamis.a
SRCEXT := cc
SOURCES = $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.o))
CFLAGS = -Wall -Wextra -std=c++11
LIBS = -lglfw -lGL -ldl -L lib
INCLUDE = -I include

.PHONY: clean test clean_gtest

$(TARGET): $(OBJECTS)
	@mkdir -p lib
	ar rcs $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	@rm -rf $(BUILDDIR)
	@rm -f $(TARGET)
	@rm -rf test/build
	@rm -rf test/bin
	@rm -f example

example: $(TARGET) example.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CFLAGS) $(INCLUDE) -c -o $(BUILDDIR)/$@.o $@.$(SRCEXT)
	$(CXX) $(BUILDDIR)/$@.o $(TARGET) -o $@ $(LIBS)

GTEST_DIR = test/gtest
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
GTEST_OBJECTS = $(GTEST_BUILDDIR)/gtest_main.o $(GTEST_BUILDDIR)/gtest-all.o
GTEST_BUILDDIR = $(GTEST_DIR)/build
GTEST_TARGET = $(GTEST_DIR)/lib/gtest_main.a

$(GTEST_BUILDDIR)/%.o: $(GTEST_DIR)/src/%.cc
	@mkdir -p $(GTEST_DIR)/build
	$(CXX) -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -g -Wall -Wextra -pthread \
	-c $^ -o $@

$(GTEST_TARGET): $(GTEST_OBJECTS)
	@mkdir -p $(GTEST_DIR)/lib
	$(AR) $(ARFLAGS) $@ $^


TEST_TARGET_DIR = test/bin
TESTS = $(patsubst test/%.cc, %, $(shell find test -maxdepth 1 -type f -name "*.$(SRCEXT)"))
TEST_TARGETS = $(patsubst %, $(TEST_TARGET_DIR)/%, $(TESTS))

test/build/%.o: test/%.$(SRCEXT) $(TARGET) $(GTEST_HEADERS)
	@mkdir -p test/build
	$(CXX) -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) $(INCLUDE) $(CFLAGS) -pthread \
		-c $< -o $@

$(TEST_TARGET_DIR)/%: test/build/%.o $(TARGET) $(GTEST_TARGET)
	@mkdir -p test/bin
	$(CXX) -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) $(INCLUDE) $(CFLAGS) -pthread \
		-lpthread $^ -o $@ $(LIBS)

test: $(TEST_TARGETS)
	$(foreach t, $(TEST_TARGETS), ./$(t);)
	@:

clean_gtest:
	@rm -rf $(GTEST_BUILDDIR)
	@rm -f $(GTEST_TARGET)

get-deps:
	sudo apt-get -qq update
	sudo apt-get -qq install xorg-dev
	git clone --depth=1 https://github.com/g-truc/glm.git && mkdir glm_build && cd glm_build && cmake ../glm && sudo make install
	git clone --depth=1 https://github.com/glfw/glfw.git && mkdir glfw_build && cd glfw_build && cmake ../glfw && make -j4 && sudo make install
