# general configuration
#########################
with_tests = 'no'
with_warnerrors = 'no'
with_debug = 'no'
with_verbose = 'no'
with_threads = '14'
version = '{BuildVersion}'

ndkroot = '{NdkRoot}'
platform_android = 'armv8'
apilevel = '29' # Android 10
hardfp = True

# external library configuration
##################################
with_system_zlib = 'no'
with_system_libpng = 'no'
with_system_libapng = 'no'
with_system_libjpeg = 'no'
with_system_openal = 'no'
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
with_system_liburing = 'no'
with_opengl = 'yes'
with_python = 'no'
with_npapisdk = 'no'
with_dl = 'yes'
with_pthread = 'yes'
with_x = 'no'

# modules configuration
#########################
build_audio_openal = 'yes'
build_cr_basic = 'no'
build_graphics_opengl = 'yes'
build_image_png = 'yes'
build_image_png3d = 'yes'
build_image_jpeg = 'yes'
build_image_webp = 'yes'
build_input_x = 'no'
build_input_w32 = 'no'
build_input_beos = 'no'
build_input_macos = 'no'
build_input_android = 'yes'
build_physics_bullet = 'yes'
build_script_ds = 'yes'
build_script_python = 'no'
build_script_smalltalk = 'no'
build_sound_ogg = 'yes'
build_video_theora = 'yes'
build_video_apng = 'yes'
build_guilauncher = 'no'
build_plugin_npapi = 'no'
build_launcher_android = 'yes'
build_archive_delga = 'yes'
build_vr_openvr = 'yes'
build_vr_openxr = 'yes'

# IGDE
########
build_igde = 'no'
igde_toolkit = 'null'
build_editor_animator = 'no'
build_editor_conversation = 'no'
build_editor_font = 'no'
build_editor_gamedefinition = 'no'
build_editor_langpack = 'no'
build_editor_particleemitter = 'no'
build_editor_rig = 'no'
build_editor_skin = 'no'
build_editor_sky = 'no'
build_editor_speechanimation = 'no'
build_editor_synthesizer = 'no'
build_editor_world = 'no'

# archiving
#############
archive_format = 'tarbz2'
archive_name_engine = 'dragengine-{ReleaseVersion}-android64'
archive_name_engine_dev = 'dragengine-dev-{ReleaseVersion}-android64'
archive_name_igde = 'deigde-{ReleaseVersion}-android64'
archive_name_igde_dev = 'deigde-dev-{ReleaseVersion}-android64'
archive_name_special = 'despecial-{ReleaseVersion}-android64'
installer_name_engine = 'install-dragengine-{ReleaseVersion}-android64'
installer_name_engine_dev = 'install-dragengine-dev-{ReleaseVersion}-android64'
installer_name_igde = 'install-deigde-{ReleaseVersion}-android64'
installer_name_igde_dev = 'install-deigde-dev-{ReleaseVersion}-android64'
addon_name_blender = 'blender-addon-dragengine'
