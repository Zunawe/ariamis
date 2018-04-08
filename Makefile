SRCDIR = src
BUILDDIR = build
TARGET = lib/libariamis.a
SRCEXT := cc
SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.o))
CFLAGS = `pkg-config --cflags glfw3` -Wall -Wextra -std=c++11
LIBS = `pkg-config --static --libs glfw3` -lGL -L lib
INCLUDE = -I include

$(TARGET): $(OBJECTS)
	@mkdir -p lib
	ar rcs $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	g++ $(CFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	@rm -rf $(BUILDDIR)
	@rm -r $(TARGET)
	@rm -rf test/build
	@rm -rf test/bin


GTEST_DIR = test/gtest
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
GTEST_OBJECTS = $(GTEST_BUILDDIR)/gtest_main.o $(GTEST_BUILDDIR)/gtest-all.o
GTEST_BUILDDIR = $(GTEST_DIR)/build
GTEST_TARGET = $(GTEST_DIR)/lib/gtest_main.a

$(GTEST_BUILDDIR)/%.o: $(GTEST_DIR)/src/%.cc
	@mkdir -p $(GTEST_DIR)/build
	g++ -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -g -Wall -Wextra -pthread \
	-c $^ -o $@

$(GTEST_TARGET): $(GTEST_OBJECTS)
	@mkdir -p $(GTEST_DIR)/lib
	$(AR) $(ARFLAGS) $@ $^


TEST_TARGET_DIR = test/bin
TESTS = $(patsubst test/%.cc, %, $(shell find test -maxdepth 1 -type f -name *.$(SRCEXT)))
TEST_TARGETS = $(patsubst %, $(TEST_TARGET_DIR)/%, $(TESTS))

test/build/%.o: test/%.cc $(SRCDIR)/*.$(SRCEXT) $(GTEST_HEADERS)
	@mkdir -p test/build
	g++ -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) $(INCLUDE) $(CFLAGS) -pthread \
		-c $< -o $@

$(TEST_TARGET_DIR)/%: test/build/%.o $(TARGET) $(GTEST_TARGET)
	@mkdir -p test/bin
	g++ -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) $(INCLUDE) $(CFLAGS) -pthread \
		-lpthread $^ -o $@ $(LIBS)

test: $(TEST_TARGETS)
	$(foreach t, $(TEST_TARGETS), ./$(t);)
	@:

clean_gtest:
	@rm -rf $(GTEST_BUILDDIR)
	@rm -f $(GTEST_TARGET)
