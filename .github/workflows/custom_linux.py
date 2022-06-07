# general configuration
#########################
with_tests = 'no'
with_warnerrors = 'no'
with_debug = 'no'
with_verbose = 'no'
with_threads = '14'
version = '{BuildVersion}'

# external library configuration
##################################
with_system_zlib = 'no'
with_system_libpng = 'no'
with_system_libapng = 'no'
with_system_libjpeg = 'no'
with_system_openal = 'no'
with_openal_backends = ','.join(['alsa', 'pulseaudio', 'portaudio', 'oss'])
with_system_libogg = 'no'
with_system_libvorbis = 'no'
with_system_libtheora = 'no'
with_system_fox = 'no'
with_system_dragonscript = 'no'
with_system_libffi = 'no'
with_system_libltdl = 'no'
with_system_libsigsegv = 'no'
with_system_smalltalk = 'no'
with_system_libevdev = 'no'
with_system_libusb = 'no'
with_system_libhidapi = 'no'
with_system_libopenhmd = 'no'
with_system_fftw = 'no'
with_system_libwebp = 'no'
with_opengl = 'yes'
with_python = 'no'
with_npapisdk = 'no'
with_dl = 'yes'
with_pthread = 'yes'
with_x = 'yes'

# modules configuration
#########################
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
build_guilauncher = 'yes'
build_plugin_npapi = 'no'
build_launcher_android = 'no'
build_archive_delga = 'yes'
build_vr_openvr = 'yes'
build_vr_openxr = 'yes'

# IGDE
########
build_igde = 'yes'
igde_toolkit = 'fox'
build_editor_animator = 'yes'
build_editor_conversation = 'yes'
build_editor_font = 'yes'
build_editor_gamedefinition = 'yes'
build_editor_langpack = 'yes'
build_editor_particleemitter = 'yes'
build_editor_rig = 'yes'
build_editor_skin = 'yes'
build_editor_sky = 'yes'
build_editor_speechanimation = 'yes'
build_editor_synthesizer = 'yes'
build_editor_world = 'yes'

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
