FROM mstorsjo/llvm-mingw

RUN apt update && apt install -y flex bison

RUN cp /usr/include/FlexLexer.h /opt/llvm-mingw/x86_64-w64-mingw32/include/
RUN mkdir -p /mizcore/build
