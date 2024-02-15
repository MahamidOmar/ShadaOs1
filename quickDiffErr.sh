#! /bin/bash

nvim -d ./tests/outputs/test_$1.err.processed ./tests/expected/test_$1.err.exp
