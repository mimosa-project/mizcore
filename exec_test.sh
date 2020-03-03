#!/bin/bash

DIR=`dirname ${0}`

pushd ${DIR}/bin
for file in *_test.out
do
  "./${file}"
done
popd