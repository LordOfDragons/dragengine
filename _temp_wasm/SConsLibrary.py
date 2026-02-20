from SConsCommon import *
import os

# import whatever is given by the parent builder
Import('parent_env parent_targets parent_report')

# setup the environment matching the OS
envDragengine = parent_targets['dragengine']['transfer.env']

buildStatic = False
# if envDragengine['platform_webwasm']:
	# buildStatic = True

# fetch values in expanded form for later use
pathLib = envDragengine.subst(envDragengine['path_de_lib'])
pathConfig = envDragengine.subst(envDragengine['path_de_config'])
pathEngine = envDragengine.subst(envDragengine['path_de_data'])
pathShare = envDragengine.subst(envDragengine['path_de_share'])
pathCache = envDragengine.subst(envDragengine['path_de_cache'])

if envDragengine['OSWindows']:
	pathSysLib = envDragengine.subst(envDragengine['systemroot'])
	#pathSysLib = envDragengine.subst(envDragengine['path_de'])

versionString = envDragengine['_versionString']
libVersionString = envDragengine['_libVersionString']

targetConfig = envDragengine['_targetConfig']

# update the compile flags
# envDragengine.Append(CXXFLAGS=' -D__DEBUG__')
# envDragengine.Append(CXXFLAGS=' -D_TESTING_')
envDragengine.Append(CXXFLAGS = [
	cmdlineDefinePath(envDragengine, 'DE_ENGINE_PATH', pathEngine),
	cmdlineDefinePath(envDragengine, 'DE_CONFIG_PATH', pathConfig),
	cmdlineDefinePath(envDragengine, 'DE_SHARE_PATH', pathShare),
	cmdlineDefinePath(envDragengine, 'DE_CACHE_PATH', pathCache)])

if envDragengine['OSWindows']:
	pathEngineBase = envDragengine.subst(envDragengine['path_de'])
	envDragengine.Append(CXXFLAGS = [
		cmdlineDefinePath(envDragengine, 'DE_ENGINE_BASE_PATH', pathEngineBase)])

if envDragengine['platform_android'] != 'no':
	archMap = {
		'armv7': 'armv7',
		'armv8': 'armv8',
		'x86': 'x860',
		'quest': 'armv8'}
	
	envDragengine.Append(CXXFLAGS = [
		cmdlineDefinePath(envDragengine, 'ANDROID_ARCH', archMap[envDragengine['platform_android']]),
		cmdlineDefinePath(envDragengine, 'ANDROID_JNIDIR', envDragengine['ANDROID_JNIDIR'])])

if 'mingw' in envDragengine['TOOLS']:
	# this line is important or else we have to ship libstdc++-6.dll and libgcc_s_sjlj-1.dll alongside
	# the build library which is a pain. if this library depends on another cross-compiled library
	# comment out this line or the static inclusion is duplicate causing errors
	pass #envDragengine.Append(LINKFLAGS = ['-static', '-static-libgcc', '-static-libstdc++'])

envDragengine.Replace(WINDOWS_INSERT_DEF = 1) # generated .def file if dll is build

if envDragengine['OSWindows']:
	envDragengine.Append(LINKFLAGS = ['-Wl,--export-all-symbols'])

if envDragengine['OSMacOS']:
	envDragengine.Append(LINKFLAGS = ['-install_name', '@rpath/libdragengine.dylib'])

if envDragengine['platform_webwasm']:
	envDragengine.Append(MODULE_LINKFLAGS = ['-sSIDE_MODULE=1'])
	envDragengine.Append(LINKFLAGS = ['-sSIDE_MODULE=1'])

# seems to affect only 32-bit compiler version
#if envDragengine['OSBeOS']:
#	envDragengine.Append(CXXFLAGS = '-Wno-multichar')

envDragengine.Append(CPPFLAGS = envDragengine['SANITIZE_FLAGS'])
envDragengine.Append(LINKFLAGS = envDragengine['SANITIZE_FLAGS'])

# setup the builders
sources = []
globFiles(envDragengine, 'src', '*.cpp', sources)
if envDragengine['OSMacOS']:
	globFiles(envDragengine, 'src', '*.mm', sources)

sources.append('base64/base64.cpp')
envDragengine.Append(CPPPATH = ['base64'])

if envDragengine['with_static_internalmodules']:
	envDragengine.Append(CPPFLAGS=['-DWITH_STATIC_INTERNALMODULES'])
	
	envDragengine['InternalModuleCreateFunctions'] = []
	envDragengine['InternalModuleCreateFunctionsPriority'] = []
	
	def createInternalModulesBuild(target, source, env):
		with open(target[0].abspath, 'w') as f:
			f.write('/* Generated File. Do not edit */\n')
			for cf in env['InternalModuleCreateFunctionsPriority']:
				f.write('extern deTObjectReference<deInternalModule> {}(deModuleSystem*);\n'.format(cf))
			for cf in env['InternalModuleCreateFunctions']:
				f.write('extern deTObjectReference<deInternalModule> {}(deModuleSystem*);\n'.format(cf))
			f.write('\n')
			f.write('static deModuleSystem::FPRegisterInternalModule vInternalModuleFunctionsPriority[]{\n')
			for cf in env['InternalModuleCreateFunctionsPriority']:
				f.write('\t{},\n'.format(cf))
			f.write('\tnullptr};\n')
			f.write('\n')
			f.write('static deModuleSystem::FPRegisterInternalModule vInternalModuleFunctions[]{\n')
			for cf in env['InternalModuleCreateFunctions']:
				f.write('\t{},\n'.format(cf))
			f.write('\tnullptr};\n')
	
	targetCreateInternalModules = envDragengine.Command(
		'deModuleSystem_generated.cpp', targetConfig, envDragengine.Action(
			createInternalModulesBuild, 'Generate Internal Modules Source'))
	
	envDragengine.Depends('src/systems/deModuleSystem.cpp', targetCreateInternalModules)

objects = [envDragengine.SharedObject(s) for s in sources]

if envDragengine['HostOSWindows']:
	cut = len(objects) // 2
	objgroup1 = envDragengine.StaticLibrary(target='objgroup1', source=objects[:cut])
	objgroup2 = envDragengine.StaticLibrary(target='objgroup2', source=objects[cut:])
	objects = objgroup1 + objgroup2

libs = envDragengine['_libs']
libs.extend(parent_targets['lib_zlib']['libs'])
envDragengine.Append(CPPPATH = parent_targets['lib_zlib']['cpppath'])
envDragengine.Append(LIBPATH = parent_targets['lib_zlib']['libpath'])

if envDragengine['OSPosix']:
	envDragengine.Replace(SHLIBVERSION = libVersionString)

envDragengine.Depends(sources, targetConfig)

cpppath = parent_targets['dragengine']['cpppath']

# internal modules
internalModuleNodes = []
internalModuleLibs = []
internalModuleLibPath = []
internalModuleCppPath = []
internalModuleCppFlags = []
internalModuleLinkFlags = []
internalModuleCreateFunctions = []
internalModuleCreateFunctionsPriority = []

for m in envDragengine['_internalModules']:
	internalModuleLibs.extend(m['syslibs'])
	"""
	for x in reversed(m['syslibs']):
		if not x in internalModuleLibs:
			internalModuleLibs.insert(0, x)
	"""
	
	for el in m['externLibs']:
		elt = parent_targets[el]
		if 'libs' in elt:
			for x in reversed(elt['libs']):
				if not x in internalModuleLibs:
					internalModuleLibs.insert(0, x)
		if 'libpath' in elt:
			for x in elt['libpath']:
				if not x in internalModuleLibPath:
					internalModuleLibPath.append(x)
		if 'cpppath' in elt:
			for x in elt['cpppath']:
				if not x in internalModuleCppPath:
					internalModuleCppPath.append(x)
		if 'cppflags' in elt:
			for x in elt['cppflags']:
				if not x in internalModuleCppFlags:
					internalModuleCppFlags.append(x)
		if 'linkflags' in elt:
			for x in elt['linkflags']:
				if not x in internalModuleLinkFlags:
					internalModuleLinkFlags.append(x)
	
	internalModuleNodes.extend(m['nodes'])
	
	if m['priority']:
		internalModuleCreateFunctionsPriority.append(m['createFunc'])
	else:
		internalModuleCreateFunctions.append(m['createFunc'])

if envDragengine['with_static_internalmodules']:
	libs.extend(internalModuleLibs)
	cpppath.extend(internalModuleCppPath)
	envDragengine.Append(CPPPATH = internalModuleCppPath)
	envDragengine.Append(LIBPATH = internalModuleLibPath)
	envDragengine.Append(CPPFLAGS = internalModuleCppFlags)
	envDragengine.Append(LINKFLAGS = internalModuleLinkFlags)
	envDragengine['InternalModuleCreateFunctions'].extend(internalModuleCreateFunctions)
	envDragengine['InternalModuleCreateFunctionsPriority'].extend(internalModuleCreateFunctionsPriority)
	
	objects.extend(internalModuleNodes)

# library
if buildStatic:
	library = envDragengine.StaticLibrary(target='dragengine', source=objects, LIBS=libs)
else:
	library = envDragengine.SharedLibrary(target='dragengine', source=objects, LIBS=libs)

targetBuild = envDragengine.Alias('dragengine_build', library)

refDir = envDragengine.Dir('src').srcnode()
envDragengine.Depends(library, parent_targets['dragengine']['headers'])

installDevelop = envDragengine['_installDevelop']

installRuntime = []

archiveDevelop = parent_targets['dragengine']['archive-engine-develop']
archiveEngine = {}

if not buildStatic:
	if envDragengine['OSPosix'] or envDragengine['OSBeOS']:
		for l in library:
			lname = l.name
			installRuntime.append(envDragengine.InstallVersionedLib(pathLib, l))
			archiveEngine['{}/{}'.format(pathLib, l.name)] = l
			
			lname = '.'.join(lname.split('.')[0:-2])
			archiveEngine['{}/{}'.format(pathLib, lname)] = envDragengine.File(lname)
			
			lname = '.'.join(lname.split('.')[0:-1])
			archiveEngine['{}/{}'.format(pathLib, lname)] = envDragengine.File(lname)

	elif envDragengine['OSWindows']:
		installRuntime.append(envDragengine.Install(pathSysLib, 'dragengine.dll'))
		installDevelop.append(envDragengine.Install(pathLib, 'dragengine.lib'))
		installDevelop.append(envDragengine.Install(pathLib, 'dragengine.def'))
		archiveEngine['{}/dragengine.dll'.format(pathSysLib)] = envDragengine.File('dragengine.dll')
		archiveDevelop['{}/dragengine.lib'.format(pathLib)] = envDragengine.File('dragengine.lib')
		archiveDevelop['{}/dragengine.def'.format(pathLib)] = envDragengine.File('dragengine.def')
		
		if envDragengine['CROSSCOMPILE_CLANG'] and envDragengine['with_install_pdb']:
			pathPdb = envDragengine.subst(envDragengine['path_pdb'])
			installRuntime.append(envDragengine.Install(pathPdb, 'dragengine.pdb'))
			archiveEngine['{}/dragengine.pdb'.format(pathPdb)] = envDragengine.File('dragengine.pdb')


if envDragengine['OSPosix'] and not envDragengine['OSBeOS'] and envDragengine['platform_android'] == 'no':
	pathDataDir = envDragengine.subst('${datadir}')
	
	for s in ['128', '64', '32']:
		target = os.path.join(pathDataDir, 'icons', 'hicolor',
			'{}x{}'.format(s, s), 'mimetypes', 'dragengine-delga.png')
		source = 'data/mime-dragengine-delga{}.png'.format(s)
		installRuntime.append(envDragengine.InstallAs(target, source))
		archiveEngine[target] = envDragengine.File(source).srcnode()
		
		target = os.path.join(pathDataDir, 'icons', 'hicolor',
			'{}x{}'.format(s, s), 'mimetypes', 'dragengine-game.png')
		source = 'data/mime-dragengine-game{}.png'.format(s)
		installRuntime.append(envDragengine.InstallAs(target, source))
		archiveEngine[target] = envDragengine.File(source).srcnode()
	
	target = os.path.join(pathDataDir, 'mime', 'packages', 'dragengine-dragengine.xml')
	source = 'data/mimetypes.xml'
	installRuntime.append(envDragengine.InstallAs(target, source))
	archiveEngine[target] = envDragengine.File(source).srcnode()


targetInstallDevelop = envDragengine.Alias('dragengine_develop', installDevelop)
targetInstallRuntime = envDragengine.Alias('dragengine_runtime', installRuntime)
targetInstall = envDragengine.Alias('dragengine', [targetInstallRuntime, targetInstallDevelop])

# source directory required for special commands
srcdir = Dir('src').srcnode().abspath

# doxygen
doxyfile = envDragengine.Command(envDragengine.File('Doxyfile').srcnode(), 'Doxyfile.in',
	Action(UpdateModuleManifest),
	ManifestUpdates=[{'action': 'text', 'value': versionString, 'keyword': '%VERSION%'}] )

targetDoxygen = envDragengine.Alias('dragengine_doc', envDragengine.Command(
	'doc/html/index.html', doxyfile, 'cd {} && doxygen ../Doxyfile'.format(srcdir)))
#parent_targets['dragengine_doc'] = ['Drag[en]gine Documentation', targetDoxygen]

# cloc
targetClocBare = BuildCLOC(envDragengine, [srcdir], '{}/clocreport.csv'.format(srcdir), ['doc'])
Depends(targetClocBare, sources)
Depends(targetClocBare, envDragengine['_headers'])
targetCloc = envDragengine.Alias('dragengine_cloc', targetClocBare)

# add the targets to the targets list
libpath = [library[0].get_dir()]
binlibs = []

if not 'mingw' in envDragengine['TOOLS']:
	cpppath.extend(parent_targets['lib_zlib']['cpppath'])
	libpath.extend(parent_targets['lib_zlib']['libpath'])
	binlibs.extend(parent_targets['lib_zlib']['libs'])

parent_targets['dragengine']['libpath'] = libpath
parent_targets['dragengine']['binlibs'] = binlibs
parent_targets['dragengine']['build'] = targetBuild
parent_targets['dragengine']['install'] = targetInstall
parent_targets['dragengine']['install-runtime'] = targetInstallRuntime
parent_targets['dragengine']['install-engine-runtime'] = targetInstallRuntime
parent_targets['dragengine']['library'] = library
parent_targets['dragengine']['archive-engine'] = archiveEngine
parent_targets['dragengine']['doxygen'] = targetDoxygen
parent_targets['dragengine']['cloc'] = targetCloc
parent_targets['dragengine']['clocReport'] = '{}/clocreport.csv'.format(srcdir)
parent_targets['dragengine']['libs'] = library

# internal module handling
if not envDragengine['with_static_internalmodules']:
	envModules = parent_env.Clone()
	
	modulesPath = '{}/modules'.format(envModules.subst(envModules['path_de_data']))
	
	modulesLibs = []
	appendLibrary(envModules, parent_targets['dragengine'], modulesLibs)
	modulesLibs.extend(internalModuleLibs)
	
	modulesInstall = []
	modulesArchive = {}
	
	envModules['InternalModuleCreateFunctions'] = []
	envModules['InternalModuleCreateFunctionsPriority'] = []
	
	def createInternalModulesBuild(target, source, env):
		with open(target[0].abspath, 'w') as f:
			f.write('/* Generated File. Do not edit */\n')
			f.write('#include <dragengine/systems/deModuleSystem.h>\n')
			f.write('\n')
			for cf in env['InternalModuleCreateFunctionsPriority']:
				f.write('extern deTObjectReference<deInternalModule> {}(deModuleSystem*);\n'.format(cf))
			for cf in env['InternalModuleCreateFunctions']:
				f.write('extern deTObjectReference<deInternalModule> {}(deModuleSystem*);\n'.format(cf))
			f.write('\n')
			f.write('static const deModuleSystem::FPRegisterInternalModule vInternalModuleFunctionsPriority[]{\n')
			for cf in env['InternalModuleCreateFunctionsPriority']:
				f.write('\t{},\n'.format(cf))
			f.write('\tnullptr};\n')
			f.write('\n')
			f.write('static const deModuleSystem::FPRegisterInternalModule vInternalModuleFunctions[]{\n')
			for cf in env['InternalModuleCreateFunctions']:
				f.write('\t{},\n'.format(cf))
			f.write('\tnullptr};\n')
			f.write('\n')
			f.write('extern "C" {\n')
			f.write('MOD_ENTRY_POINT_ATTR const deModuleSystem::FPRegisterInternalModule* DEInternalModuleFunctionsPriority(){\n')
			f.write('\treturn (deModuleSystem::FPRegisterInternalModule*)&vInternalModuleFunctionsPriority;\n')
			f.write('}\n')
			f.write('MOD_ENTRY_POINT_ATTR const deModuleSystem::FPRegisterInternalModule* DEInternalModuleFunctions(){\n')
			f.write('\treturn (deModuleSystem::FPRegisterInternalModule*)&vInternalModuleFunctions;\n')
			f.write('}\n')
			f.write('}\n')
	
	envModules.Append(CPPFLAGS = parent_env['MODULE_CPPFLAGS'])
	envModules.Append(LINKFLAGS = parent_env['MODULE_LINKFLAGS_NOMODVER'])
	
	if not envModules['OSMacOS']:
		envModules.Append(MODULE_LINKFLAGS = ['-Wl,--version-script=modules.version'])
	
	if envModules['platform_webwasm']:
		envModules.Append(LINKFLAGS = ['-sSIDE_MODULE=1'])
	
	envModules.Append(CPPPATH = internalModuleCppPath)
	envModules.Append(LIBPATH = internalModuleLibPath)
	envModules.Append(CPPFLAGS = internalModuleCppFlags)
	envModules.Append(LINKFLAGS = internalModuleLinkFlags)
	envModules['InternalModuleCreateFunctions'].extend(internalModuleCreateFunctions)
	envModules['InternalModuleCreateFunctionsPriority'].extend(internalModuleCreateFunctionsPriority)
	
	modulesTargetCreateInternalModules = envModules.Command(
		'deInternalModulesMain_generated.cpp', targetConfig, envModules.Action(
			createInternalModulesBuild, 'Generate Internal Modules Source'))
	
	modulesObjects = [envModules.SharedObject(modulesTargetCreateInternalModules)]
	modulesLibs.extend(internalModuleNodes)
	
	modulesLibrary = envModules.LoadableModule(target='demodules', source=modulesObjects, LIBS=modulesLibs)
	modulesTargetBuild = envModules.Alias('deinternalmodules_build', modulesLibrary)
	
	for l in modulesLibrary:
		if l.name.endswith(envModules['SHLIBSUFFIX']):
			modulesInstall.append(envModules.Install(modulesPath, l))
			modulesArchive[os.path.join(modulesPath, l.name)] = l
		
		if envModules['OSWindows'] and envModules['CROSSCOMPILE_CLANG'] and envModules['with_install_pdb']:
			pathPdb = os.path.join(envModules.subst(envModules['path_pdb']), 'EngineModules')
			modulesInstall.append(envModules.Install(pathPdb, 'demodules.pdb'))
			modulesArchive['{}/demodules.pdb'.format(pathPdb)] = envModules.File('demodules.pdb')
	
	modulesTargetInstall = envModules.Alias('deinternalmodules', modulesInstall)
	
	parent_targets['deinternalmodules'] = {
		'name': 'Drag[en]gine Internal Modules',
		'library': modulesLibrary,
		'build': modulesTargetBuild,
		'install': modulesTargetInstall,
		'install-engine-runtime': modulesTargetInstall,
		'archive-engine': modulesArchive}
