# Drag[en]gine Game Engine

![dragengine_logo](https://dragondreams.ch/wp-content/uploads/2020/03/banner_medium.png)

- Website: https://dragondreams.ch/?page_id=152
- Documentation (GitHub Pages): https://lordofdragons.github.io/dragengine
- Development Wiki: https://developer.dragondreams.ch/wiki/doku.php
- IndieDB Engine Profile: https://www.indiedb.com/engines/dragengine
- GameDev Engine Profile: https://www.gamedev.net/manage/projects/356/
- Source Code Repository: https://github.com/LordOfDragons/dragengine

Example projects can be found on Development Wiki or in this repository:
- Example Projects Repository: https://github.com/LordOfDragons/deexamples

[![Nightly](https://github.com/LordOfDragons/dragengine/actions/workflows/nightly.yml/badge.svg)](https://github.com/LordOfDragons/dragengine/actions/workflows/nightly.yml)

# Game developer information

The Drag[en]gine is based on a non-blackbox principle. In particular this means
__for developing games using the Drag[en]gine you do not need to compile the game__
__engine nor do you have to link against it__. If you just want to develop games
using Drag[en]gine installing the pre-compiled release files is all you need.

The source code repository is interesting only for people interested in the
inner workings and those wanting to create engine modules or editor modules.

# License Information

The Drag\[en\]gine Game Engine is licensed under [MIT](https://choosealicense.com/licenses/mit)
for clarity. Optionally the [L-GPL](https://www.gnu.org/licenses/lgpl-3.0.html)
can be used. This affects only game engine modules, launchers and IGDE editors.
Game projects do not link against the game engine and thus are not affected by
the license. They can use any license they see fit.

In a nutshell this means: __If you create a game, no matter if commercial or not__
__, you do not have to worry about licensing at all.__

# GIT Branches

The __master__ branch is the current development branch with the most recent work.
It is potentially unstable and should be only used by developers working on
bleeding edge software build against the upcoming release.

The __stable__ branch is the stable branch pointing to the latest release. This
branch is for public use by distribution builders and developers working on
software to work with the latest stable release.

All other branches are internal feature development and platform support branches
not intended for public use. If you want to work on these branches please get in
contact with the development team first.

# Building Linux

To customize build either copy "custom.py.dist" to "custom.py" and edit or add the
parameters in custom.py.dist as command line arguments. For example:

    scons with_debug=yes with_system_openal=no


To see the list of supported build targets run

    scons -h

To compile and install run

    scons

By default the build tries to find as many dependency libraries as possible in
the host system. Not found libraries are compiled from in-tree sourced under "extern".
You can force building certain libraries either in-tree or from system by setting the
respective with_system_* parameter to 'yes' (to force using the library from host
system and fail if not found) or 'no' (to force compile from in-tree version) or
'auto' (use from system if found otherwise fall back to in-tree version).

The default installation prefix is "/usr". To change the installation prefix use
"custom.py" or use

    scons prefix=/usr/local

To build distribution archives run

    scons archive

The archives will be located under "archive/build"

To build installers run

    scons installer

The installers will be located under "installer/build"

In general all compiling takes place inside "build" directories underneath the
respective SConscript files. To make a full clean you can delete all directories
named "build" safely

Every target can be cleared by appending "-c" to the command line. For example
to clear build files run

    scons -c

To clear only archive files run

    scons -c archive

Clearing only removes files not directories. Clearing clears also all direct and
indirect dependencies of the targets. This is the way SCons behaves.

When running SCons as user it is recommended to use "prefix" parameter to define
the installation directory otherwise installing files fails with permission
denied errors. It is not recommended to run SCons as root unless you intent to
install files directly into the system.

Package maintainers requiring sandboxing to build packages can use the SCons
sandbox support to place installed files in a different location than the
"prefix" parameter defines like this:

    scons --install-sandbox=path/to/sandbox

See the __debian__ branch for an example of how this is used.


# Build Dependencies

These dependencies are required to build all targets:

- Git Lfs
- SCons 3+
- Python 3.5+
- GCC 8+
- CMake
- autoconf
- libtool
- gperf
- nasm
- patchelf
- glslang-tools

These dependencies (headers and link libraries, aka development files) allow using
system libraries. If missing in-tree builds are used:
- libdscript: https://github.com/LordOfDragons/dragonscript
- libx11
- libxrandr
- libgl
- libxi
- libpulse
- libasound2
- portaudio19
- libxft
- libjpeg
- libopenal
- libogg
- libvorbis
- libtheora
- libhidapi
- libopenhmd
- libevdev
- libsoundtouch
- libwebp
- libwebm
- libvpx
- libopenvr
- libfreetype

Under Debian based distros (Ubuntu for example) using this command line installs all
packages required to build the game engine using as many system libraries as possible:

    apt install scons libdscript-dev cmake autoconf libtool nasm libx11-dev \
        libxrandr-dev libgl-dev libxi-dev libpulse-dev libasound2-dev \
        portaudio19-dev, libxft-dev, libjpeg-dev, libopenal-dev libogg-dev \
        libvorbis-dev libtheora-dev libhidapi-dev libopenhmd-dev patchelf \
        libevdev-dev libsoundtouch-dev libwebp-dev libwebm-dev libvpx-dev \
        libopenvr-dev glslang-tools libfreetype-dev

For Arch based distros using this command line installs all packages required to build
the game engine using as many system libraries as possible:

    pacman -S gcc scons cmake autoconf libtool nasm libx11 libxrandr libglvnd \
        libxi libxft libjpeg openal libogg libvorbis libtheora hidapi patchelf \
        libevdev soundtouch libwebp libvpx glslang freetype2 patch automake \
        make libpng pkgconfig

# Building Windows

Install Visual Studio Community Edition. Then open the solution file "vs/dragengine.sln".
You can then launch the build of the entire solution. NuGet pulls the necesdsary
packages. External dependencies are contained inside the source and automatically
unpacked for you.

Only 64-bit Release builds are officially supported.

Cross-Compiling using SCons is no more officially supported. Build on a Windows machine instead.
See https://lordofdragons.github.io/dragengine/building/windows for cross-compiling.

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
