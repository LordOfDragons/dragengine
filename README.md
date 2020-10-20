# Drag[en]gine Game Engine

![dragengine_logo](https://dragondreams.ch/wp-content/uploads/2020/03/banner_medium.png)

- Website: https://dragondreams.ch/?page_id=152
- IndieDB Engine Profile: https://www.indiedb.com/engines/dragengine
- GameDev Engine Profile: https://www.gamedev.net/manage/projects/356/
- Development Wiki: https://developer.dragondreams.ch/wiki/doku.php
- Source Code Repository: https://github.com/LordOfDragons/dragengine

Example projects can be found on Development Wiki or in this repository:
- Example Projects Repository: https://github.com/LordOfDragons/deexamples

# License Information

The Drag\[en\]gine Game Engine is dual licensed under [L-GPL!](https://www.gnu.org/licenses/lgpl-3.0.html)
(for commercial projects and proprietary use like on consoles) and
[GPL!](https://www.gnu.org/licenses/gpl-3.0.html) (for free game project).
This affects only game engine modules, launchers and IGDE editors.
Game projects do not link against the game engine and thus are not affected by
the license. They can use any license they see fit.

In a nutshell this means: __If you create a game, no matter if commercial or not, you are free to choose your license.__

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
respective with_system_* parameter to 'yes' (to force using the library from host
system and fail if not found) or 'no' (to force compile from in-tree version) or
'auto' (use from system if found otherwise fall back to in-tree version).

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


# Run in "Local Directory Mode"

The Drag\[en\]gine Game Engine, Launchers and the IGDE can be run in the "Local Directory Mode".

The **Live Launcher** is an example of a launcher using this mode to run games from any
directory without installing.

It is possible to run also the IGDE and all launchers in this mode using a source-able file
setting up the required **Environment Variables** with appropriate values. You have to first
prepare a directory with the game engine files. For this download the linux binary distribution
file and unpack it into a directory of your choice. Then copy the **prepare_local_dir_mode.source**
file into the directory. Change into the directory then source this file like this:

    source prepare_local_dir_mode.source

or like this (for consoles not supporting the 'source' builtin command):

    . prepare_local_dir_mode.source

The terminal is now ready to run **delauncher-gui**, **delauncher-console** and **deigde**
binaries from the local directory. All user configuration is also store under the **user**
directory. You can also change the source-able file to choose different directory if needed.
