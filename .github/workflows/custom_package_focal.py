# general configuration
#########################
with_tests = 'no'
with_warnerrors = 'no'
with_debug = 'no'
with_verbose = 'no'
with_threads = '14'
version = '{ReleaseVersion}'

# external library configuration
##################################
with_system_zlib = 'yes'
with_system_libpng = 'yes'
with_system_libapng ='yes'
with_system_libjpeg ='yes'
with_system_libwebp = 'yes'
with_system_openal = 'yes'
with_openal_backends = ','.join(['alsa', 'pulseaudio', 'portaudio', 'oss'])
with_system_libogg ='yes'
with_system_libvorbis ='yes'
with_system_libtheora ='yes'
with_system_fox ='no'               # ubuntu focal still has 1.6
with_system_dragonscript = 'yes'
with_system_libffi = 'no'           # smalltalk only
with_system_libltdl = 'no'          # smalltalk only
with_system_libsigsegv = 'no'       # smalltalk only
with_system_smalltalk = 'no'        # smalltalk only
with_system_libevdev = 'no'         # ubuntu package not usable
with_system_libusb = 'no'           # ubuntu package not usable
with_system_libhidapi = 'yes'
with_system_libopenhmd = 'yes'
with_system_fftw = 'no'             # not used right now
with_opengl = 'yes'
with_python = 'no'
with_npapisdk = 'no'
with_dl = 'yes'
with_pthread = 'yes'
with_x = 'yes'

# modules configuration
#####################
build_audio_openal = 'yes'
build_cr_basic = 'yes'
build_graphics_opengl = 'yes'
build_image_png = 'yes'
build_image_png3d = 'yes'
build_image_jpeg = 'yes'
build_image_webp = 'yes'
build_input_x = 'yes'
build_input_w32 = 'no'
build_input_beos = 'no'
build_input_macos = 'no'
build_input_android = 'no'
build_physics_bullet = 'yes'
build_script_ds = 'yes'
build_script_python = 'no'
build_script_smalltalk = 'no'
build_sound_ogg = 'yes'
build_video_theora = 'yes'
build_video_apng = 'yes'
build_igde = 'yes'
build_guilauncher = 'yes'
build_plugin_npapi = 'no'
build_launcher_android = 'no'
build_archive_delga = 'yes'

# archiving
#############
archive_format = 'tarbz2'
archive_name_engine = 'dragengine-{ReleaseVersion}-linux64'
archive_name_engine_dev = 'dragengine-dev-{ReleaseVersion}-linux64'
archive_name_igde = 'deigde-{ReleaseVersion}-linux64'
archive_name_igde_dev = 'deigde-dev-{ReleaseVersion}-linux64'
archive_name_special = 'despecial-{ReleaseVersion}-linux64'
installer_name_engine = 'install-dragengine-{ReleaseVersion}-linux64'
installer_name_engine_dev = 'install-dragengine-dev-{ReleaseVersion}-linux64'
installer_name_igde = 'install-deigde-{ReleaseVersion}-linux64'
installer_name_igde_dev = 'install-deigde-dev-{ReleaseVersion}-linux64'
addon_name_blender = 'blender-addon-dragengine'
