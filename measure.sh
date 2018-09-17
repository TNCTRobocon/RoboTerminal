#!/bin/bash
# 作者 terurin
# 用途 コンパイル時間の計測
make clean
make pch.hpp.gch

echo "----------------------------">>measure.log
(time make -j) 2>>measure.log 
date>>measure.log 
