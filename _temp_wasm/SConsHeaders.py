from SConsCommon import *
import os

# import whatever is given by the parent builder
Import('parent_env parent_targets parent_report')

# setup the environment matching the OS
envDragengine = parent_env.Clone()

libs = []

if envDragengine['OSBeOS']:
	libs.extend(['root', 'be'])
	
elif envDragengine['OSMacOS']:
	envDragengine.Append(FRAMEWORKS = ['Cocoa'])
	
elif envDragengine['OSPosix']:
	pass
	
elif envDragengine['OSWindows']:
	libs.extend(envDragengine['WINDOWS_LIBS'])
	libs.append('winmm')
	#libs.extend(['kernel32', 'user32', 'gdi32', 'winmm', 'wsock32', 'advapi32'])
	"""if envDragengine['with_debug']:
		lib.append('dbghelp')
		envDragengine.Append(CXXFLAGS = ['-DWITH_DBGHELP'])"""
	
else:
	Exit('No supported OS found!')

if envDragengine['platform_android'] != 'no':
	libs.extend(envDragengine['ANDROID_LIBS'])

binlibs = []
linklibs = ['dragengine']

# do configure stuff
conf = envDragengine.Configure()

if envDragengine['OSPosix'] or envDragengine['OSMacOS']:
	hasDL = False
	if envDragengine['with_dl'] != TernaryVariableNo:
		hasDL = conf.CheckLibWithHeader('dl', 'dlfcn.h', 'c++', 'dlerror();')
		if hasDL:
			envDragengine.Append(CXXFLAGS = '-DHAS_LIB_DL')
			if envDragengine['platform_android'] == 'no':
				libs.append('dl')
				binlibs.append('dl')
		elif envDragengine['with_dl'] == TernaryVariableYes:
			print('Requested to use dynamic library system but dlfcn.h or libdl could not be found or is not usable.')
			Exit(1)
	
	hasPThreads = False
	if envDragengine['platform_android'] != 'no':
		envDragengine.Append(CXXFLAGS = '-DHAS_LIB_PTHREAD')
		hasPThreads = True # special implementation bundled in OS. no library include
	elif envDragengine['with_pthread'] != TernaryVariableNo:
		hasPThreads = conf.CheckLibWithHeader('pthread', 'pthread.h', 'c++', 'pthread_exit(NULL);')
		if hasPThreads:
			envDragengine.Append(CXXFLAGS = '-DHAS_LIB_PTHREAD')
			libs.append('pthread')
			binlibs.append('pthread')
		else:
			hasPThreads = conf.CheckLibWithHeader('pthreads', 'pthread.h', 'c++', 'pthread_exit(NULL);')
			if hasPThreads:
				envDragengine.Append(CXXFLAGS = '-DHAS_LIB_PTHREAD')
				libs.append('pthreads')
				binlibs.append('pthreads')
			else:
				hasPThreads = conf.CheckLibWithHeader('root', 'pthread.h', 'c++', 'pthread_exit(NULL);')
				if hasPThreads:
					envDragengine.Append(CXXFLAGS = '-DHAS_LIB_PTHREAD')
					# no lib append since root is already included
				elif envDragengine['with_pthread'] == TernaryVariableYes:
					print('Requested to use pthreads but pthread.h or libpthread could not be found or is not usable.')
					Exit(1)
	
	hasX11 = False
	if envDragengine['with_x'] != TernaryVariableNo:
		hasX11 = conf.CheckLibWithHeader('X11', 'X11/Xlib.h', 'c++', 'XOpenDisplay(NULL);')
		if hasX11:
			envDragengine.Append(CXXFLAGS = '-DHAS_LIB_X11')
			libs.append('X11')
			binlibs.append('X11')
			
			if conf.CheckLibWithHeader('Xrandr', 'X11/extensions/Xrandr.h', 'c++', 'XRRQueryVersion(NULL,NULL,NULL);'):
				envDragengine.Append(CXXFLAGS = '-DHAS_LIB_XRANDR')
				libs.append('Xrandr')
				binlibs.append('Xrandr')
			else:
				Exit('X11 is used but no Xrandr extension is not found.')
			
		elif envDragengine['with_x'] == TernaryVariableYes:
			print('Requested to use X Window System but Xlib.h or libX11 could not be found or is not usable.')
			Exit(1)
	
	# android specific problems
	#hasUtimensat = conf.CheckFunc('utimensat', 'sys/stat.h')
	hasPthreadCancel = conf.CheckFunc('pthread_cancel')

conf.Finish()

# fetch values in expanded form for later use
pathInclude = envDragengine.subst(envDragengine['path_de_include'])

versionString = envDragengine['version']
if envDragengine['force_version']:
	versionString = envDragengine['force_version']

libVersionString = versionString
while len(libVersionString.split('.')) < 3:
	libVersionString = '{}.0'.format(libVersionString)

# write the configuration file required by a few source files
def buildWriteConfig(source, target, env):
	configFilePath = target[0].abspath
	configFileDefines = {}
	configFileDefines['DE_VERSION'] = versionString
	configFileDefines['OS_UNIX'] = env['OSPosix']
	configFileDefines['OS_UNIX_X11'] = (env['OSPosix'] and hasX11
		and not env['OSWindows']
		and not env['OSBeOS']
		and not env['OSMacOS']
		and env['platform_android'] == 'no'
		and not env['platform_webwasm'])
	configFileDefines['OS_W32'] = env['OSWindows']
	configFileDefines['OS_BEOS'] = env['OSBeOS']
	configFileDefines['OS_MACOS'] = env['OSMacOS']
	configFileDefines['OS_ANDROID'] = env['platform_android'] != 'no'
	configFileDefines['OS_WEBWASM'] = env['platform_webwasm']
	if env['platform_android'] == 'quest':
		configFileDefines['OS_ANDROID_QUEST'] = True
	if env['OSPosix'] or env['OSMacOS']:
		configFileDefines['HAS_LIB_X11'] = hasX11
		configFileDefines['HAS_LIB_PTHREAD'] = hasPThreads
		configFileDefines['HAS_LIB_DL'] = hasDL
		# android specific problems
		#configFileDefines['HAS_FUNC_UTIMENSAT'] = hasUtimensat
		configFileDefines['HAS_FUNC_PTRHEAD_CANCEL'] = hasPthreadCancel
	
	if WriteConfigFile(configFilePath, configFileDefines, env):
		raise Exception('Writing configuration file failed')

targetConfig = envDragengine.Command(
	envDragengine.File('src/dragengine_configuration.h').srcnode(),
	'SConsHeaders.py', buildWriteConfig)

# setup the builders
headers = []
globFiles(envDragengine, 'src', '*.h', headers)

ignoreHeaders = []
ignoreHeaders.append('src/doxy_main.h')

headers = [envDragengine.File(h).srcnode() for h in headers if not h in ignoreHeaders]
headers.extend(targetConfig)
envDragengine['_headers'] = headers

copyHeaders = []
refDir = envDragengine.Dir('src').srcnode()
for h in headers:
	copyHeaders.append(envDragengine.Command(
		os.path.join('include', 'dragengine', refDir.rel_path(h)),
		h, Copy('$TARGET', '$SOURCE')))

installHeaders = [envDragengine.InstallAs(os.path.join(pathInclude, refDir.rel_path(h)), h) for h in headers]
installDevelop = installHeaders[:]

archiveDevelop = {os.path.join(pathInclude, refDir.rel_path(h)):h for h in headers}

# add the targets to the targets list
cpppath = [envDragengine.Dir('include')]

internalModules = []

def addInternalModule(env, nodes, externLibs, createFunc, syslibs=None, priority=False):
	internalModules.append({
		'env': env,
		'nodes': nodes,
		'externLibs': externLibs,
		'createFunc': createFunc,
		'syslibs': syslibs or [],
		'priority': priority})

envDragengine['_libs'] = libs
envDragengine['_versionString'] = versionString
envDragengine['_libVersionString'] = libVersionString
envDragengine['_targetConfig'] = targetConfig
envDragengine['_installDevelop'] = installDevelop
envDragengine['_internalModules'] = internalModules

parent_targets['dragengine'] = {
	'name': 'Drag[en]gine Game Engine',
	'transfer.env': envDragengine,
	'cpppath': cpppath,
	'binlibs': binlibs,
	'libs': linklibs,
	'headers': copyHeaders,
	'archive-engine-develop': archiveDevelop,
	'add internal module': addInternalModule}

# report
if envDragengine['OSPosix'] or envDragengine['OSMacOS']:
	if hasDL:
		parent_report['use dynamic libraries'] = 'yes'
	else:
		parent_report['use dynamic libraries'] = 'no'
	if hasX11:
		parent_report['use X Window System'] = 'yes'
	else:
		parent_report['use dynamic libraries'] = 'no'
