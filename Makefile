default: wp


.PHONY: wp test
wp:
	bazel build //src:$@

test:
	bazel run //test:tests

