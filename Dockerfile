FROM arm64v8/ubuntu

RUN apt update -y && apt full-upgrade -y
RUN apt install -y curl 
RUN DEBIAN_FRONTEND="noninteractive" apt install -y tzdata
RUN apt install -y make texinfo build-essential autoconf git cmake pkg-config file zip
ADD https://releases.bazel.build/4.2.1/release/bazel-4.2.1-linux-arm64 /root
RUN chmod a+x /root/bazel-4.2.1-linux-arm64
RUN ln /root/bazel-4.2.1-linux-arm64 /usr/local/bin/bazel
