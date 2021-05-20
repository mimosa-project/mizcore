#!/bin/bash

SCRIPT_DIR=$(cd $(dirname $0); pwd)
pushd $SCRIPT_DIR/../external
git clone https://github.com/onqtam/doctest.git
git clone https://github.com/Tessil/hat-trie.git
git clone https://github.com/gabime/spdlog.git
popd
