---
layout: page
title: Linux
permalink: /building/linux/
nav_order: 3
parent: Building
---

# Building Linux

You do __NOT__ have to build the game engine to play or create games.
Building is only required for Linux distribution maintainers or those
having a special use case.
{: .important }

To customize build either copy "custom.py.dist" to "custom.py" and edit or add the
parameters in custom.py.dist as command line arguments. For example:

```bash
scons with_debug=yes with_system_openal=no
```

To see the list of supported build targets run

```bash
scons -h
```

To compile and install run

```bash
scons
```

By default the build tries to find as many dependency libraries as possiible in
the host system. Not found libraries are compiled from in-tree sourced under "extern".
You can force building certain libraries either in-tree or from system by setting the
respective with_system_* parameter to 'yes' (to force using the library from host
system and fail if not found) or 'no' (to force compile from in-tree version) or
'auto' (use from system if found otherwise fall back to in-tree version).

The default installation prefix is "/usr". To change the installation prefix use
"custom.py" or use

```bash
scons prefix=/usr/local
```

To build distribution archives run

```bash
scons archive
```

The archives will be located under "archive/build"

To build installers run

```bash
scons installer
```

The installers will be located under "installer/build"

In general all compiling takes place inside "build" directories underneath the
respective SConscript files. To make a full clean you can delete all directories
named "build" safely

Every target can be cleared by appending "-c" to the command line. For example
to clear build files run

```bash
scons -c
```

To clear only archive files run

```bash
scons -c archive
```

Clearing only removes files not directories. This is the way SCons behaves.

When running SCons as user it is recommended to use "prefix" parameter to define
the installation directory otherwise installing files fails with permission
denied errors. It is not recommended to run SCons as root unless you intent to
install files directly into the system.

Package maintainers requiring sandboxing to build packages can use the SCons
sandbox support to place installed files in a different location than the
"prefix" parameter defines like this:

```bash
scons --install-sandbox=path/to/sandbox
```

See the __debian__ branch for an example of how this is used.
