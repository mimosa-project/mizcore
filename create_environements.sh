#!/bin/bash

pushd ./common
git clone https://github.com/gabime/spdlog.git
cp -R spdlog/include/spdlog/ ./include/
popd
