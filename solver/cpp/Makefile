CC := clang++
SRCS := main.cpp
CXX_FLAGS := @compile_flags.txt -march=native -mtune=native
BUILD_DIR = build


.PHONY:	debug
debug: BUILD_DIR = build/debug
debug: CXX_FLAGS += -g -Og -fsanitize=address,undefined -fstack-protector-all -ftrapv
debug: build


.PHONY:	release
release: BUILD_DIR = build/release
release: CXX_FLAGS += -Ofast -DNDEBUG -Wno-unused-variable -Wno-unused-function -Wno-unused-const-variable
release: build


EXE_FILE = $(BUILD_DIR)/main


.PHONY:	build
build:	$(SRCS)
	@\mkdir -p $(BUILD_DIR)
	$(CC) $(CXX_FLAGS) $^ -o $(EXE_FILE)


.PHONY:	clean
clean:
	\rm -rf $(BUILD_DIR)
