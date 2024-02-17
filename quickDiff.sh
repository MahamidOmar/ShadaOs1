#! /bin/bash

nvim -d ./tests/outputs/test_$1.out.processed ./tests/expected/test_$1.out.exp
