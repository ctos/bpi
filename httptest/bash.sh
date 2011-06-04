#!/bin/bash
rm COOKIE/*
make test
./test www.google.com
./test www.baidu.com
./test www.google.com.hk
./test cms.hit.edu.cn





