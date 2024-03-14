---
layout: page
title: Windows
permalink: /building/windows/
nav_order: 3
parent: Building
---

# Building Windows

Install Visual Studio Community Edition. Then open the solution file _vs/dragengine.sln_.
You can then launch the build of the entire solution. NuGet pulls the necesdsary
packages. External dependencies are contained inside the source and automatically
unpacked for you.

Only 64-bit Release builds are officially supported.
{: .warning }


# Cross-Compiling

Cross-Compiling is no more officially supported. Build on a Windows machine instead.
{: .warning }

Install MinGW 64-Bit cross-compiler for your system or use a docker image with it.

To build use the same commands as outlined in the [linux build page]({{ site.baseurl }}{% link building/linux.markdown %}
but append _tools=mingw64_ to the command line like this:

```bash
scons tools=mingw64 archive
```

Always use a build target otherwise scons tries to install into your linux system
using windows path names.

The "build_windows.sh" script is a little helper.

To build the windows installers you need a docker image supporting the InnoSetup
compiler. You can build the docker image yourself using the
_windowsSetupCompiler.dockerfile_ docker file:

```bash
docker build --file=windowsSetupCompiler.dockerfile --tag=compile-windows-installer .
```

To build the installer first build the archives using

```bash
scons tools=mingw64 archive
```

Then change into the _installer/windows_ directory and run

```bash
./create_installer.sh
```

The installer is now located inside _installer/windows/build_

It should be possible to build the sources also on a windows machine using Windows
MinGW-64. VisualStudio compile is not supported officially.
