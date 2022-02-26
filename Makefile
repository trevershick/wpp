#rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
#SRCS = $(wildcard src/*.c)
#HDRS = $(wildcard src/*.h)
#$(info "bazel sources is ${BAZEL_SRCS}")

default: cmake

.PHONY: all format cmake install

all: clean format cmake compile_commands.json

clean:
	rm -rf build
	rm -f compile_commands.json

build:
	cmake -B build .

cmake: build
	cmake --build build

install:
	cmake --build build -t install

format:
	clang-format -i \
		src/lib/*.h \
		src/lib/*.c \
		src/cmd/*.c \
		test/*.h \
		test/*.c
	cmake-format -i CMakeLists.txt

build/compile_commands.json: build cmake

compile_commands.json: build/compile_commands.json
	ln -sf $@ $<
# for mac, we have to preserve the sandbox because the debug symbols are created
# in the sandbox and lldb can't find them if the sandbox disappears
#debug:
#	bazel build --sandbox_debug -c dbg --strip=never //src:wp
#
#lldb: debug
#	lldb --one-line "b main" ./bazel-bin/src/wp -- -s testfiles -r $(PWD)/workspaces.rc
#
test:
	cmake --build build -t

docker_build:
	docker bulid -t docker_4_2_1 .

