from SCons.Script import Dir
from SCons.Util import flatten
import os

def webWasmUpdateEnv(env):
	if env['platform_webwasm']:
		try:
			env.Tool('emscripten', toolpath=[os.environ['EMSCRIPTEN_TOOL_PATH']])
		except Exception as e:
			print('EMScripten missing. Build using "emscons scons ..."')
			raise e
	else:
		raise SCons.Errors.UserError('Invalid web platform %s'.format(env['platform_web']))
	
	env.Append(CPPFLAGS = ['-DWEB_WASM'])
	
	# disable some nag warnings which are plain out stupid
	env.Append(CPPFLAGS = ['-Wno-nontrivial-memcall'])

	env.Append(LINKFLAGS = ['-sMIN_WEBGL_VERSION=2'])
	env.Append(LINKFLAGS = ['-sOFFSCREENCANVAS_SUPPORT'])
	env.Append(LINKFLAGS = ['-sOFFSCREEN_FRAMEBUFFER'])
	env.Append(LINKFLAGS = ['-sFULL_ES3'])
	
	# env.Append(LINKFLAGS = ['-s', 'SIDE_MODULE=1'])
	
	env.Append(MODULE_LINKFLAGS = ['-sSIDE_MODULE=1'])
	env.Append(MODULE_LINKFLAGS_NOMODVER = ['-sSIDE_MODULE=1'])
	
	env['CROSSCOMPILE_CFLAGS'] = []
	env['CROSSCOMPILE_CPPFLAGS'] = []
	env['CROSSCOMPILE_CXXFLAGS'] = []
	env['CROSSCOMPILE_LINKFLAGS'] = []
	
	env.Append(CROSSCOMPILE_CXXFLAGS = ['-sDISABLE_EXCEPTION_CATCHING=0'])
	env.Append(CROSSCOMPILE_LINKFLAGS = ['-sDISABLE_EXCEPTION_CATCHING=0'])
	
	env.Append(CROSSCOMPILE_CFLAGS = ['-O3'])
	env.Append(CROSSCOMPILE_CXXFLAGS = ['-O3'])
	
	env.Append(CROSSCOMPILE_LINKFLAGS = ['-sALLOW_MEMORY_GROWTH'])
	
	env.Append(CROSSCOMPILE_CFLAGS = ['-pthread'])
	env.Append(CROSSCOMPILE_CXXFLAGS = ['-pthread'])
	env.Append(CROSSCOMPILE_LINKFLAGS = ['-pthread'])
	
	if env['with_debug']:
		env.Append(CROSSCOMPILE_CFLAGS = ['-g3', '-gsource-map'])
		env.Append(CROSSCOMPILE_CXXFLAGS = ['-g3', '-gsource-map'])
		env.Append(CROSSCOMPILE_LINKFLAGS = ['-g3', '-gsource-map'])
	
	env.Append(CFLAGS = env['CROSSCOMPILE_CFLAGS'])
	env.Append(CPPFLAGS = env['CROSSCOMPILE_CPPFLAGS'])
	env.Append(CXXFLAGS = env['CROSSCOMPILE_CXXFLAGS'])
	env.Append(LINKFLAGS = env['CROSSCOMPILE_LINKFLAGS'])
	
	import threading
	env['_mutexRunActionSynchronized'] = threading.Lock()
