from SCons.Script import Dir
from SCons.Util import flatten
import os


# Create android build environment
def androidUpdateEnv( env ):
	if env['platform_android'] == 'armv7':
		compiler = 'armv7a-linux-androideabi{}'.format(env['apilevel'])
		compiler2 = 'arm-linux-androideabi'
		arch = 'arm'
		host = 'arm-linux'
		fullarch = 'armeabi-v7a'
		jnidir = 'armeabi-v7a'
		
	elif env['platform_android'] == 'armv8':
		compiler = 'aarch64-linux-android{}'.format(env['apilevel'])
		compiler2 = 'aarch64-linux-android'
		arch = 'aarch'
		host = 'aarch64-linux'
		fullarch = 'arm64-v8a'
		jnidir = 'arm64-v8a'

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
	pathToolchain = os.path.join(ndkroot, 'toolchains', 'llvm', 'prebuilt', 'linux-x86_64')
	#pathSysroot = '{}/toolchains/llvm/prebuilt/linux-x86_64/sysroot'.format(ndkroot)
	pathBin = os.path.join(pathToolchain, 'bin')
	pathCompiler = os.path.join(pathBin, compiler)
	pathCompiler2 = os.path.join(pathBin, compiler2)
	cmakeToolchain = os.path.join(ndkroot, 'build', 'cmake', 'android.toolchain.cmake')
	
	env['CC'] = '{}-clang'.format(pathCompiler)
	env['CXX'] = '{}-clang++'.format(pathCompiler)
	env['LD'] = '{}-ld'.format(pathCompiler2)
	env['STRIP'] = '{}-strip'.format(pathCompiler2)
	env['OBJCOPY'] = '{}-objcopy'.format(pathCompiler2)
	env['AS'] = '{}-as'.format(pathCompiler2)
	env['AR'] = '{}-ar'.format(pathCompiler2)
	env['RANLIB'] = '{}-ranlib'.format(pathCompiler2)
	env['NASM'] = '{}/yasm'.format(pathBin)
	
	# newer NDK changed compiler file names. how lovely U_U
	if env.Detect(os.path.join(pathBin, 'llvm-ar')):
		pathCompiler2 = os.path.join(pathBin, 'llvm')
		env['LD'] = '{}-link'.format(pathCompiler2)
		env['STRIP'] = '{}-strip'.format(pathCompiler2)
		env['OBJCOPY'] = '{}-objcopy'.format(pathCompiler2)
		env['AS'] = '{}-as'.format(pathCompiler2)
		env['AR'] = '{}-ar'.format(pathCompiler2)
		env['RANLIB'] = '{}-ranlib'.format(pathCompiler2)
	
	#env['ANDROID_SYSROOT'] = pathSysroot
	env['ANDROID_BIN'] = pathBin
	env['ANDROID_COMPILER'] = compiler
	env['ANDROID_HOST'] = host
	env['ANDROID_JNIDIR'] = jnidir
	env['ANDROID_ARCH'] = arch
	env['ANDROID_ABI'] = fullarch
	env['ANDROID_APILEVEL'] = env['apilevel']
	env['ANDROID_HARDFP'] = env['platform_android'] == 'armv7' and env['hardfp']
	env['ANDROID_CMAKETOOLCHAIN'] = cmakeToolchain
	env['ANDROID_NDKROOT'] = ndkroot
	
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
