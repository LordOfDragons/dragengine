# configuration for debian package building
###########################################

# general configuration
#########################
with_tests = 'no'
with_warnerrors = 'no'
with_debug = 'no'
with_verbose = 'no'

prefix = '/usr'
libdir = '${prefix}/lib'
includedir = '${prefix}/include'
datadir = '${prefix}/share'
sysconfdir = '/etc'
execdir = '${prefix}/games'
sysvardir = '/var'
cachedir = '${sysvardir}/cache'

path_de_data = '${libdir}/games/dragengine'
path_de_share = '${datadir}/games/dragengine'

path_igde_data = '${libdir}/games/deigde'
path_igde_share = '${datadir}/games/deigde'

path_launcher_data = '${libdir}/games/delauncher'
path_launcher_share = '${datadir}/games/delauncher'
path_launcher_games = '${datadir}/games/delauncher/games'

# external library configuration
##################################
# debian expects to not use in-tree versions. as far as possible this is
# done here and in-tree is enabled only for those which cause problems
with_system_zlib = 'yes'
with_system_libpng = 'yes'
with_system_libapng ='yes'
with_system_libjpeg ='yes'
with_system_openal = 'yes'
with_openal_backends = ','.join(['alsa', 'pulseaudio', 'portaudio', 'oss'])
with_system_libogg ='yes'
with_system_libvorbis ='yes'
with_system_libtheora ='yes'
with_system_libhidapi = 'yes'
with_system_libopenhmd = 'yes'
with_system_libevdev = 'yes'
with_system_dragonscript = 'yes'
with_opengl = 'yes'
with_dl = 'yes'
with_pthread = 'yes'
with_x = 'yes'

# these are not used since python and npapisdk are disabled. set to in-tree
# to not upset the build system although they will never be build at all
with_python = 'no'
with_npapisdk = 'no'

# these packages cause troubles on debian and are build in-tree

# libfox in debian is 1.6 but 1.7 is required
with_system_fox ='no'

# libusb is required by libhidapi which is system compiled so it is not required.
# set to 'no' to avoid build system trying to find it in system
with_system_libusb = 'no'

# these are only required for the smalltalk module which is currently not build.
# to not upset the build system with not finding packages they are set to in-tree
# building although they will never be build at all
with_system_libffi = 'no'
with_system_libltdl = 'no'
with_system_libsigsegv = 'no'
with_system_smalltalk = 'no'

# this package is not used right now but might be later on by the openal module.
# as with the smalltalk case above this package is set to in-tree building
# although it will never be build at all
with_system_fftw = 'no'

# modules configuration
#########################
build_audio_openal = 'yes'
build_cr_basic = 'yes'
build_graphics_opengl = 'yes'
build_image_png = 'yes'
build_image_png3d = 'yes'
build_image_jpeg = 'yes'
build_input_x = 'yes'
build_physics_bullet = 'yes'
build_script_ds = 'yes'
build_sound_ogg = 'yes'
build_video_theora = 'yes'
build_video_apng = 'yes'
build_igde = 'yes'
build_guilauncher = 'yes'
build_archive_delga = 'yes'

build_input_w32 = 'no'
build_input_beos = 'no'
build_input_macos = 'no'
build_input_android = 'no'
build_script_python = 'no'
build_script_smalltalk = 'no'
build_plugin_npapi = 'no'
build_launcher_android = 'no'

# archiving
#############
# the names used here are important for the 'rules' file to find them
archive_format = 'tarbz2'
archive_name_engine = 'dragengine-linux'
archive_name_engine_dev = 'dragengine-dev-linux'
archive_name_igde = 'deigde-linux'
archive_name_igde_dev = 'deigde-dev-linux'
archive_name_special = 'despecial-linux'
installer_name_engine = 'install-dragengine-linux'
installer_name_engine_dev = 'install-dragengine-dev-linux'
installer_name_igde = 'install-deigde-linux'
installer_name_igde_dev = 'install-deigde-dev-linux'
