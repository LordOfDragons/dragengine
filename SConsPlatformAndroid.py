from SCons.Script import Dir
from SCons.Util import flatten


# Create android build environment
def androidUpdateEnv( env ):
	if env['platform_android'] == 'armv7':
		compiler = 'armv7a-linux-androideabi{}'.format(env['apilevel'])
		compiler2 = 'arm-linux-androideabi'
		arch = 'arm'
		host = 'arm-linux'
		fullarch = 'armeabi-v7a'
		jnidir = 'armeabi-v7a'
		
	elif env['platform_android'] == 'x86':
		compiler = 'i686-linux-androideabi{}'.format(env['apilevel'])
		compiler2 = 'i686-linux-androideabi'
		arch = 'x86'
		host = 'i686-linux'
		fullarch = 'x86'
		jnidir = 'x86'
		
	else:
		raise SCons.Errors.UserError('Invalid android platform %s'.format(env['platform_android']))
	
	ndkroot = env.subst(env['ndkroot'])
	pathToolchain = '{}/toolchains/llvm/prebuilt/linux-x86_64'.format(ndkroot)
	#pathSysroot = '{}/toolchains/llvm/prebuilt/linux-x86_64/sysroot'.format(ndkroot)
	pathBin = '{}/bin'.format(pathToolchain)
	pathCompiler = '{}/{}'.format(pathBin, compiler)
	pathCompiler2 = '{}/{}'.format(pathBin, compiler2)
	
	env['CC'] = '{}-clang'.format(pathCompiler)
	env['CXX'] = '{}-clang++'.format(pathCompiler)
	env['LD'] = '{}-ld'.format(pathCompiler2)
	env['STRIP'] = '{}-strip'.format(pathCompiler2)
	env['OBJCOPY'] = '{}-objcopy'.format(pathCompiler2)
	env['AS'] = '{}-as'.format(pathCompiler2)
	env['AR'] = '{}-ar'.format(pathCompiler2)
	env['RANLIB'] = '{}-ranlib'.format(pathCompiler2)
	env['NASM'] = '{}/yasm'.format(pathBin)
	
	#env['ANDROID_SYSROOT'] = pathSysroot
	env['ANDROID_BIN'] = pathBin
	env['ANDROID_COMPILER'] = compiler
	env['ANDROID_HOST'] = host
	env['ANDROID_JNIDIR'] = jnidir
	env['ANDROID_APILEVEL'] = env['apilevel']
	env['ANDROID_HARDFP'] = env['platform_android'] == 'armv7' and env['hardfp']
	
	env.Append(ANDROID_LIBS = ['m', 'z', 'log', 'c', 'android'])
	
	env.Append(CPPFLAGS = ['-DANDROID={}'.format(env['apilevel'])])
	env.Append(CPPFLAGS = ['-O3'])
	
	env['CROSSCOMPILE_HOST'] = host
	env['CROSSCOMPILE_CFLAGS'] = []
	env['CROSSCOMPILE_CPPFLAGS'] = []
	env['CROSSCOMPILE_CXXFLAGS'] = []
	env['CROSSCOMPILE_LINKFLAGS'] = []
	
	if env['ANDROID_HARDFP']:
		env.Append(CROSSCOMPILE_CFLAGS = ['-D_NDK_MATH_NO_SOFTFP=1'])
		env.Append(CROSSCOMPILE_CPPFLAGS = ['-D_NDK_MATH_NO_SOFTFP=1'])
	
	#env.Append(CROSSCOMPILE_CXXFLAGS = ['-Wno-unused-private-field'])
	
	env.Append(CFLAGS = env['CROSSCOMPILE_CFLAGS'])
	env.Append(CPPFLAGS = env['CROSSCOMPILE_CPPFLAGS'])
	env.Append(CXXFLAGS = env['CROSSCOMPILE_CXXFLAGS'])
	env.Append(LINKFLAGS = env['CROSSCOMPILE_LINKFLAGS'])
	
	import threading
	env['_mutexRunActionSynchronized'] = threading.Lock()

# NOTE hard floating point calling convention has been dropped from NDK in r12 upwards. The reason
#      is there are many problems and the observed benefits are due to compiler optimization bugs
#      not due to the use of soft float calling convention. According to Google there is no speed
#      gain in using hard-float calling convention so this is not done here. In particular the
#      following is not permitted anymore:
#        - do not use library m_hard instead of m
#        - do not use compiler flags -mfloat-abi=hard nor -mfloat-abi=softfp (gcc not allowed)
#        - do not use compiler flags -mhard-float (redundant by now)
#      This only affects the calling convention. Using hard-floating point support itself is still
#      supported but just using soft-float calling convention to avoid ABI problems
def oldAndroidUpdateEnv( env ):
	gccVersion = '4.9'
	stlsupport = 'gnu-libstdc++'
	
	if env[ 'platform_android' ] == 'armv7':
		compiler = 'arm-linux-androideabi'
		arch = 'arm'
		host = 'arm-linux'
		fullarch = 'armeabi-v7a'
		jnidir = 'armeabi-v7a'
		
	elif env[ 'platform_android' ] == 'x86':
		compiler = 'i686-linux-android'
		arch = 'x86'
		host = 'i686-linux'
		fullarch = 'x86'
		jnidir = 'x86'
		
	else:
		raise SCons.Errors.UserError( 'Invalid android platform %s'.format( env[ 'platform_android' ] ) )
	
	ndkroot = env.subst( env[ 'ndkroot' ] )
	pathToolchain = '{}/toolchains/{}-{}/prebuilt/linux-x86_64'.format( ndkroot, compiler, gccVersion )
	pathBin = '{}/bin'.format( pathToolchain )
	pathSysroot = '{}/platforms/android-{}/arch-{}'.format( ndkroot, env[ 'apilevel' ], arch )
	pathCompiler = '{}/{}'.format( pathBin, compiler )
	
	env[ 'CC' ] = '{}-gcc'.format( pathCompiler )
	env[ 'CXX' ] = '{}-g++'.format( pathCompiler )
	env[ 'LD' ] = '{}-ld'.format( pathCompiler )
	env[ 'STRIP' ] = '{}-strip'.format( pathCompiler )
	env[ 'OBJCOPY' ] = '{}-objcopy'.format( pathCompiler )
	env[ 'AS' ] = '{}-as'.format( pathCompiler )
	env[ 'AR' ] = '{}-ar'.format( pathCompiler )
	env[ 'RANLIB' ] = '{}-ranlib'.format( pathCompiler )
	
	if env[ 'platform_android' ] == 'x86':
		env[ 'NASM' ] = '{}/yasm'.format( pathBin )
	
	env[ 'ANDROID_SYSROOT' ] = pathSysroot
	env[ 'ANDROID_BIN' ] = pathBin
	env[ 'ANDROID_COMPILER' ] = compiler
	env[ 'ANDROID_HOST' ] = host
	env[ 'ANDROID_JNIDIR' ] = jnidir
	env[ 'ANDROID_HARDFP' ] = ( env[ 'platform_android' ] == 'armv7' and env[ 'hardfp' ] )
	
	# libraries to link against required for libraries and binaries to load on android.
	# additional libraries can be required. android is in general quite picky about
	# these libraries and the loading order
	env.Append( ANDROID_LIBS = [ 'm', 'z', 'log', 'c', 'android' ] )
	
	# libs.append( 'gnustl_static' ) # stl support using static gnustl
	env.Append( CPPFLAGS = [ '--sysroot={}'.format( pathSysroot ) ] )
	env.Append( CPPFLAGS = [ '-DANDROID={}'.format( env[ 'apilevel' ] ) ] )
	env.Append( CPPFLAGS = [ '-O3' ] ) # default is -O2 so try to squeeze out a bit more speed if possible
	#env.Append( CROSSCOMPILE_CPPFLAGS = [ '-fsigned-char' ] ) # force char to be signed char although android has unsiged char as default
	#from libjpeg-turbo
	#if env[ 'platform_android' ] == 'armv7':
	#	env.Append( CROSSCOMPILE_CPPFLAGS = [ '-march=armv7-a', '-mfloat-abi=softfp', '-mfpu=neon', '-O3' ] )
	
	env.Append( LINKFLAGS = [ '--sysroot={}'.format( pathSysroot ) ] )
	
	env[ 'CROSSCOMPILE_CFLAGS' ] = []
	env[ 'CROSSCOMPILE_CPPFLAGS' ] = []
	env[ 'CROSSCOMPILE_CXXFLAGS' ] = []
	env[ 'CROSSCOMPILE_LINKFLAGS' ] = []
	
	# sysroot
	env.Append( CROSSCOMPILE_CPPFLAGS = [ '--sysroot={}'.format( pathSysroot ) ] )
	env.Append( CROSSCOMPILE_LINKFLAGS = [ '--sysroot={}'.format( pathSysroot ) ] )
	
	# stl support
	pathStl = '{}/sources/cxx-stl/{}/{}'.format( ndkroot, stlsupport, gccVersion )
	
	env.Append(ANDROID_LIBS = ['supc++'])  # required for stl and c++ support in general
	env.Append( CROSSCOMPILE_CXXFLAGS = [ '-fexceptions' ] ) # enable exception support
	#env.Append( CROSSCOMPILE_CXXFLAGS = [ '-frtti' ] ) # enable rtti support (for stl)
	env.Append( CROSSCOMPILE_CXXFLAGS = [ '-fno-rtti' ] ) # disable rtti support (for stl)
	env.Append( CROSSCOMPILE_CPPFLAGS = [ '"-I{}/include"'.format( pathStl ) ] )
	env.Append( CROSSCOMPILE_CPPFLAGS = [ '"-I{}/libs/{}/include"'.format( pathStl, fullarch ) ] )
	
	#env.Append( CROSSCOMPILE_LINKFLAGS = [ '-fexceptions' ] ) # enable exception support
	env.Append( CROSSCOMPILE_LINKFLAGS = [ '"-L{}/libs/{}"'.format( pathStl, fullarch ) ] )
	env.Append( CROSSCOMPILE_LINKFLAGS = [ '-no-canonical-prefixes' ] ) # from toolchains/arm-linux-androideabi-4.9/setup.mk
	
	# armv7a support
	if env[ 'platform_android' ] == 'armv7':
		env.Append( CROSSCOMPILE_CPPFLAGS = [ '-march=armv7-a', '-mthumb', '-mfpu=vfpv3-d16', '-mfpu=neon' ] )
		env.Append( CROSSCOMPILE_LINKFLAGS = [ '-march=armv7-a', '-mthumb', '-Wl,--fix-cortex-a8' ] )
	
	# hardware floating point support
	if env[ 'ANDROID_HARDFP' ]:
		env.Append( CROSSCOMPILE_CFLAGS = '-D_NDK_MATH_NO_SOFTFP=1' )
		env.Append( CROSSCOMPILE_CPPFLAGS = '-D_NDK_MATH_NO_SOFTFP=1' )
		# silence compiler warning about abi mismatch. this is side-effect which can not be prevented
		env.Append( CROSSCOMPILE_LINKFLAGS = [ '-Wl,--no-warn-mismatch' ] )
	
	# apply android special flags
	env.Append( CFLAGS = env[ 'CROSSCOMPILE_CFLAGS' ] )
	env.Append( CPPFLAGS = env[ 'CROSSCOMPILE_CPPFLAGS' ] )
	env.Append( CXXFLAGS = env[ 'CROSSCOMPILE_CXXFLAGS' ] )
	env.Append( LINKFLAGS = env[ 'CROSSCOMPILE_LINKFLAGS' ] )
