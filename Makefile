TARGET_EXEC := experiment

BUILD_DIR := ./build
SRC_DIRS := ./src
TEST_DIRS := ./test

ifeq ($(OS),Windows_NT)
  SHELL := powershell.exe
  .SHELLFLAGS := -NoProfile -Command
  TEST_PATH_CMD := test-path
  AND := -and
  OR := -or
else
  TEST_PATH_CMD := test
  AND := &&
  OR := ||
endif

# # Find all the C and C++ files we want to compile
# # Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
# ifeq ($(OS), Windows_NT)
#   command = Get-ChildItem -Path ('./src', './test') -Recurse -Include ('*.cpp', '*.c', '*.s') | Resolve-Path -Relative
#   # $(info $(command))
#   SRCS := $(shell $(command))
#   # $(info $(SRCS))

# else
#   SRCS := $(shell find $(SRC_DIRS) -name "*.cpp" -or -name '*.c' -or -name '*.s')
# endif

# # Prepends BUILD_DIR and appends .o to every src file
# # As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o
# OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# # String substitution (suffix version without %).
# # As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
# DEPS := $(OBJS:.o=.d)

# # Every folder in ./src will need to be passed to GCC so that it can find header files
# ifeq ($(OS), Windows_NT)
#   command = Get-ChildItem $(SRC_DIRS) -Directory -Recurse | Resolve-Path -Relative
#   # $(info $(command))
#   INC_DIRS := $(shell $(command))
#   # $(info $(INC_DIRS))
# else
#   INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# endif

# # Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
# INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP -O3


# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	($(TEST_PATH_CMD) $(dir $@)) $(OR) (mkdir -p $(dir $@))
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	($(TEST_PATH_CMD) $(dir $@)) $(OR) (mkdir -p $(dir $@))
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


pre_test_search: 
OBJS := $(BUILD_DIR)/$(TEST_DIRS)/test_search.cpp.o \
$(BUILD_DIR)/$(SRC_DIRS)/r_tree.cpp.o \
$(BUILD_DIR)/$(SRC_DIRS)/search.cpp.o
TARGET_EXEC:=$(TEST_DIRS)/test_search
test_search: pre_test_search $(BUILD_DIR)/$(TARGET_EXEC);

pre_test_tree_cons:;
OBJS := $(BUILD_DIR)/$(TEST_DIRS)/test_tree_construction.cpp.o \
$(BUILD_DIR)/$(SRC_DIRS)/r_tree.cpp.o \
$(BUILD_DIR)/$(SRC_DIRS)/nearest_x.cpp.o \
$(BUILD_DIR)/$(SRC_DIRS)/exec.cpp.o \
$(BUILD_DIR)/$(SRC_DIRS)/sort_tile_recursive.cpp.o \
$(BUILD_DIR)/$(SRC_DIRS)/hilbert.cpp.o
TARGET_EXEC:=$(TEST_DIRS)/test_tree_cons
test_tree_cons: pre_test_tree_cons $(BUILD_DIR)/$(TARGET_EXEC);

$(BUILD_DIR)/$(TEST_DIRS)/test%: $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
# -include $(DEPS)