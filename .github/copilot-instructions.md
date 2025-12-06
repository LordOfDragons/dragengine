# Copilot Instructions for Drag[en]gine Game Engine

## Project Overview

Drag[en]gine is a modular game engine written primarily in C++ with Python build scripts. The engine follows a non-blackbox principle where game developers don't need to compile or link against the engine. The project is licensed under MIT (optionally L-GPL).

## Project Structure

- `src/dragengine/` - Core game engine code
- `src/modules/` - Engine modules (graphics, audio, physics, etc.)
- `src/deigde/` - Integrated Game Development Environment (IGDE)
- `src/launcher/` - Game launchers
- `src/tests/` - Test suite
- `extern/` - External dependencies (in-tree builds)
- `vs/` - Visual Studio solution files for Windows builds
- `ghdocs/` - GitHub Pages documentation

## Build System

The project uses **SCons** (Python-based build system) for Linux/cross-platform builds and **Visual Studio** for Windows native builds.

### Building on Linux

```bash
# Basic build
scons

# With custom options
scons with_debug=yes with_system_openal=no

# Build and create archives
scons archive

# See all build options
scons -h
```

### Build Parameters

- Use `custom.py` for build customization (copy from `dist.custom.py` or platform-specific variants)
- `prefix` - Installation prefix (default: `/usr`)
- `with_debug` - Enable debug build
- `with_system_*` - Control system vs in-tree library usage (`yes`, `no`, `auto`)

### Building on Windows

- Open `vs/dragengine.sln` in Visual Studio
- Only 64-bit Release builds are officially supported
- NuGet handles external dependencies automatically

## Coding Standards

### C++ Style

1. **File Headers**: All source files must include the MIT license header:
   ```cpp
   /*
    * MIT License
    *
    * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
    *
    * [Standard MIT license text]
    */
   ```

2. **Include Guards**: Use `#ifndef _CLASSNAME_H_` pattern for header files

3. **Naming Conventions**:
   - Classes: PascalCase with prefix (e.g., `deObjectDebug`, `igdeEnvironment`)
     - `de` prefix for engine core classes
     - `igde` prefix for IGDE classes
   - Private members: `p` prefix (e.g., `pRefCount`, `pLogName`)
   - Methods: PascalCase (e.g., `AddReference`, `FreeReference`)
   - Constants: Not standardized, use context-appropriate style

4. **Indentation**: Use tabs for indentation

5. **Braces**: Opening brace on same line for methods, control structures

6. **Comments**:
   - Use `//` for single-line comments
   - Use `/* */` for multi-line comments
   - Doxygen-style comments with `\brief` for class/method documentation

7. **Memory Management**:
   - Reference counting is used extensively
   - Implement `AddReference()` and `FreeReference()` methods where appropriate
   - Delete objects when reference count reaches zero

### Python Style

- Used for build scripts (SCons) and utilities
- Follow PEP 8 conventions
- Python 3.5+ compatibility required

## Platform Support

- **Linux**: Primary development platform, uses GCC 8+
- **Windows**: Visual Studio builds, 64-bit only
- **Android**: Via SCons with platform-specific configuration
- **Web/WebAssembly**: Experimental support

## Dependencies

The project can use both system libraries and in-tree builds from `extern/`:

### Build Tools Required
- SCons 3+
- Python 3.5+
- GCC 8+ (Linux) or Visual Studio (Windows)
- CMake, autoconf, libtool, gperf, nasm, patchelf, glslang-tools

### Optional System Libraries
- libdscript (DragonScript scripting engine)
- X11, XRandR, OpenGL, Xi (Linux graphics)
- PulseAudio, ALSA, PortAudio (audio)
- OpenAL, Ogg, Vorbis, Theora (media)
- HIDAPI, OpenHMD, evdev (input devices)
- And many more (see README.md for full list)

## Git Workflow

- **master** - Development branch (potentially unstable)
- **stable** - Latest stable release branch
- Other branches are internal feature/platform branches

## Testing

- Tests are located in `src/tests/`
- Use SCons to build and run tests
- Maintain compatibility with existing test infrastructure

## Documentation

- GitHub Pages documentation in `ghdocs/`
- API documentation generated from code
- Development Wiki: https://developer.dragondreams.ch/wiki/doku.php

## Important Notes

1. **Non-Blackbox Principle**: Game developers don't compile/link against the engine
2. **License**: MIT license applies - games can use any license
3. **Modular Architecture**: Engine functionality is provided through loadable modules
4. **Cross-Platform**: Code should be portable across Linux, Windows, Android, and Web
5. **Backward Compatibility**: Maintain API compatibility when possible

## When Making Changes

1. Respect existing code style and conventions
2. Add appropriate license headers to new files
3. Test on Linux build system (primary platform)
4. Consider cross-platform implications
5. Update documentation if adding new features
6. Don't break the modular architecture
7. Keep game engine and IGDE code separate where appropriate

## Common Patterns

- Reference counting for memory management
- Use of prefixes for class organization (`de`, `igde`, etc.)
- Extensive use of inheritance and polymorphism
- Module system for extensibility
- Exception handling with custom exception classes

## Resources

- Repository: https://github.com/LordOfDragons/dragengine
- Website: https://dragondreams.ch/?page_id=152
- Documentation: https://lordofdragons.github.io/dragengine
- Examples: https://github.com/LordOfDragons/deexamples
