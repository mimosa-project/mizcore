FROM ubuntu

RUN apt update && apt install -y wget xz-utils git patch python3 libssl-dev lzma-dev libxml2-dev clang cmake flex bison
RUN git clone https://github.com/tpoechtrager/osxcross.git
RUN wget -P /osxcross/tarballs https://github.com/phracker/MacOSX-SDKs/releases/download/11.3/MacOSX11.3.sdk.tar.xz

WORKDIR /osxcross
RUN yes | ./build.sh
ENV PATH $PATH:/osxcross/target/bin
RUN cp /usr/include/FlexLexer.h /osxcross/target/SDK/MacOSX11.3.sdk/usr/include/
