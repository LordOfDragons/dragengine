# Drag[en]gine Game Engine

![dragengine_logo](https://dragondreams.ch/wp-content/uploads/2020/03/banner_medium.png)

- Website: https://dragondreams.ch/?page_id=152
- IndieDB Engine Profile: https://www.indiedb.com/engines/dragengine
- GameDev Engine Profile: https://www.gamedev.net/manage/projects/356/
- Development Wiki: https://developer.dragondreams.ch/wiki/doku.php
- Source Code Repository: https://github.com/LordOfDragons/dragengine

Example projects can be found on Development Wiki or in this repository:
- Example Projects Repository: https://github.com/LordOfDragons/deexamples

# Building Linux

To customize build either copy "custom.py.dist" to "custom.py" and edit or add the
parameters in custom.py.dist as command line arguments. For example:
    scons with_debug=yes with_system_openal=no


To see the list of supported build targets run

    scons -h

To compile and install run

    scons

By default the build tries to find as many dependency libraries as possiible in
the host system. Not found libraries are compiled from in-tree sourced under "extern".
You can force building certain libraries either in-tree or from system by setting the
respective "with_system_*" parameter to 'yes' (to force using the library from host
system and fail if not found) or 'no' (to force compile from in-tree version).

To build distribution archives run

    scons archive

The archives will be located under "archive/build"

To build installers run

    scons installer

The installers will be located under "installer/build"

In general all compiling takes place inside "build" directories underneath the
respective SConscript files. To make a full clean you can delete all directories
named "build" safely


# Build Dependencies

- SCons 2.5.1+
- Python 2.7+ or 3.5+
- GCC 8+
- CMake (for in-tree building of external software)
- libtool (for in-tree building of external software)
- gperf (for in-tree building of external software)


# Building Windows

Install MinGW 64-Bit cross-compiler for your system or use a docker image with it.

To build use the same commands as under the "Building Linux" section but append
"tools=mingw64" to the command line like this:

    scons tools=mingw64 archive

Always use a build target otherwise scons tries to install into your linux system
using windows path names.

The "build_windows.sh" script is a little helper.

To build the windows installers you need a docker image supporting the InnoSetup
compiler. You can build the docker image yourself using the
"windowsSetupCompiler.dockerfile" docker file:

    docker build --file=windowsSetupCompiler.dockerfile --tag=compile-windows-installer .

To build the installer first build the archives using

    scons tools=mingw64 archive

Then change into the "installer/windows" directory and run

    ./create_installer.sh

The installer is now located inside "installer/windows/build"

It should be possible to build the sources also on a windows machine using Windows
MinGW-64. VisualStudio compile is not supported officially.
