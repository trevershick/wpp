default: build

TEST_OPTS := --test_output=all --sandbox_debug --test_verbose_timeout_warnings
TESTS 		:= //...

.PHONY: wp test debug lldb

build:
	bazel build //src:$@

# for mac, we have to preserve the sandbox because the debug symbols are created
# in the sandbox and lldb can't find them if the sandbox disappears
debug:
	bazel build --sandbox_debug -c dbg --strip=never //src:wp

lldb: debug
	lldb --one-line "b main" ./bazel-bin/src/wp -- -s testfiles -r $(PWD)/workspaces.rc

test:
	bazel test $(TEST_OPTS) $(TESTS)

