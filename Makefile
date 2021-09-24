rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRCS = $(wildcard src/*.c)
HDRS = $(wildcard src/*.h)
TEST_SRCS = $(wildcard test/*.c)
TEST_HDRS = $(wildcard test/*.h)
ALL_SRCS = $(SRCS) $(TEST_SRCS)
ALL_HDRS = $(HDRS) $(TEST_HDRS)
BAZEL_SRCS = $(wildcard *.bazel) $(wildcard src/*.bazel) $(wildcard test/*.bazel)

#$(info "bazel sources is ${BAZEL_SRCS}")

default: all

TEST_OPTS := --test_output=all --sandbox_debug --test_verbose_timeout_warnings
TESTS 		:= //...

.PHONY: wp test debug lldb all format

all: wp test compile_commands.json

format:
	clang-format -i $(ALL_SRCS) $(ALL_HDRS)

bin/bazel-compdb:
	./bin/bootstrap-bazel-compdb

compile_commands.json: bin/bazel-compdb $(BAZEL_SRCS)
	./bin/bazel-compdb

wp:
	bazel build //src:wp

# for mac, we have to preserve the sandbox because the debug symbols are created
# in the sandbox and lldb can't find them if the sandbox disappears
debug:
	bazel build --sandbox_debug -c dbg --strip=never //src:wp

lldb: debug
	lldb --one-line "b main" ./bazel-bin/src/wp -- -s testfiles -r $(PWD)/workspaces.rc

test:
	bazel test $(TEST_OPTS) $(TESTS)

