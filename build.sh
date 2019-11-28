#!/bin/bash

cd $(dirname "$0")
g++ src/*.cpp -o build/snake -Wall -Wextra

