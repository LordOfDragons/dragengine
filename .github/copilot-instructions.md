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

## Excluded Files

Files matching the following glob pattern must not be changed:
- `src/dragengine/base64/**/*`
- `src/dragengine/src/extern/**/*`
- `src/deigde/editors/project/minizip/**/*`
- `src/modules/archive/delga/minizip/**/*`
- `src/modules/graphic/opengl/src/convexhull3d/**/*`
- `src/modules/graphic/opengl/src/extensions/*gl*.h`
- `src/modules/graphic/opengl/src/extensions/*khr*.h`
- `src/modules/graphic/opengl/squish/**/*`
- `src/modules/physics/bullet/Bullet/**/*`
- `src/modules/physics/bullet/patches/**/*`
- `src/modules/vr/openxr/src/loader/loader_interfaces.h`
- `src/modules/vr/openxr/src/extension/xdev/*`
- `src/shared/vulkan/extern/include/**/*`

## Build System

The project uses **SCons** (Python-based build system) for Linux/cross-platform builds and **Visual Studio** for Windows native builds.

### Building on Linux

To build in AI agent mode on Github requires installing additional packages to build the project. See section "Install required packages" and "Install GLSL Tools" in ".github/workflows/build_linux.yml" for details. Once installed the following build commands can be used:

```bash
# Build entire code base
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
- Only 64-bit Release builds are officially supported (target ReleaseDebug)
- NuGet handles external dependencies automatically

## Coding Standards

### C++ Style

1. **File Headers**: All source files must include the MIT license header:
   ```cpp
   /*
    * MIT License
    *
    * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
    *
    * [Standard MIT license text]
    */
   ```

2. **Include Guards**: Use `#ifndef _CLASSNAME_H_` pattern for header files

3. **Line Breaks**:
   - Max 100 characters per line
   - Keep code readable with appropriate line breaks

4. **Naming Conventions**:
   - Classes: PascalCase with prefix (e.g., `deObjectDebug`, `igdeEnvironment`)
     - `de` prefix for engine core classes
     - `igde` prefix for IGDE classes
   - Private members: `p` prefix (e.g., `pRefCount`, `pLogName`)
   - Protected members: `p` prefix (e.g., `pWidth`, `pHeight`)
   - Methods: PascalCase (e.g., `AddReference`, `FreeReference`)
   - Constants: Not standardized, use context-appropriate style

5. **Indentation**:
   - Use tabs for indentation

6. **Braces, Parenthesis and Square Brackets**:
   - Opening brace on same line for methods, control structures
   - No whitespace around braces except for control structures
   - No whitespace around parenthsis and square brackets

7. **Comments**:
   - Use `//` for single-line comments
   - Use `/* */` for multi-line comments
   - Doxygen-style comments with `\brief` for class/method documentation
   - Group related functions with doxygen-style `\name` blocks
      - Constructors/destructors in group named "Constructors and Destructors"
      - Functions for development purpose only in group named "Internal Use Only"
      - Other methods in group named "Management"

8. **Memory Management**:
   - Classes required to be reference counted have to subclass directly or indirectly `deObject` or `deThreadSafeObject`
      - Destructors must be protected or private to prevent direct deletion
   - Use `A::Ref` smart pointers for reference counted class instances
      - If class is missing public `Ref` using add it in a public section (`using Ref = deTObjectReference<A>;`)
   - For functions creating new reference counted objects, return `A::Ref` smart pointers
   - Create reference counted objects using `A::Ref::New(...)`
   - A reference counted object of type `B::Ref` is automatically casted to `A::Ref` if `B` is a subclass of `A`
   - For collections of reference counted objects use `decTObjectOrderedSet` with `using List = decTObjectOrderedSet<A>;`
   - If class supports `GetName()` use as type `decTCollectionQueryByName<decTObjectOrderedSet<A>,A>` for `using List`
   - If class supports `GetPath()` use as type `decTCollectionQueryByPath<decTObjectOrderedSet<A>,A>` for `using List`
   - If class supports `GetName()` and `GetPath()` use as type `decTCollectionQueryByPathAndName<decTObjectOrderedSet<A>,A>` for `using List`
   - If class supports `GetId()` use as type `decTCollectionQueryById<decTObjectOrderedSet<A>,A>` for `using List`

9. **Member Ordering**:
   - First public declarations like typedefs, enums, inner classes or constants
   - Then private variables
   - Then constructors and destructors
   - Then public methods
   - Then protected declarations like typedefs, enums, inner classes or constants
   - Then protected methods
   - Then protected variables
   - Then private declarations like typedefs, enums, inner classes or constants
   - Then private methods
   - Then private variables
   - Static members before instance members

### Python Style

- Used for build scripts (SCons) and utilities
- Follow PEP 8 conventions
- Python 3.5+ compatibility required

## Platform Support

- **Linux**: Primary development platform, uses GCC 8+
- **Windows**: Visual Studio builds, 64-bit only
- **Android**: Via SCons with platform-specific configuration
- **Web/WebAssembly**: Experimental support

## Haiku-OS (BeOS)

### Porting from FOX-ToolKit to Haiku's native API
- **Prefixing**: Use `Beos` prefix for Haiku based classes where prefix `Fox` is used for FOX based classes.
- **Prefer BLayoutAPI**: Always use `BLayoutBuilder` and `BGroupLayout/BGridLayout`. Do not use manual `BRect` frame coordinates for widget placement unless specifically requested.
- **Window Flags**: Use `B_AUTO_UPDATE_SIZE_LIMITS` for `BWindow` objects to ensure they automatically resize to fit their content.
- **Widget Hierarchy**: Map FOX `FXWindow`-derived classes to Haiku `BView` or its convenience subclasses (e.g., `BGroupView`).
- When refactoring from the FOX backend, identify the FXSEL target/selector patterns and replace them with a BMessage system where the what constant represents the action.
- Explicitly note that in Haiku, calling AddChild() on a BWindow or BView transfers ownership to the parent. The agent must ensure the backend does not double-delete these objects in its own destructor to avoid 'use-after-free' crashes.
- To bridge FOX-style selectors, the Haiku backend must implement a central MessageReceived(BMessage* msg) handler in the BWindow subclass. The agent should map specific BMessage->what codes to the abstract UI layer's dispatch system.
- When the abstract layer calls a method to update a widget (e.g., SetText), the backend must verify it is in the window's thread or use window->Lock() / window->Unlock(). Failures to lock the window before modifying a BView will lead to runtime errors.
- Do not hardcode pixel dimensions for widgets. Use BLayoutBuilder to create flexible grids and groups. This ensures the UI remains usable regardless of the user's system font size—a core requirement of the Haiku User Interface Guidelines.

### Threading and Safety
- **Locker Pattern**: When accessing a `BView` or its properties from a non-window thread, always wrap the call in `window->Lock()` and `window->Unlock()`, or use `BAutolock`.
- **Message Loops**: Ensure the backend correctly implements `MessageReceived(BMessage* msg)` to bridge the Haiku event loop back to the abstract UI layer's dispatchers.

### API and Tooling
- **Native Types**: Use Haiku-specific types for fixed-size integers (e.g., `int32`, `uint32`) rather than standard C++ types for better API compatibility.
- **Reference**: Treat the [Haiku Book](https://api.haiku-os.org) as the primary source for API signatures and documentation.
- **Resource Management**: In Haiku, `BWindow` takes ownership of its child `BView` objects; ensure the backend doesn't double-delete widgets during cleanup.

### Reasoning and Execution Process
Before writing any code for the Haiku-OS backend, the agent MUST follow these steps in order and output its reasoning:

1. **API Mapping Analysis**: Compare the FOX-Toolkit class (reference) with the abstract UI interface. Identify the closest native Be API equivalent (e.g., `FXTextField` -> `BTextControl`).
2. **Threading Audit**: Identify which methods in the abstract interface require `BWindow` locking or thread-safe `BMessenger` calls.
3. **Layout Strategy**: Plan the `BLayoutBuilder` hierarchy. Determine if the FOX packer logic can be replaced by a `BGroupLayout` or `BGridLayout`.
4. **Message Routing**: Define the `uint32` message constants needed to bridge Haiku's `BMessage` system back to the abstract layer's event handlers.
5. **Implementation**: Only after steps 1–4 are validated, generate the C++ header and source files.

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
- Must not commit these files:
   - `compile_commands.json`

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
