[![trevershick](https://circleci.com/gh/trevershick/wpp.svg?style=svg)](https://app.circleci.com/pipelines/github/trevershick/wpp?branch=main)
This project is less about the code right now and more about just learning some bazel.


What?
-----
Will return you a string value via stdout based on your current directory (or file if specified)...

There are potentially many uses, but I use this to obtain the specific test target I need to 
execute based on the current file I'm editing in vim.

*My Setup*

`~/bin` is in my path.

`~/workspaces.rc` is as follows:

```
[testfile]
~/workspaces/Components/Barney=TestA
```

I run TestA thru a runner by executing (in Vim):

`!./test-runner.sh -t $(wpp -s testfile -f %)`

which ends up running:

`./test-runner.sh -t TestA` when i'm editing a file in ~/workspaces/Components/Barney/...(anywhere)

The actual command line is a little fictionalized, the actual filenames and paths have been changed to protect the not so innocent.


Building
=====

Requires bazel (sorry).  May require `texinfo` on linux.

Run:

`make`

Yes. Just `make`

Program Usage
=====
```
Usage:
./bazel-bin/src/wp -s <section> [options]

  -h               show list of command-line options
  -d               show debug output
  -g               generate a sample rc file

CONFIG OPTIONS
  -s <section>     the section in which to look
  -r <config file> the RC file to load
  -r               (optional), ~/workspaces.rc is the default
  -f <file/dir>    use this file or directory for lookup
                   (optional), PWD is used by default

EXAMPLE CONFIG FILE (example_config.rc)
[section1]
~/workspaces=value1
~/whatever=value2
[section2]
~=my home

executing the command :
./bazel-bin/src/wp -f example_config.rc -s section1 -f ~/workspaces/something.txt would yield value1
```

