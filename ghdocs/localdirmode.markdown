---
layout: page
title: Local Directory Mode
permalink: /localdirmode/
nav_order: 4
---

# Local Directory Mode

The Drag\[en\]gine Game Engine, Launchers and the IGDE can be run in the _Local Directory Mode_.

The _Live Launcher_ is an example of a launcher using this mode to run games from any
directory without installing.

It is possible to run also the IGDE and all launchers in this mode using a source-able file
setting up the required _Environment Variables_ with appropriate values. You have to first
prepare a directory with the game engine files. For this download the linux binary distribution
file and unpack it into a directory of your choice. Then copy the _prepare_local_dir_mode.source_
file into the directory. Change into the directory then source this file like this:

```bash
source prepare_local_dir_mode.source
```

or like this (for consoles not supporting the 'source' builtin command):

```bash
. prepare_local_dir_mode.source
```

The terminal is now ready to run _delauncher-gui_, _delauncher-console_ and _deigde_
binaries from the local directory. All user configuration is also store under the _user_
directory. You can also change the source-able file to choose different directory if needed.
