#!/bin/bash
#Check for local NotazSDL
if [ -e "./SDL/libSDL-1.2.so.0" ]
then
   # This means user provided NotazSDL  
   echo "Using user-provided SDL!"
   export LD_LIBRARY_PATH=./SDL
fi

#Support NotazSDL
# Try and use it anyway!
export SDL_VIDEODRIVER=omapdss
#export SDL_OMAP_VSYNC=1
#export SDL_OMAP_FORCE_DOUBLEBUF=1
./PandoraPanic_R

