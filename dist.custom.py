"""
Notes

Most parameters in here are tri-state and accept the values 'auto', 'yes' and 'no'.
"""


# General configuration
#########################

with_tests = 'no'
with_warnerrors = 'no'
with_debug = 'no'
with_verbose = 'no'



# External Library Configuration
##################################

# Use libraries installed on the system instead of compiling from yours. For all
# options under this category the following values can be used:
# 
# auto (default)
#   Use the library installed on the system if found. Otherwise build from internal sources.
# 
# yes
#   Force using the library installed on the system. Building from internal sources is not
#   allowed. If the library is not installed on the system the build will be aborted.
# 
# no
#   The library installed on the system is not used if found. The library is always build
#   from the internal sources. This is best used for cross-compiling and building
#   distribution archives for foreign system where installed libraries can not be predicted.

# zLib Compression Library
with_system_zlib = 'auto'

# libPNG Graphic Library
with_system_libpng = 'auto'

# libPNG Graphic Library patched with Animated-PNG support. This is usually not installed
# on systems and should be best build from sources (no option)
with_system_libapng ='auto'

# libJPEG Graphic Library
with_system_libjpeg ='auto'

# OpenAL Audio Library.
with_system_openal = 'auto'

# OGG Sound Library
with_system_libogg ='auto'

# OGG Vorbis Sound Library. Depends on OGG.
with_system_libvorbis ='auto'

# Theora Video Library. Depends on OGG Vorbis and OGG.
with_system_libtheora ='auto'

# FOX-ToolKit Library. Depends on libPNG, libJPEG and zLib.
with_system_fox ='auto'

# DragonScript Programming Language.
with_system_dragonscript = 'auto'

# libFFI Library.
with_system_libffi = 'auto'

# libLTDL Library.
with_system_libltdl = 'auto'

# libSegV Library.
with_system_libsigsegv = 'auto'

# SmallTalk Programming Language. Depends on libFFI and libSegV
with_system_smalltalk = 'auto'

# OpenGL Graphic Library. This library can not be compiled from sources.
with_opengl = 'auto'

# Python Programming Language.
with_python = 'auto'

# NPAPI SDK. This library can not be compiled from sources.
with_npapisdk = 'auto'

# DL Library. This library can not be compiled. Found on Linux type systems.
with_dl = 'auto'

# POSIX Thread Library. This library can not be compiled. Found on Linux type systems.
with_pthread = 'auto'

# X-System Library. This library can not be compiled. Found on Linux type systems.
with_x = 'auto'

# Use system libevdev
with_system_libevdev = 'auto'

# Use system libusb
with_system_libusb = 'auto'

# Use system libhidapi
with_system_libhidapi = 'auto'

# Use system libopenhmd
with_system_libopenhmd = 'auto'

# Use system libfftw
with_system_fftw = 'auto'



# Modules Configuration
#########################

build_audio_openal = 'auto'
build_cr_basic = 'auto'
build_graphics_opengl = 'auto'
build_image_png = 'auto'
build_image_png3d = 'auto'
build_image_jpeg = 'auto'
build_input_x = 'auto'
build_input_w32 = 'auto'
build_input_android = 'auto'
build_input_beos = 'auto'
build_input_macos = 'auto'
build_physics_bullet = 'auto'
build_script_ds = 'auto'
build_script_python = 'auto'
build_script_smalltalk = 'auto'
build_sound_ogg = 'auto'
build_video_theora = 'auto'
build_video_apng = 'auto'
build_igde = 'auto'
build_guilauncher = 'auto'
build_plugin_npapi = 'auto'
build_launcher_android = 'auto'

# Archiving
#############
archive_format = 'tarbz2'
archive_name_engine = 'dragengine'
archive_name_engine_dev = 'dragengine-dev'
archive_name_igde = 'igde'
archive_name_igde_dev = 'igde-dev'
archive_name_special = 'special'

# Installing
##############
installer_name_engine = 'install-dragengine'
installer_name_engine_dev = 'install-dragengine-dev'
installer_name_igde = 'install-igde'
installer_name_igde_dev = 'install-igde-dev'



# Windows Platform
####################
#
# Special path indicators can be used. Only one special path indicator can be used
# and it has to be the first component in the path. The following special path
# indicators are supported:
#
# @ProgramFiles
#   The Program Files folder. A typical path is C:\Program Files.
#
# @System
#   The Windows System folder. A typical path is C:\Windows\System32.
#
# @RoamingAppData
#   The file system directory that serves as a common repository for
#   application-specific data. A typical path is C:\Documents and
#   Settings\username\Application Data.
#
# @ProgramData
#   The file system directory that contains application data for all users.
#   A typical path is C:\Documents and Settings\All Users\Application Data.
#   This folder is used for application data that is not user specific. For
#   example, an application can store a spell-check dictionary, a database
#   of clip art, or a log file in the $ProgramData folder. This information
#   will not roam and is available to anyone using the computer.
#
# @PublicDocuments
#   The file system directory that contains documents that are common to all
#   users. A typical path is C:\Documents and Settings\All Users\Documents.
#
# @LocalAppData
#   The file system directory that serves as a data repository for local
#   (nonroaming) applications. A typical path is C:\Documents and
#   Settings\username\Local Settings\Application Data.
#
# @Documents
#   The virtual folder that represents the My Documents desktop item. A typical
#   path is C:\Documents and Settings\username\My Documents.</td>
#
# @Windows
#   The Windows directory or $System. This corresponds to the %windir% or
#   %SYSTEMROOT% environment variables. A typical path is C:\Windows.
