# 
# Example build configuration.
# 
# To use rename to "custom.py" and uncomment the desired parameters.
# 
# Most parameters in here are tri-state and accept the values 'auto', 'yes' and 'no'.
# Since this is a python file after all you can also use the values True and False
# instead of 'yes' and 'no' as well as all kinds of code you can use in python for
# example to adjust configuration on external parameters.
#


# General configuration
#########################

# Build engine tests. Builds "detests" binary.
# 
# Possible values: 'yes', 'no'
# 
# with_tests = 'no'

# Build with debug symbols for GDB usage.
# 
# Possible values: 'yes', 'no'
# 
# with_debug = 'no'

# Treat warnings as errors. Used only if with_debug is 'yes'.
# 
# Possible values: 'yes', 'no'
# 
# with_warnerrors = 'no'

# Enable sanitizing building. Used only if with_debug is 'yes'. Requires a GCC or LLVM
# compiler supporting sanitizing as described in
# https://github.com/google/sanitizers/wiki/AddressSanitizer .
# 
# Possible values: 'yes', 'no'
# 
# with_sanitize = 'no'

# Enable thread sanitizing. Used only if with_debug is 'yes'. Requires a GCC or LLVM
# compiler supporting sanitizing as described in
# https://github.com/google/sanitizers/wiki/AddressSanitizer .
# Can not be used together with with_sanitize as they conflict.
# 
# Possible values: 'yes', 'no'
# 
# with_sanitize_thread = 'no'

# Verbose compilation showing command lines.
# 
# Possible values: 'yes', 'no'
# 
# with_verbose = 'no'

# Force version. Empty to disable. Used only for nightly builds.
# If you are not "Jenkins" this option is not for you.
# 
# Value type: string
# 
# force_version = ''



# Controlling In-Tree Building of External Software
#####################################################
# 
# Various parts in the game engine and modules require external libraries to be present
# to build and run. If the required libraries are not present on the host system they
# can be build automatically from in-tree copies of the library source code. This is
# the default mode and is activated if the parameter value is 'auto'.
# 
# For self-sustained builds all external libraries have to be build from in-tree sources
# to ensure maximum compatibility. To force building from in-tree source code set the
# desired parameters to 'no'.
# 
# For creating Linux Distro builds with maximum re-use of code no in-tree sources should
# be used. To force never using in-tree source code set the desired parameters to 'yes'.
# 
# Possible values:
#   'auto'
#      Use the library installed on the system if found. Otherwise build from internal sources.
#   
#   'yes'
#      Force using the library installed on the system. Building from internal sources is not
#      allowed. If the library is not installed on the system the build will be aborted.
#   
#   'no'
#      The library installed on the system is not used if found. The library is always build
#      from the internal sources. This is best used for cross-compiling and building
#      distribution archives for foreign system where installed libraries can not be predicted.
# 
# Default value for all parameters in this section: 'auto'

# with_system_zlib = 'auto'
#   Use System Zlib
# 
# with_system_libpng = 'auto'
#   Use System libpng
# 
# with_system_sndio = 'auto'
#   Use System sndio
# 
# with_system_libapng = 'auto'
#   Use System libapng
# 
# with_system_libjpeg = 'auto'
#   Use System JPEG
# 
# with_system_openal = 'auto'
#   Use System OpenAL
# 
# with_system_libogg = 'auto'
#   Use System libogg
# 
# with_system_libvorbis = 'auto'
#   Use System libvorbis
# 
# with_system_libtheora = 'auto'
#   Use System libtheora
# 
# with_system_fox = 'auto'
#   Use System FOX Toolkit
# 
# with_system_dragonscript = 'auto'
#   Use System DragonScript
# 
# with_system_libffi = 'auto'
#   Use System libffi
# 
# with_system_libltdl = 'auto'
#   Use System libltdl
# 
# with_system_libsigsegv = 'auto'
#   Use System libsigsegv
# 
# with_system_smalltalk = 'auto'
#   Use System Smalltalk
# 
# with_system_libevdev = 'auto'
#   Use System libevdev
# 
# with_system_libusb = 'auto'
#   Use System libusb
# 
# with_system_libhidapi = 'auto'
#   Use System libhidapi
# 
# with_system_libopenhmd = 'auto'
#   Use System libopenhmd
# 
# with_system_fftw = 'auto'
#   Use System fftw

# When compiling OpenAL what backends are required.
# 
# Possible values: list of: 'alsa', 'pulseaudio', 'portaudio', 'oss'
# 
# with_openal_backends = ['alsa', 'pulseaudio', 'portaudio', 'oss']

# Path to DragonScript include files or empty to use system default
# 
# Value type: string with directory path
# 
# with_dragonscript_inc = ''

# Path to DragonScript library files or empty to use system default
# 
# Value type: string with directory path
# 
# with_dragonscript_lib = ''



# Module Configuration
########################
# 
# Determines which parts of the game engine are build.
# 
# Possible values: 'yes', 'no', 'auto'

# build_audio_openal = 'auto'
#   Build OpenAL Audio Module
# 
# build_cr_basic = 'auto'
#   Build Basic Crash-Recovery Module'
# 
# build_graphics_opengl = 'auto'
#   Build OpenGL Graphics Module
# 
# build_image_png = 'auto'
#   Build PNG Image Module
# 
# build_image_png3d = 'auto'
#   Build PNG-3D Image Module
# 
# build_image_jpeg = 'auto'
#   Build JPEG Image Module
# 
# build_input_x = 'auto'
#   Build X Input Module
# 
# build_input_w32 = 'auto'
#   Build Windows Input Module
# 
# build_input_beos = 'auto'
#   Build BeOS Input Module
# 
# build_input_macos = 'auto'
#   Build MacOS Input Module
# 
# build_input_android = 'auto'
#   Build Android Input Module
# 
# build_physics_bullet = 'auto'
#   Build Bullet Physics Module
# 
# build_script_ds = 'auto'
#   Build DragonScript Script Module
# 
# build_script_python = 'auto'
#   Build Python Script Module
# 
# build_script_smalltalk = 'auto'
#   Build Smalltalk Script Module
# 
# build_sound_ogg = 'auto'
#   Build OGG Vorbis Sound Module
# 
# build_video_theora = 'auto'
#   Build Theora Video Module
# 
# build_video_apng = 'auto'
#   Build Animated PNG Video Module
# 
# build_igde = 'auto'
#   Build IGDE
# 
# build_guilauncher = 'auto'
#   Build GUI Launcher
# 
# build_launcher_android = 'auto'
#   Build Android Launcher
# 
# build_archive_delga = 'auto'
#   Build DELGA Archive Module



# Archiving and Installing
############################

# Archive file format.
# 
# Possible values: 'tarbz2', 'zip'
# 
# archive_format = 'tarbz2'

# Archive file name without extension for Drag[en]gine archive.
# 
# Value type: string
# 
# archive_name_engine = 'dragengine'

# Archive file name without extension for Drag[en]gine Development archive
# 
# Value type: string
# 
# archive_name_engine_dev = 'dragengine-dev'

# Archive file name without extension for IGDE archive
# 
# Value type: string
# 
# archive_name_igde = 'deigde'

# Archive file name without extension for IGDE Development archive
# 
# Value type: string
# 
# archive_name_igde_dev = 'deigde_dev'

# Archive file name without extension for Special archive
# 
# Value type: string
# 
# archive_name_special = 'despecial'

# Installer file name without extension for Drag[en]gine installer
# 
# Value type: string
# 
# installer_name_engine = 'install-dragengine'

# Installer file name without extension for Drag[en]gine Development installer
# 
# Value type: string
# 
# installer_name_engine_dev = 'install-dragengine-dev'

# Installer file name without extension for IGDE installer
# 
# Value type: string
# 
# installer_name_igde = 'install-deigde'

# Installer file name without extension for IGDE Development installer
# 
# Value type: string
# 
# installer_name_igde_dev = 'install-deigde-dev'



# Operating System Specific
#############################
# 
# See the operating system specific dist.custom.* files
# 
# dist.posix.custom.py
#   for POSIX platforms
# 
# dist.windows.custom.py
#   for Windows platforms
# 
# dist.beos.custom.py
#   for BeOS platforms
# 
# dist.macos.custom.py
#   for MacOS platforms
# 
# dist.android.custom.py
#   for Android platforms
