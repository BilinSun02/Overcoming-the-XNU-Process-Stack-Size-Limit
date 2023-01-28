#!/bin/bash
#DYLD_LIBRARIES_BACK__PRIVATE="$DYLD_INSERT_LIBRARIES"
#echo "$DYLD_LIBRARIES_BACK__PRIVATE"
DYLD_INSERT_LIBRARIES="$DYLD_INSERT_LIBRARIES:./splitter.dylib"
./exe
#No need to restore the enrionmental variables as they are temporal and only valid during the lifespan of this script.
