from SConsCommon import *
from SConsPlatformAndroid import androidUpdateEnv
from SConsPlatformWebWasm import webWasmUpdateEnv
import shlex


# create environment
tools = ARGUMENTS.get('tools', '')
if tools:
	if tools == 'mingw64':
		import os, shutil
		
		# import os. llvm-mingw needs to be installed outside /usr . required PATH to be respected
		parent_env = Environment(ENV = {'PATH': os.environ['PATH']}, CPPPATH='.', LIBPATH='.', tools=['mingw'])
		#parent_env = Environment(CPPPATH='.', LIBPATH='.', tools=['mingw'])
		
		compiler = 'x86_64-w64-mingw32'
		if not parent_env.Detect('{}-g++'.format(compiler)):
			print('Windows 64-bit Cross-Compiler not found.')
			Return()
		
		parent_env.Replace(CC = '{}-gcc'.format(compiler))
		parent_env.Replace(CXX = '{}-g++'.format(compiler))
		parent_env.Replace(LD = '{}-ld'.format(compiler))
		parent_env.Replace(AR = '{}-ar'.format(compiler))
		parent_env.Replace(STRIP = '{}-strip'.format(compiler))
		#parent_env.Replace(MAKE = '{}-make'.format(compiler))
		parent_env.Replace(RANLIB = '{}-ranlib'.format(compiler))
		parent_env.Replace(NM = '{}-nm'.format(compiler))
		parent_env.Replace(RC = '{}-windres'.format(compiler))
		parent_env.Replace(DLLTOOL = '{}-dlltool'.format(compiler))
		
		parent_env.Replace(SHCCFLAGS = ['$CCFLAGS']) # remove -fPIC if included. just to silence misleading warnings
		parent_env.Replace(SHLIBPREFIX = '') # fix prefix since the environment is set up for unix
		parent_env.Replace(SHLIBSUFFIX = '.dll') # fix suffix since the environment is set up for unix
		parent_env.Replace(LIBPREFIX = '') # fix prefix since the environment is set up for unix
		parent_env.Replace(LIBPREFIXES = ['']) # fix prefix since the environment is set up for unix
		parent_env.Replace(LIBSUFFIX = '.lib')
		parent_env.Replace(LIBSUFFIXES = ['.lib', '.a'])
		
		parent_env['OS_NAME'] = 'win32'
		parent_env['SYS_PLATFORM'] = 'win32'
		parent_env['CROSSCOMPILE_HOST'] = compiler
		
		parent_env['CROSSCOMPILE_CLANG'] = parent_env.Detect('{}-clang++'.format(compiler)) != None
		
		# try to guess the sysroot
		pathCompiler = shutil.which(parent_env['CXX'])
		dirCompiler = os.path.split(pathCompiler)[0]
		dirCompilerUp = os.path.split(dirCompiler)[0]
		parent_env['CROSSCOMPILE_SYSROOT'] = os.path.join(dirCompilerUp, compiler)
		#print('sysroot: ' + parent_env['CROSSCOMPILE_SYSROOT'])
		
		# prevent stdc++6 problems with missnig symbols on different compilers
		#parent_env.Append(CPPFLAGS = ['-std=c++11'])
		#parent_env.Append(CROSSCOMPILE_CFLAGS = ['-std=c++11'])
		#parent_env.Append(CROSSCOMPILE_CPPFLAGS = ['-std=c++11'])
		
		# app store requirements
		parent_env.Append(LINKFLAGS = ['-Wl,--dynamicbase'])
		parent_env.Append(LINKFLAGS = ['-Wl,--nxcompat'])
		parent_env['WINDOWS_LIBS'] = []
		#parent_env.Append(WINDOWS_LIBS = ['winstorecompat'])
		#parent_env.Append(WINDOWS_LIBS = ['kernel32', 'user32', 'gdi32', 'winmm', 'wsock32', 'advapi32'])
		
		parent_env.Append(CROSSCOMPILE_LINKFLAGS = ['-Wl,--dynamicbase'])
		parent_env.Append(CROSSCOMPILE_LINKFLAGS = ['-Wl,--nxcompat'])
		
		print('*** Using Windows 64 Cross Compiler')
		
	else:
		parent_env = Environment(CPPPATH='.', LIBPATH='.', tools=[tools])
		parent_env['OS_NAME'] = os.name
		parent_env['SYS_PLATFORM'] = sys.platform
		parent_env['WINDOWS_LIBS'] = []
		#parent_env.Append(WINDOWS_LIBS = ['kernel32', 'user32', 'gdi32', 'winmm', 'wsock32', 'advapi32'])

elif sys.platform == 'win32':
	print('Windows detected. Setting up MinGW64 toolchain')
	parent_env = Environment(CPPPATH='.', LIBPATH='.', tools=['mingw'])
	"""parent_env.Replace(CC = 'x86_64-w64-mingw32-gcc.exe')
	parent_env.Replace(CXX = 'x86_64-w64-mingw32-g++.exe')
	parent_env.Replace(LD = 'x86_64-w64-mingw32-g++.exe')
	parent_env.Replace(AR = 'x86_64-w64-mingw32-ar')
	parent_env.Replace(STRIP = 'x86_64-w64-mingw32-strip')
	parent_env.Replace(MAKE = 'x86_64-w64-mingw32-make')
	parent_env.Replace(RC = 'x86_64-w64-mingw32-windres')
	parent_env.Replace(RC = 'x86_64-w64-mingw32-dlltool')"""
	#parent_env.Append(ENV = {'PATH' : 'C:\\MinGW\\x86_64-w64-mingw32\\bin\\'})
	#parent_env.Append(ENV = {'PATH' : 'C:\MinGW\bin;C:\MinGW\libexec\gcc\x86_64-w64-mingw32\4.8.3;' + os.environ['PATH']})
	##parent_env.Append(ENV = {'PATH' : 'C:\MinGW\bin;' + os.environ['PATH']})
	parent_env['OS_NAME'] = os.name
	parent_env['SYS_PLATFORM'] = sys.platform
	parent_env['WINDOWS_LIBS'] = []
	#parent_env.Append(WINDOWS_LIBS = ['kernel32', 'user32', 'gdi32', 'winmm', 'wsock32', 'advapi32'])
	
else:
	parent_env = Environment(CPPPATH='.', LIBPATH='.')
	parent_env['OS_NAME'] = os.name
	parent_env['SYS_PLATFORM'] = sys.platform

if not 'CROSSCOMPILE_CLANG' in parent_env:
	parent_env['CROSSCOMPILE_CLANG'] = False

# Haiku: The PATH found by SCons are wrong in many ways causing binaries to be not found.
#        Replace them with sane values. This is not a 'good' solution but should work.
if sys.platform == 'haiku1':
	parent_env['ENV']['PATH'] = os.environ['PATH']

parent_env.Tool('logStdOut')
if parent_env['LogStdOut_Enabled']:
	parent_env['LOG_STD_OUT_FILE'] = open('build.log', 'w')

parent_env.Tool('runExternalCommand')
parent_env.Tool('macos_bundle')
parent_env.Tool('downloadArtifact')
parent_env.Tool('updateModuleManifest')

InitCommon(parent_env)
#print('os.name', os.name)
#print('sys.platform', sys.platform)

# load platform specific tools
if parent_env['OSBeOS']:
	parent_env.Tool('beosResource')

# append flags
parent_env.Replace(MODULE_CPPFLAGS = [])
parent_env.Replace(MODULE_LINKFLAGS = [])

if 'CPPFLAGS' in os.environ:
	parent_env.Append(CPPFLAGS = shlex.split(os.environ['CPPFLAGS']))

if 'LDFLAGS' in os.environ:
	parent_env.Append(LINKFLAGS = shlex.split(os.environ['LDFLAGS']))

if parent_env['OSPosix']:
	parent_env.Append(CPPFLAGS = ['-DOS_UNIX'])

if parent_env['OSWindows']:
	parent_env.Append(CPPFLAGS = ['-DOS_W32', '-mwindows'])
	# mingw requires this to recognize wWinMain
	parent_env.Append(CPPFLAGS = ['-municode'])
	
elif parent_env['OSBeOS']:
	parent_env.Append(CPPFLAGS = ['-DOS_BEOS'])
	parent_env.Append(LINKFLAGS = ['-L/boot/common/lib'])
	
elif parent_env['OSMacOS']:
	parent_env.Append(CPPFLAGS = ['-DOS_MACOS'])

if not (parent_env['OSPosix'] or parent_env['OSWindows'] or parent_env['OSBeOS'] or parent_env['OSMacOS']):
	Exit('No supported OS found!')

# parameters
#params = Variables(['parameters.cache', 'custom.py'])
params = Variables(['custom.py'])

params.Add(EnumVariable('platform_android', 'Build for Android platform', 'no', ['no', 'armv7', 'armv8', 'x86', 'quest']))
params.Add(BoolVariable('platform_webwasm', 'Build for Web WASM platform', False))
params.Add(BoolVariable('with_tests', 'Build engine tests', False))
params.Add(BoolVariable('with_debug', 'Build with debug symbols for GDB usage', False))
params.Add(BoolVariable('with_debug_symbols', 'For release build only force debug symbols', False))
params.Add(BoolVariable('with_warnerrors', 'Treat warnings as errors (dev-builds)', False))
params.Add(BoolVariable('with_sanitize', 'Enable sanitizing (dev-builds)', False))
params.Add(BoolVariable('with_sanitize_thread', 'Enable thread sanitizing (dev-builds)', False))
params.Add(BoolVariable('with_verbose', 'Verbose compilation showing command lines(dev-builds)', False))
params.Add(BoolVariable('with_ci', 'Build CI', False))
params.Add(StringVariable('version', 'Version', '9999'))
params.Add(StringVariable('force_version', 'Force version (empty to disable)', ''))
params.Add(StringVariable('with_threads', 'Count of threads to use for building external packages', '1'))
params.Add(StringVariable('with_cmake_flags', 'Additional flags for external CMake builds', ''))
params.Add(StringVariable('with_cmake_c_flags', 'Additional C flags for external CMake builds', ''))
params.Add(BoolVariable('with_engine_module_checks', 'Check engine module file before loading', True))
params.Add(StringVariable('distro_maintained_info_url', 'Package is distribution maintaned and URL contains update information', ''))

params.Add(StringVariable('url_extern_artifacts',
	'Base URL to download external artifacts from if missing',
	'https://dragondreams.s3.eu-central-1.amazonaws.com/dragengine/extern'))

params.Add(TernaryVariable('with_system_zlib', 'Use System Zlib'))
params.Add(TernaryVariable('with_system_libpng', 'Use System libpng'))
params.Add(TernaryVariable('with_system_sndio', 'Use System sndio'))
params.Add(TernaryVariable('with_system_libapng', 'Use System libapng'))
params.Add(TernaryVariable('with_system_libjpeg', 'Use System JPEG'))
params.Add(TernaryVariable('with_system_openal', 'Use System OpenAL', TernaryVariableNo))
params.Add(ListVariable('with_openal_backends', 'When compiling OpenAL what backends are required',
	[], ['alsa', 'pulseaudio', 'portaudio', 'oss']))
params.Add(TernaryVariable('with_system_libogg', 'Use System libogg'))
params.Add(TernaryVariable('with_system_libvorbis', 'Use System libvorbis'))
params.Add(TernaryVariable('with_system_libtheora', 'Use System libtheora'))
params.Add(TernaryVariable('with_system_fox', 'Use System FOX Toolkit'))
params.Add(TernaryVariable('with_system_dragonscript', 'Use System DragonScript'))
params.Add(PathVariable('with_dragonscript_inc',
	'Path to DragonScript include files or empty to use system default',
	'', PathVariable.PathAccept))
params.Add(PathVariable('with_dragonscript_lib',
	'Path to DragonScript library files or empty to use system default',
	'', PathVariable.PathAccept))
params.Add(TernaryVariable('with_system_libevdev', 'Use System libevdev'))
params.Add(TernaryVariable('with_system_soundtouch', 'Use System soundtouch'))
params.Add(TernaryVariable('with_system_libwebp', 'Use System libwebp'))
params.Add(TernaryVariable('with_system_libwebm', 'Use System libwebm'))
params.Add(TernaryVariable('with_system_libvpx', 'Use System libvpx'))
params.Add(TernaryVariable('with_system_liburing', 'Use System liburing'))
params.Add(TernaryVariable('with_system_openvr', 'Use System OpenVR'))
params.Add(TernaryVariable('with_system_openxr', 'Use System OpenXR'))
params.Add(TernaryVariable('with_system_jsoncpp', 'Use System JsonCpp'))
params.Add(PathVariable('with_deremotelauncher_inc',
	'Path to DERemoteLauncher include files or empty to use system default',
	'', PathVariable.PathAccept))
params.Add(PathVariable('with_deremotelauncher_lib',
	'Path to DERemoteLauncher library files or empty to use system default',
	'', PathVariable.PathAccept))
params.Add(PathVariable('with_denetwork_inc',
	'Path to DENetwork include files or empty to use system default',
	'', PathVariable.PathAccept))
params.Add(PathVariable('with_denetwork_lib',
	'Path to DENetwork library files or empty to use system default',
	'', PathVariable.PathAccept))
params.Add(TernaryVariable('with_system_freetype', 'Use System FreeType'))

params.Add(TernaryVariable('with_opengl', 'Use OpenGL'))
params.Add(TernaryVariable('with_python', 'Use Python'))
params.Add(TernaryVariable('with_npapisdk', 'Use NPAPI SDK'))

params.Add(TernaryVariable('build_archive_delga', 'Build DELGA Archive Module'))
params.Add(TernaryVariable('build_audio_openal', 'Build OpenAL Audio Module'))
params.Add(TernaryVariable('build_cr_basic', 'Build Basic Crash-Recovery Module'))
params.Add(TernaryVariable('build_graphics_opengl', 'Build OpenGL Graphics Module'))
params.Add(TernaryVariable('build_graphics_vulkan', 'Build Vulkan Graphics Module', False))
params.Add(TernaryVariable('build_guilauncher', 'Build GUI Launcher'))
params.Add(TernaryVariable('build_consolelauncher', 'Build Console Launcher'))
params.Add(TernaryVariable('build_font_freetype', 'Build FreeType Font Module'))
params.Add(TernaryVariable('build_image_jpeg', 'Build JPEG Image Module'))
params.Add(TernaryVariable('build_image_png', 'Build PNG Image Module'))
params.Add(TernaryVariable('build_image_png3d', 'Build PNG-3D Image Module'))
params.Add(TernaryVariable('build_image_webp', 'Build WebP Image Module'))
params.Add(TernaryVariable('build_image_webp3d', 'Build WebP-3D Image Module'))
params.Add(TernaryVariable('build_input_android', 'Build Android Input Module'))
params.Add(TernaryVariable('build_input_beos', 'Build BeOS Input Module'))
params.Add(TernaryVariable('build_input_macos', 'Build MacOS Input Module'))
params.Add(TernaryVariable('build_input_w32', 'Build Windows Input Module'))
params.Add(TernaryVariable('build_input_x', 'Build X Input Module'))
params.Add(TernaryVariable('build_launcher_android', 'Build Android Launcher'))
params.Add(TernaryVariable('build_physics_bullet', 'Build Bullet Physics Module'))
params.Add(TernaryVariable('build_script_ds', 'Build DragonScript Script Module'))
params.Add(TernaryVariable('build_script_python', 'Build Python Script Module'))
params.Add(TernaryVariable('build_service_eossdk', 'Build EOS SDK Service Module'))
params.Add(TernaryVariable('build_service_modio', 'Build ModIO Service Module'))
params.Add(TernaryVariable('build_service_steamsdk', 'Build SteamSDK Service Module'))
params.Add(TernaryVariable('build_sound_ogg', 'Build OGG Vorbis Sound Module'))
params.Add(TernaryVariable('build_video_apng', 'Build Animated PNG Video Module'))
params.Add(TernaryVariable('build_video_theora', 'Build Theora Video Module'))
params.Add(TernaryVariable('build_video_webm', 'Build WebM Video Module'))
params.Add(TernaryVariable('build_vr_openvr', 'Build OpenVR VR Module', False))
params.Add(TernaryVariable('build_vr_openxr', 'Build OpenXR VR Module'))

params.Add(BoolVariable('build_ai_deai_internal', 'Build Drag[en]gine AI AI Module as internal module', True))
params.Add(BoolVariable('build_ani_deanimator_internal', 'Build Drag[en]gine Animator Animator Module as internal module', True))
params.Add(BoolVariable('build_anim_deanim_internal', 'Build Drag[en]gine Animation Animation Module as internal module', True))
params.Add(BoolVariable('build_archive_delga_internal', 'Build DELGA Ardhive Module as internal module', True))
params.Add(BoolVariable('build_audio_null_internal', 'Build Null Audio Module as internal module', True))
params.Add(BoolVariable('build_audio_openal_internal', 'Build OpenAL Audio Module as internal module', True))
params.Add(BoolVariable('build_comb_fbx_internal', 'Build FBX based modules as internal module', True))
params.Add(BoolVariable('build_cr_basic_internal', 'Build Basic Crash-Recovery Module as internal module', True))
params.Add(BoolVariable('build_cr_simplyquit_internal', 'Build SimplyQuit Crash-Recovery Module as internal module', True))
params.Add(BoolVariable('build_font_defont_internal', 'Build Drag[en]gine Font Font Module as internal module', True))
params.Add(BoolVariable('build_font_freetype_internal', 'Build FreeType Font Module as internal module', True))
params.Add(BoolVariable('build_graphic_null_internal', 'Build Null Graphic Module as internal module', True))
params.Add(BoolVariable('build_graphic_opengl_internal', 'Build OpenGL Graphic Module as internal module', True))
params.Add(BoolVariable('build_image_ies_internal', 'Build IES Image Module as internal module', True))
params.Add(BoolVariable('build_image_jpeg_internal', 'Build JPEG Image Module as internal module', True))
params.Add(BoolVariable('build_image_png_internal', 'Build PNG Image Module as internal module', True))
params.Add(BoolVariable('build_image_png3d_internal', 'Build PNG-3D Image Module as internal module', True))
params.Add(BoolVariable('build_image_tga_internal', 'Build Targa Image Module as internal module', True))
params.Add(BoolVariable('build_image_webp_internal', 'Build WebP Image Module as internal module', True))
params.Add(BoolVariable('build_image_webp3d_internal', 'Build WebP-3D Image Module as internal module', True))
params.Add(BoolVariable('build_input_android_internal', 'Build Android Input Module as internal module', True))
params.Add(BoolVariable('build_input_console_internal', 'Build Console Input Module as internal module', True))
params.Add(BoolVariable('build_input_x_internal', 'Build X Input Module as internal module', True))
params.Add(BoolVariable('build_input_beos_internal', 'Build BeOS Input Module as internal module', True))
params.Add(BoolVariable('build_langpack_delangpack_internal', 'Build DELangPack Language Pack Module as internal module', True))
params.Add(BoolVariable('build_model_demodel_internal', 'Build Drag[en]gine Model Model Module as internal module', True))
params.Add(BoolVariable('build_network_basic_internal', 'Build Basic Network Module as internal module', True))
params.Add(BoolVariable('build_occmesh_deoccmesh_internal', 'Build Drag[en]gine Occlusion Mesh Occlusion Mesh Module as internal module', True))
params.Add(BoolVariable('build_physics_bullet_internal', 'Build Bullet Physics Module as internal module', True))
params.Add(BoolVariable('build_rig_derig_internal', 'Build Drag[en]gine Rig Rig Module as internal module', True))
params.Add(BoolVariable('build_script_ds_internal', 'Build DragonScript Script Module as internal module', True))
params.Add(BoolVariable('build_service_modio_internal', 'Build Mod.io Service Module as internal module', True))
params.Add(BoolVariable('build_skin_deskin_internal', 'Build Drag[en]gine Skin Skin Module as internal module', True))
params.Add(BoolVariable('build_sound_ogg_internal', 'Build OGG Vorbis Sound Module as internal module', True))
params.Add(BoolVariable('build_synthesizer_desynthesizer_internal', 'Build Drag[en]gine Synthesizer Synthesizer Module as internal module', True))
params.Add(BoolVariable('build_video_apng_internal', 'Build APNG Video Module as internal module', True))
params.Add(BoolVariable('build_video_theora_internal', 'Build Theora Video Module as internal module', True))
params.Add(BoolVariable('build_video_webm_internal', 'Build WebM Video Module as internal module', True))
params.Add(BoolVariable('build_vr_null_internal', 'Build Null VR Module as internal module', True))
params.Add(BoolVariable('build_vr_openvr_internal', 'Build OpenVR VR Module as internal module', False))
params.Add(BoolVariable('build_vr_openxr_internal', 'Build OpenXR VR Module as internal module', True))

params.Add(BoolVariable('with_static_internalmodules', 'Statically link internal modules', False))
params.Add(BoolVariable('with_engine_deal', 'Build Drag[en]gine Asset Library', True))

params.Add(TernaryVariable('build_igde', 'Build IGDE'))
params.Add(TernaryVariable('build_editor_animator', 'Build IGDE Animator Editor'))
params.Add(TernaryVariable('build_editor_conversation', 'Build IGDE Conversation Editor'))
params.Add(TernaryVariable('build_editor_font', 'Build IGDE Font Editor'))
params.Add(TernaryVariable('build_editor_gamedefinition', 'Build IGDE Game Definition Editor'))
params.Add(TernaryVariable('build_editor_langpack', 'Build IGDE Language Pack Editor'))
params.Add(TernaryVariable('build_editor_particleemitter', 'Build IGDE Particle Emitter Editor'))
params.Add(TernaryVariable('build_editor_rig', 'Build IGDE Rig Editor'))
params.Add(TernaryVariable('build_editor_skin', 'Build IGDE Skin Editor'))
params.Add(TernaryVariable('build_editor_sky', 'Build IGDE Sky Editor'))
params.Add(TernaryVariable('build_editor_speechanimation', 'Build IGDE Speech Animation Editor'))
params.Add(TernaryVariable('build_editor_synthesizer', 'Build IGDE Synthesizer Editor'))
params.Add(TernaryVariable('build_editor_world', 'Build IGDE World Editor'))

params.Add(EnumVariable('archive_format', 'Archive file format', 'tarbz2', ['tarbz2', 'zip']))
params.Add(StringVariable('archive_name_engine',
	'Archive file name without extension for Drag[en]gine archive', 'dragengine'))
params.Add(StringVariable('archive_name_engine_dev',
	'Archive file name without extension for Drag[en]gine Development archive', 'dragengine-dev'))
params.Add(StringVariable('archive_name_igde',
	'Archive file name without extension for IGDE archive', 'deigde'))
params.Add(StringVariable('archive_name_igde_dev',
	'Archive file name without extension for IGDE Development archive', 'deigde_dev'))
params.Add(StringVariable('archive_name_special',
	'Archive file name without extension for Special archive', 'despecial'))

params.Add(StringVariable('addon_name_blender',
	'Blender addon file name without extension', 'blender-addon-dragengine'))

params.Add(StringVariable('installer_name_engine',
	'Installer file name without extension for Drag[en]gine installer', 'install-dragengine'))
params.Add(StringVariable('installer_name_engine_dev',
	'Installer file name without extension for Drag[en]gine Development installer', 'install-dragengine-dev'))
params.Add(StringVariable('installer_name_launcher',
	'Installer file name without extension for Drag[en]gine Launcher installer', 'install-dragengine-launcher'))
params.Add(StringVariable('installer_name_igde',
	'Installer file name without extension for IGDE installer', 'install-deigde'))
params.Add(StringVariable('installer_name_igde_dev',
	'Installer file name without extension for IGDE Development installer', 'install-deigde-dev'))

params.Add(StringVariable('apk_name_launcher',
	'Android Launcher APK file name without extension', 'DELauncher'))
params.Add(StringVariable('android_version_code', 'Android version code', '99999'))


if parent_env['OSMacOS']:
	params.Add(TernaryVariable('with_dl', 'Use the dynamic library system'))
	params.Add(TernaryVariable('with_pthread', 'Use pthread'))
	params.Add(TernaryVariable('with_x', 'Use the X Window System'))
	
	params.Add(EnumVariable('igde_toolkit', 'ToolKit to use for building IGDE', 'null', ['null']))
	
	params.Add(PathVariable('prefix', 'System path', '/usr', PathVariable.PathAccept))
	params.Add(PathVariable('libdir', 'System libraries', '${prefix}/lib', PathVariable.PathAccept))
	params.Add(PathVariable('includedir', 'System includes', '${prefix}/include', PathVariable.PathAccept))
	params.Add(PathVariable('datadir', 'System shares', '${prefix}/share', PathVariable.PathAccept))
	params.Add(PathVariable('sysconfdir', 'System configuration', '/etc', PathVariable.PathAccept))
	params.Add(PathVariable('execdir', 'System binaries', '/Applications', PathVariable.PathAccept))
	params.Add(PathVariable('sysvardir', 'System var', '${prefix}/var', PathVariable.PathAccept))
	params.Add(PathVariable('cachedir', 'System cache', '${sysvardir}/lib', PathVariable.PathAccept))

	params.Add(PathVariable('path_de_bin', 'Path to the Drag[en]gine binaries',
		'${execdir}', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_lib', 'Path to the Drag[en]gine libraries',
		'${libdir}', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_include', 'Path to the Drag[en]gine headers',
		'${includedir}/dragengine', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_config', 'Path to the Drag[en]gine configuration',
		'${sysconfdir}/dragengine', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_data', 'Path to the Drag[en]gine data',
		'${libdir}/dragengine', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_share', 'Path to the Drag[en]gine shares',
		'${datadir}/dragengine', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_cache', 'Path to the Drag[en]gine cache',
		'${cachedir}/dragengine', PathVariable.PathAccept))

	params.Add(PathVariable('path_igde_bin', 'Path to the IGDE binaries',
		'${execdir}', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_lib', 'Path to the IGDE libraries',
		'${libdir}', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_include', 'Path to the IGDE headers',
		'${includedir}/deigde', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_config', 'Path to the IGDE configuration',
		'${sysconfdir}/deigde', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_data', 'Path to the IGDE data',
		'${libdir}/deigde', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_share', 'Path to the IGDE shares',
		'${datadir}/deigde', PathVariable.PathAccept))

	params.Add(PathVariable('path_launcher_bin', 'Path to the Launcher binaries',
		'${execdir}', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_bin_lib', 'Path to the Launcher binary libraries',
		'${libdir}', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_config', 'Path to the Launcher configuration',
		'${sysconfdir}/delauncher', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_data', 'Path to the Launcher data',
		'${libdir}/delauncher', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_share', 'Path to the Launcher shares',
		'${datadir}/delauncher', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_games', 'Path to the Launcher games',
		'/opt/delauncher/games', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_include', 'Path to the Launcher headers',
		'${includedir}/delauncher', PathVariable.PathAccept))
	
elif parent_env['OSBeOS']:
	params.Add(TernaryVariable('with_dl', 'Use the dynamic library system'))
	params.Add(TernaryVariable('with_pthread', 'Use pthread'))
	params.Add(TernaryVariable('with_x', 'Use the X Window System'))
	
	params.Add(EnumVariable('igde_toolkit', 'ToolKit to use for building IGDE', 'null', ['null']))
	
	params.Add(PathVariable('prefix', 'System path', '/boot/system', PathVariable.PathAccept))
	params.Add(PathVariable('libdir', 'System libraries', '${prefix}/lib', PathVariable.PathAccept))
	params.Add(PathVariable('includedir', 'System includes', '${prefix}/develop/include', PathVariable.PathAccept))
	params.Add(PathVariable('datadir', 'System shares', '${prefix}/data', PathVariable.PathAccept))
	params.Add(PathVariable('sysconfdir', 'System configuration', '${prefix}/settings', PathVariable.PathAccept))
	params.Add(PathVariable('execdir', 'System binaries', '${prefix}/bin', PathVariable.PathAccept))
	params.Add(PathVariable('sysvardir', 'System var', '${prefix}/settings', PathVariable.PathAccept))
	params.Add(PathVariable('cachedir', 'System cache', '${prefix}/cache', PathVariable.PathAccept))
	params.Add(PathVariable('docdir', 'System documentation', '${prefix}/documentation', PathVariable.PathAccept))

	params.Add(PathVariable('path_de_bin', 'Path to the Drag[en]gine binaries',
		'${execdir}', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_lib', 'Path to the Drag[en]gine libraries',
		'${libdir}', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_include', 'Path to the Drag[en]gine headers',
		'${includedir}/dragengine', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_config', 'Path to the Drag[en]gine configuration',
		'${sysconfdir}/dragengine', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_data', 'Path to the Drag[en]gine data',
		'${libdir}/dragengine', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_share', 'Path to the Drag[en]gine shares',
		'${datadir}/dragengine', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_cache', 'Path to the Drag[en]gine cache',
		'${cachedir}/dragengine', PathVariable.PathAccept))

	params.Add(PathVariable('path_igde_bin', 'Path to the IGDE binaries',
		'${execdir}', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_lib', 'Path to the IGDE libraries',
		'${libdir}', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_include', 'Path to the IGDE headers',
		'${includedir}/deigde', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_config', 'Path to the IGDE configuration',
		'${sysconfdir}/deigde', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_data', 'Path to the IGDE data',
		'${libdir}/deigde', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_share', 'Path to the IGDE shares',
		'${datadir}/deigde', PathVariable.PathAccept))

	params.Add(PathVariable('path_launcher_bin', 'Path to the Launcher binaries',
		'${execdir}', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_bin_lib', 'Path to the Launcher binary libraries',
		'${libdir}', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_config', 'Path to the Launcher configuration',
		'${sysconfdir}/delauncher', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_data', 'Path to the Launcher data',
		'${libdir}/delauncher', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_share', 'Path to the Launcher shares',
		'${datadir}/delauncher', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_games', 'Path to the Launcher games',
		'${sysvardir}/delauncher/games', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_include', 'Path to the Launcher headers',
		'${includedir}/delauncher', PathVariable.PathAccept))
	
elif parent_env['OSPosix']:
	params.Add(TernaryVariable('with_dl', 'Use the dynamic library system'))
	params.Add(TernaryVariable('with_pthread', 'Use pthread'))
	params.Add(TernaryVariable('with_x', 'Use the X Window System'))
	
	params.Add(EnumVariable('igde_toolkit', 'ToolKit to use for building IGDE', 'fox', ['fox','null']))
	
	params.Add(PathVariable('prefix', 'System path', '/usr', PathVariable.PathAccept))
	params.Add(PathVariable('libdir', 'System libraries', '${prefix}/lib', PathVariable.PathAccept))
	params.Add(PathVariable('includedir', 'System includes', '${prefix}/include', PathVariable.PathAccept))
	params.Add(PathVariable('datadir', 'System shares', '${prefix}/share', PathVariable.PathAccept))
	params.Add(PathVariable('sysconfdir', 'System configuration', '/etc', PathVariable.PathAccept))
	params.Add(PathVariable('execdir', 'System binaries', '${prefix}/bin', PathVariable.PathAccept))
	params.Add(PathVariable('sysvardir', 'System var', '/var', PathVariable.PathAccept))
	params.Add(PathVariable('cachedir', 'System cache', '${sysvardir}/lib', PathVariable.PathAccept))
	
	params.Add(PathVariable('path_de_bin', 'Path to the Drag[en]gine binaries',
		'${execdir}', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_lib', 'Path to the Drag[en]gine libraries',
		'${libdir}', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_include', 'Path to the Drag[en]gine headers',
		'${includedir}/dragengine', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_config', 'Path to the Drag[en]gine configuration',
		'${sysconfdir}/dragengine', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_data', 'Path to the Drag[en]gine data',
		'${libdir}/dragengine', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_share', 'Path to the Drag[en]gine shares',
		'${datadir}/dragengine', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_cache', 'Path to the Drag[en]gine cache',
		'${cachedir}/dragengine', PathVariable.PathAccept))
	
	params.Add(PathVariable('path_igde_bin', 'Path to the IGDE binaries',
		'${execdir}', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_lib', 'Path to the IGDE libraries',
		'${libdir}', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_include', 'Path to the IGDE headers',
		'${includedir}/deigde', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_config', 'Path to the IGDE configuration',
		'${sysconfdir}/deigde', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_data', 'Path to the IGDE data',
		'${libdir}/deigde', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_share', 'Path to the IGDE shares',
		'${datadir}/deigde', PathVariable.PathAccept))
	
	params.Add(PathVariable('path_launcher_bin', 'Path to the Launcher binaries',
		'${execdir}', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_bin_lib', 'Path to the Launcher binary libraries',
		'${libdir}', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_config', 'Path to the Launcher configuration',
		'${sysconfdir}/delauncher', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_data', 'Path to the Launcher data',
		'${libdir}/delauncher', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_share', 'Path to the Launcher shares',
		'${datadir}/delauncher', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_games', 'Path to the Launcher games',
		'/opt/delauncher/games', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_include', 'Path to the Launcher headers',
		'${includedir}/delauncher', PathVariable.PathAccept))
	
elif parent_env['OSWindows']:
	params.Add(EnumVariable('igde_toolkit', 'ToolKit to use for building IGDE', 'fox', ['fox','null']))
	
	params.Add(PathVariable('programfiles', 'Window program files directory',
		'@ProgramFiles', PathVariable.PathAccept))
	params.Add(PathVariable('systemroot', 'Window system root directory',
		'@System', PathVariable.PathAccept))
	
	params.Add(PathVariable('path_de_sdk', 'Path to Drag[en]gine SDK directory',
		'${path_de}/SDK', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_sdk_lib', 'Path to Drag[en]gine SDK libraries',
		'${path_de_sdk}/lib', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_sdk_inc', 'Path to Drag[en]gine SDK includes',
		'${path_de_sdk}/include', PathVariable.PathAccept))
	
	params.Add(PathVariable('path_de', 'Path to the Drag[en]gine Installation',
		'${programfiles}/Dragengine', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_bin', 'Path to the Drag[en]gine binaries',
		'${path_de}/Bin', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_lib', 'Path to the Drag[en]gine libraries',
		'${path_de_sdk_lib}', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_include', 'Path to the Drag[en]gine headers',
		'${path_de_sdk_inc}/dragengine', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_config', 'Path to the Drag[en]gine configuration',
		'${path_de}/Config', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_data', 'Path to the Drag[en]gine data',
		'${path_de}/Data', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_share', 'Path to the Drag[en]gine shares',
		'${path_de}/Share', PathVariable.PathAccept))
	params.Add(PathVariable('path_de_cache', 'Path to the Drag[en]gine cache',
		'@LocalAppData/Dragengine/Cache', PathVariable.PathAccept))
	
	params.Add(PathVariable('path_igde_sdk', 'Path to Drag[en]gine IGDE SDK directory',
		'${path_igde}/SDK', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_sdk_lib', 'Path to Drag[en]gine IGDE SDK libraries',
		'${path_igde_sdk}/lib', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_sdk_inc', 'Path to Drag[en]gine IGDE SDK includes',
		'${path_igde_sdk}/include', PathVariable.PathAccept))
	
	params.Add(PathVariable('path_igde', 'Path to the IGDE Installation',
		'${programfiles}/DEIGDE', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_bin', 'Path to the IGDE binaries',
		'${path_igde}/Bin', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_lib', 'Path to the IGDE libraries',
		'${path_igde_sdk_lib}', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_include', 'Path to the IGDE headers',
		'${path_igde_sdk_inc}/deigde', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_config', 'Path to the IGDE configuration',
		'${path_igde}/Config', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_data', 'Path to the IGDE data',
		'${path_igde}/Data', PathVariable.PathAccept))
	params.Add(PathVariable('path_igde_share', 'Path to the IGDE shares',
		'${path_igde}/Share', PathVariable.PathAccept))
	
	params.Add(PathVariable('path_launcher', 'Path to the Launcher Installation',
		'${path_de}/Launchers', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_bin', 'Path to the Launcher binaries',
		'${path_launcher}/Bin', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_bin_lib', 'Path to the Launcher binary libraries',
		'${path_launcher_bin}', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_config', 'Path to the Launcher configuration',
		'@RoamingAppData/DELaunchers/Config', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_data', 'Path to the Launcher data',
		'${path_launcher}/Data', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_share', 'Path to the Launcher shares',
		'${path_launcher}/Share', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_games', 'Path to the Launcher games',
		'@Public/DELaunchers/Games', PathVariable.PathAccept))
	
	params.Add(PathVariable('path_launcher_sdk', 'Path to Drag[en]gine Launcher SDK directory',
		'${path_launcher}/SDK', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_sdk_lib', 'Path to Drag[en]gine Launcher SDK libraries',
		'${path_launcher_sdk}/lib', PathVariable.PathAccept))
	params.Add(PathVariable('path_launcher_sdk_inc', 'Path to Drag[en]gine Launcher SDK includes',
		'${path_launcher_sdk}/include', PathVariable.PathAccept))
	
	params.Add(PathVariable('path_launcher_include', 'Path to the Launcher headers',
		'${path_de_sdk_inc}/delauncher', PathVariable.PathAccept))
	
	params.Add(BoolVariable('with_install_pdb', 'Install/archive PDB files', False))
	params.Add(PathVariable('path_pdb', 'Path to the PDBs',
		'${path_de}/PDB', PathVariable.PathAccept))
	
else:
	Exit('No supported OS found!')

params.Update(parent_env)
#print(parent_env.Dump())

# external flags
parent_env.Replace(EXTERN_CMAKE_FLAGS = shlex.split(parent_env['with_cmake_flags']))
parent_env.Replace(EXTERN_CMAKE_C_FLAGS = shlex.split(parent_env['with_cmake_c_flags']))

if parent_env['platform_android'] != 'no':
	parent_env.Append(EXTERN_CMAKE_C_FLAGS = ['-Wall'])

# determine sanitize flags to use
parent_env.Replace(SANITIZE_FLAGS = [])
parent_env.Replace(SANITIZE_LINK_FLAGS = [])

if parent_env['with_debug'] and parent_env['with_sanitize']:
	if parent_env['with_sanitize_thread']:
		parent_env.Append(SANITIZE_FLAGS = ['-fsanitize=thread'])
		
	else:
		flags = [
			'-fsanitize=address',
			'-fsanitize-address-use-after-scope',
			'-fsanitize=pointer-compare',
			'-fsanitize=pointer-subtract']
		flags.extend([
			'-fsanitize=leak'])
		flags.extend([
			'-fsanitize=undefined',
			'-fsanitize=shift',
			'-fsanitize=shift-exponent',
			'-fsanitize=shift-base',
			'-fsanitize=integer-divide-by-zero',
			'-fsanitize=unreachable',
			'-fsanitize=vla-bound',
			'-fsanitize=null',
			'-fsanitize=return',
			'-fsanitize=signed-integer-overflow',
			'-fsanitize=bounds',
			'-fsanitize=bounds-strict',
			'-fsanitize=alignment',
			'-fsanitize=object-size',
			'-fsanitize=float-divide-by-zero',
			'-fsanitize=float-cast-overflow',
			'-fsanitize=nonnull-attribute',
			'-fsanitize=returns-nonnull-attribute',
			'-fsanitize=bool',
			'-fsanitize=enum',
			'-fsanitize=vptr',
			'-fsanitize=pointer-overflow',
			'-fsanitize=builtin'])
		
		if parent_env['CROSSCOMPILE_CLANG'] or parent_env['platform_android'] != 'no':
			flags = ['-fsanitize=address']
		
		# newer asan can incorrectly flag warnings causing compilation to fail
		flags.append('-Wno-error')
		
		parent_env.Append(SANITIZE_FLAGS = flags)

# for modules hide everything except the entry point. for this the default visibility
# is set to hidden and only the entry point is qualified with normal visibility.
# hiding also inlines is an optimization and helps to remove some special cases.
# the version script is required to hide symbols of linked static libraries.
# the -s flag eventually strips unused code linked in by static libraries

if 'CROSSCOMPILE_CPPFLAGS' in parent_env:
	parent_env.Append(CROSSCOMPILE_NOSAN_CPPFLAGS = parent_env['CROSSCOMPILE_CPPFLAGS'])
	
if 'CROSSCOMPILE_LINKFLAGS' in parent_env:
	parent_env.Append(CROSSCOMPILE_NOSAN_LINKFLAGS = parent_env['CROSSCOMPILE_LINKFLAGS'])

if parent_env['with_debug']:
	parent_env.Append(MODULE_CPPFLAGS = ['-DMOD_ENTRY_POINT_ATTR='])
	if parent_env['with_sanitize']:
		parent_env.Append(MODULE_CPPFLAGS = parent_env['SANITIZE_FLAGS'])
		parent_env.Append(MODULE_LINKFLAGS = parent_env['SANITIZE_FLAGS'])
		parent_env.Append(MODULE_LINKFLAGS = parent_env['SANITIZE_LINK_FLAGS'])
		parent_env.Append(CROSSCOMPILE_CPPFLAGS = parent_env['SANITIZE_FLAGS'])
		parent_env.Append(CROSSCOMPILE_LINKFLAGS = parent_env['SANITIZE_FLAGS'])
		parent_env.Append(CROSSCOMPILE_LINKFLAGS = parent_env['SANITIZE_LINK_FLAGS'])
	parent_env.Replace(MODULE_LINKFLAGS_NOMODVER=parent_env['MODULE_LINKFLAGS'])
	
else:
	parent_env.Append(MODULE_CPPFLAGS = ['-fvisibility=hidden'])
	parent_env.Append(MODULE_LINKFLAGS = ['-fvisibility=hidden'])
	
	parent_env.Append(MODULE_CXXFLAGS = ['-fvisibility-inlines-hidden'])
	parent_env.Append(MODULE_LINKFLAGS = ['-fvisibility-inlines-hidden'])
	
	parent_env.Replace(MODULE_LINKFLAGS_NOMODVER=parent_env['MODULE_LINKFLAGS'])
	
	if not parent_env['OSMacOS']:
		parent_env.Append(MODULE_LINKFLAGS = ['-Wl,--version-script=module.version'])
		if not parent_env['with_debug_symbols']:
			parent_env.Append(MODULE_LINKFLAGS = ['-s'])
	
	if parent_env['platform_android'] != 'no':
		parent_env.Append(MODULE_LINKFLAGS = ['-Wl,--undefined-version'])
	
	parent_env.Append(MODULE_CPPFLAGS = ['-DMOD_ENTRY_POINT_ATTR=__attribute__\\(\\(visibility\\(\\"default\\"\\)\\)\\)'])

# android
if parent_env['platform_android'] != 'no':
	ndkroot = ''
	if 'NDK_ROOT' in os.environ:
		ndkroot = os.path.expanduser(os.environ['NDK_ROOT'])

	params.Add(PathVariable('ndkroot', 'Path to NDK toolchain (NDK_ROOT env-param by default)', ndkroot, PathVariable.PathAccept))
	params.Add(StringVariable('apilevel', 'Android API level', '18'))
	params.Add(BoolVariable('hardfp', 'Use hardware floating point support instead of softfp on ARMv7 only', False))

	params.Update(parent_env)

	androidUpdateEnv(parent_env)

# web wasm
if parent_env['platform_webwasm']:
	webWasmUpdateEnv(parent_env)

# disable verbose compile messages if requested
if not parent_env['with_verbose']:
	DisableVerboseCompiling(parent_env)

if parent_env['with_debug'] or parent_env['with_debug_symbols']:
	if parent_env['OSWindows']:
		if parent_env['CROSSCOMPILE_CLANG']:
			parent_env.Append(CPPFLAGS = ['-g', '-gcodeview'])
			parent_env.Append(LINKFLAGS = ['-Wl,-pdb='])
		else:
			parent_env.Append(CPPFLAGS = ['-g', '-gstabs', '-ggdb'])
	else:
		parent_env.Append(CPPFLAGS = ['-g'])
		# mingw produces internal compiler errors on 8.x GCC. disabled until fixed or a check is present
		#parent_env.Append(CPPFLAGS = ['-fno-omit-frame-pointer'])

# set flags based on parameters
if parent_env['platform_android'] == 'no':
	parent_env.Append(CPPFLAGS = ['-O2'])  # because android platform script defines this already
parent_env.Append(CPPFLAGS = ['-Wall'])

parent_env.Append(CPPFLAGS = ['-Wshadow', '-Wwrite-strings'])

# disable the new (and truely stupid) new gcc 8.1 shenanigans.
# see https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html#index-Wclass-memaccess .
# the idea behind all this is all nice and dandy but it prevents legit fast memory handling
# trying to pretend idiots from shooting themselves in the foot. if somebody uses memcpy then
# he should know what he is doing so stop breaking builds with non-sense errors.
# 
# and to add insult to injury MacOS does not understand the flag. great... just great
if (not parent_env['OSMacOS'] and not parent_env['CROSSCOMPILE_CLANG']
	and parent_env['platform_android'] == 'no' and not parent_env['platform_webwasm']):
	parent_env.Append(CXXFLAGS = ['-Wno-class-memaccess'])

if parent_env['with_warnerrors']:
	parent_env.Append(CPPFLAGS = ['-Werror'])

if parent_env['platform_android'] != 'no':
	parent_env.Append(CPPFLAGS = ['-Wno-unused-private-field'])
	parent_env.Append(CPPFLAGS = ['-Wno-tautological-constant-compare'])
	parent_env.Append(CPPFLAGS = ['-Wno-nontrivial-memcall'])

# clang uses some warnings we do not care about right now
if parent_env['CROSSCOMPILE_CLANG']:
	parent_env.Append(CPPFLAGS = ['-Wno-unused-function'])
	parent_env.Append(CPPFLAGS = ['-Wno-unused-private-field'])

# no default targets
Default(None)

# define the targets array and reports dictionary to be filled
parent_targets = {}
parent_report = {}

# report stuff
if parent_env['OSPosix']:
	parent_report['prefix'] = parent_env.subst(parent_env['prefix'])
	parent_report['dragengine binary path'] = parent_env.subst(parent_env['path_de_bin'])
	parent_report['dragengine library path'] = parent_env.subst(parent_env['path_de_lib'])
	parent_report['dragengine include path'] = parent_env.subst(parent_env['path_de_include'])
	parent_report['dragengine configuration path'] = parent_env.subst(parent_env['path_de_config'])
	parent_report['dragengine data path'] = parent_env.subst(parent_env['path_de_data'])
	parent_report['dragengine shared data path'] = parent_env.subst(parent_env['path_de_share'])
	parent_report['dragengine cache path'] = parent_env.subst(parent_env['path_de_cache'])

elif parent_env['OSWindows']:
	parent_report['system library path'] = parent_env.subst(parent_env['systemroot'])
	parent_report['dragengine sdk path'] = parent_env.subst(parent_env['path_de_sdk'])
	parent_report['dragengine path'] = parent_env.subst(parent_env['path_de'])
	parent_report['dragengine binary path'] = parent_env.subst(parent_env['path_de_bin'])
	parent_report['dragengine library path'] = parent_env.subst(parent_env['path_de_lib'])
	parent_report['dragengine include path'] = parent_env.subst(parent_env['path_de_include'])
	parent_report['dragengine configuration path'] = parent_env.subst(parent_env['path_de_config'])
	parent_report['dragengine data path'] = parent_env.subst(parent_env['path_de_data'])
	parent_report['dragengine shared data path'] = parent_env.subst(parent_env['path_de_share'])
	parent_report['deigde path'] = parent_env.subst(parent_env['path_igde'])
	parent_report['deigde binary path'] = parent_env.subst(parent_env['path_igde_bin'])
	parent_report['deigde library path'] = parent_env.subst(parent_env['path_igde_lib'])
	parent_report['deigde include path'] = parent_env.subst(parent_env['path_igde_include'])
	parent_report['deigde configuration path'] = parent_env.subst(parent_env['path_igde_config'])
	parent_report['deigde data path'] = parent_env.subst(parent_env['path_igde_data'])
	parent_report['deigde shared data path'] = parent_env.subst(parent_env['path_igde_share'])
	parent_report['dragengine cache path'] = parent_env.subst(parent_env['path_de_cache'])
	#parent_report['program files'] = parent_env.subst(parent_env['programfiles'])
	parent_report['delauncher games'] = parent_env.subst(parent_env['path_launcher_games'])

parent_report['platform_android'] = parent_env['platform_android']
if parent_env['platform_android'] != 'no':
	parent_report['ndkroot'] = parent_env.subst(parent_env['ndkroot'])
	parent_report['apilevel'] = parent_env['apilevel']
	parent_report['hardfp'] = 'yes' if parent_env['hardfp'] else 'no'

parent_report['platform_webwasm'] = 'yes' if parent_env['platform_webwasm'] else 'no'

parent_report['build dragengine tests'] = 'yes' if parent_env['with_tests'] else 'no'
parent_report['treat warnings as errors'] = 'yes' if parent_env['with_warnerrors'] else 'no'
parent_report['build with debug symbols'] = 'yes' if (
	parent_env['with_debug'] or parent_env['with_debug_symbols']) else 'no'
parent_report['build with sanitizing'] = 'yes' if parent_env['with_sanitize'] else 'no'
parent_report['build with thread sanitizing'] = 'yes' if parent_env['with_sanitize_thread'] else 'no'
parent_report['version'] = parent_env['version']



# external libraries
extdirs = []
extdirs.append('extern/zlib')
extdirs.append('extern/libpng')
extdirs.append('extern/libapng')
extdirs.append('extern/libjpeg')
extdirs.append('extern/freetype')
extdirs.append('extern/sndio')
extdirs.append('extern/jsoncpp')
extdirs.append('extern/openal')
extdirs.append('extern/openvr')
extdirs.append('extern/libogg')
extdirs.append('extern/libvorbis')
extdirs.append('extern/libtheora')
extdirs.append('extern/fox')
extdirs.append('extern/dragonscript')
extdirs.append('extern/libevdev')
extdirs.append('extern/soundtouch')
extdirs.append('extern/libvpx')
extdirs.append('extern/libwebp')
extdirs.append('extern/libwebm')
extdirs.append('extern/openxr')
extdirs.append('extern/steamsdk')
extdirs.append('extern/eossdk')
extdirs.append('extern/liburing')
extdirs.append('extern/modio')
extdirs.append('extern/deremotelauncher')
extdirs.append('extern/denetwork')
extdirs.append('extern/agdk')

for extdir in extdirs:
	SConscript(dirs=extdir, variant_dir='{}/build'.format(extdir),
		duplicate=0, exports='parent_env parent_targets parent_report')

SConscript(dirs='extern/mingw', variant_dir='extern/mingw/build',
	duplicate=0, exports='parent_env parent_targets parent_report')

# drag[en]gine game engine
SConscript(dirs='src/dragengine', variant_dir='src/dragengine/build', duplicate=0,
	exports='parent_env parent_targets parent_report')

scdirs = []

# shared libraries
scdirs.append('src/shared/vulkan')

# game engine modules
scdirs.append('src/modules/animator/deanimator')

scdirs.append('src/modules/ai/deai')

scdirs.append('src/modules/animation/deanim')

scdirs.append('src/modules/audio/null')
scdirs.append('src/modules/audio/openal')

scdirs.append('src/modules/crashrecovery/simplyquit')
scdirs.append('src/modules/crashrecovery/basicrecovery')

scdirs.append('src/modules/font/defont')
scdirs.append('src/modules/font/freetype')

scdirs.append('src/modules/graphic/null')
scdirs.append('src/modules/graphic/opengl')
scdirs.append('src/modules/graphic/vulkan')

scdirs.append('src/modules/image/png')
scdirs.append('src/modules/image/png3d')
scdirs.append('src/modules/image/tga')
scdirs.append('src/modules/image/jpeg')
scdirs.append('src/modules/image/ies')
scdirs.append('src/modules/image/webp')
scdirs.append('src/modules/image/webp3d')

scdirs.append('src/modules/input/console')
scdirs.append('src/modules/input/xsystem')
scdirs.append('src/modules/input/w32input')
scdirs.append('src/modules/input/beos')
scdirs.append('src/modules/input/macos')
scdirs.append('src/modules/input/android')

scdirs.append('src/modules/langpack/delangpack')

scdirs.append('src/modules/model/demodel')

scdirs.append('src/modules/network/basic')

scdirs.append('src/modules/occlusionmesh/deocclusionmesh')

scdirs.append('src/modules/physics/bullet')

scdirs.append('src/modules/rig/derig')

scdirs.append('src/modules/scripting/dragonscript')
#scdirs.append('src/modules/scripting/python')

scdirs.append('src/modules/skin/deskin')

scdirs.append('src/modules/sound/oggvorbis')

scdirs.append('src/modules/synthesizer/desynthesizer')

scdirs.append('src/modules/video/theora')
scdirs.append('src/modules/video/apng')
scdirs.append('src/modules/video/webm')

scdirs.append('src/modules/archive/delga')

scdirs.append('src/modules/combined/fbx')

scdirs.append('src/modules/vr/null')
scdirs.append('src/modules/vr/openvr')
scdirs.append('src/modules/vr/openxr')

scdirs.append('src/modules/service/steamsdk')
scdirs.append('src/modules/service/eossdk')
scdirs.append('src/modules/service/modio')

# engine asset library
scdirs.append('src/dragengine/asset')

# launchers
scdirs.append('src/launcher/shared')
scdirs.append('src/launcher/console')
scdirs.append('src/launcher/gui')
scdirs.append('src/launcher/android')
scdirs.append('src/launcher/live')
scdirs.append('src/launcher/web')

# tests
scdirs.append('src/tests')

# integrated game development environment
scdirs.append('src/deigde/deigde')

scdirs.append('src/deigde/editors/world')
scdirs.append('src/deigde/editors/animator')
scdirs.append('src/deigde/editors/rigeditor')
scdirs.append('src/deigde/editors/font')
scdirs.append('src/deigde/editors/sky')
scdirs.append('src/deigde/editors/skin')
scdirs.append('src/deigde/editors/particleEmitter')
scdirs.append('src/deigde/editors/speechAnimation')
scdirs.append('src/deigde/editors/conversation')
scdirs.append('src/deigde/editors/langpack')
scdirs.append('src/deigde/editors/synthesizer')
scdirs.append('src/deigde/editors/gameDefinition')
scdirs.append('src/deigde/editors/project')

for scdir in scdirs:
	SConscript(dirs=scdir, variant_dir='{}/build'.format(scdir),
		duplicate=0, exports='parent_env parent_targets parent_report')

#tools
SConscript(dirs='src/tools/blender', variant_dir='src/tools/blender/build',
	duplicate=0, exports='parent_env parent_targets parent_report')

# special stuff
SConscript(dirs='src/launcher/usbdrive', variant_dir='src/launcher/usbdrive/build',
	duplicate=0, exports='parent_env parent_targets parent_report')



# archiving
SConscript(dirs='archive', variant_dir='archive/build',
	duplicate=0, exports='parent_env parent_targets parent_report')

"""
SConscript('src/tools/launcher/android/SConscriptSpecial',
	variant_dir='src/tools/launcher/android/build_apk',
	duplicate=0, exports='parent_env parent_targets parent_report')
"""

SConscript('archive/SConsHaikuHpkg.py', variant_dir='archive/build/haiku',
	duplicate=0, exports='parent_env parent_targets parent_report')



# installers
SConscript(dirs='installer', variant_dir='installer/build',
	duplicate=0, exports='parent_env parent_targets parent_report')


#params.Save('parameters.cache', parent_env)

# add aliases
buildAll = []
installAll = []
installAllRuntime = []
installEngineRuntime = []
installIgdeRuntime = []
doxygenAll = []
clocAll = []
clocReports = []

for key in parent_targets:
	if 'build' in parent_targets[key]:
		buildAll.append(parent_targets[key]['build'])
	
	if 'install' in parent_targets[key]:
		installAll.append(parent_targets[key]['install'])
		if 'install-runtime' in parent_targets[key]:
			installAllRuntime.append(parent_targets[key]['install-runtime'])
		else:
			installAllRuntime.append(parent_targets[key]['install'])
	
	if 'install-engine-runtime' in parent_targets[key]:
		installEngineRuntime.append(parent_targets[key]['install-engine-runtime'])
	
	if 'install-igde-runtime' in parent_targets[key]:
		installIgdeRuntime.append(parent_targets[key]['install-igde-runtime'])
	
	if 'doxygen' in parent_targets[key]:
		doxygenAll.append(parent_targets[key]['doxygen'])
	
	if 'cloc' in parent_targets[key]:
		clocAll.append(parent_targets[key]['cloc'])
		clocReports.append(parent_targets[key]['clocReport'])

targetBuildAll = parent_env.Alias('build', buildAll)
parent_targets['build'] = {
	'name' : 'Build Everything',
	'target' : targetBuildAll}

targetInstallAll = parent_env.Alias('install', installAll)
parent_targets['install'] = {
	'name' : 'Install Everything',
	'target' : targetInstallAll}

targetInstallAllRuntime = parent_env.Alias('install_runtime', installAllRuntime)
parent_targets['install_runtime'] = {
	'name' : 'Install Everything Runtime (no development files)',
	'target' : targetInstallAllRuntime}

targetInstallEngineRuntime = parent_env.Alias('install_engine_runtime', installEngineRuntime)
parent_targets['install_engine_runtime'] = {
	'name' : 'Install Engine Runtime (no development files)',
	'target' : targetInstallEngineRuntime}

targetInstallIgdeRuntime = parent_env.Alias('install_igde_runtime', installIgdeRuntime)
parent_targets['install_igde_runtime'] = {
	'name' : 'Install IGDE Runtime (no development files)',
	'target' : targetInstallIgdeRuntime}

targetDoxygenAll = parent_env.Alias('doxygen', doxygenAll)
parent_targets['doxygen'] = {
	'name' : 'Doxygen Everything',
	'target' : targetDoxygenAll}

if clocReports:
	targetClocSummary = parent_env.Alias('cloc_summary',
		BuildCLOCSummary(parent_env, clocReports, 'clocsummary.csv'))
	clocAll.append(targetClocSummary)

targetClocAll = parent_env.Alias('cloc', clocAll)
parent_targets['cloc'] = {
	'name' : 'CLoc Everything',
	'target' : targetClocAll}

# default is building and installing everything
Default('install')



# produce help
Help(BuildHelpText(parent_targets))

# print out report
PrintConfigReport(parent_report)
