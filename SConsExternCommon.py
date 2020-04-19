from SCons.Util import flatten


# create script header with various exports for compiler and flags.
# returns list of script lines.
# 
# env: Environment to fins parameters in
# deps: List of parent targets to depend on.
# cppflags: Optional list of manual cpp flags
# linkflags: Optional list of manual link flags
def createScriptHeader(env, deps=[], cflags=[], cppflags=[], cxxflags=[], linkflags=[], pkgconfig=[]):
	script = []
	
	# append all known compiler declarations if present
	exportCommand = 'export'
	if env['HostOSWindows']:
		exportCommand = 'SET'
	for x in ['CC', 'CXX', 'LD', 'AS', 'AR', 'STRIP', 'MAKE', 'RC', 'RANLIB', 'NM', 'NASM']:
		if x in env:
			script.append('{} {}="{}"'.format(exportCommand, x, env.subst(env[x])))
	
	# combine all flags into one array to later write out to the script
	combinedCFlags = []
	combinedCPPFlags = []
	combinedCXXFlags = []
	combinedLinkFlags = []
	combinedPkgConfig = []
	
	# append the cross compiler flags if present
	if 'CROSSCOMPILE_CFLAGS' in env:
		combinedCFlags.extend( env[ 'CROSSCOMPILE_CFLAGS' ] )
	
	if 'CROSSCOMPILE_CPPFLAGS' in env:
		combinedCPPFlags.extend( env[ 'CROSSCOMPILE_CPPFLAGS' ] )
	
	if 'CROSSCOMPILE_CXXFLAGS' in env:
		combinedCXXFlags.extend( env[ 'CROSSCOMPILE_CXXFLAGS' ] )
	
	if 'CROSSCOMPILE_LINKFLAGS' in env:
		combinedLinkFlags.extend( env[ 'CROSSCOMPILE_LINKFLAGS' ] )
	
	if 'CROSSCOMPILE_PKGCONFIG' in env:
		combinedPkgConfig.extend( env[ 'CROSSCOMPILE_PKGCONFIG' ] )
	
	# append manual flags
	combinedCFlags.extend( cflags )
	combinedCPPFlags.extend( cppflags )
	combinedCXXFlags.extend( cxxflags )
	combinedLinkFlags.extend( linkflags )
	combinedPkgConfig.extend(pkgconfig)
	
	# append required flags for dependencies
	for dep in deps:
		if 'cflags' in dep:
			combinedCFlags.extend( dep[ 'cflags' ] )
		
		if 'cppflags' in dep:
			combinedCPPFlags.extend( dep[ 'cppflags' ] )
		
		if 'cxxflags' in dep:
			combinedCXXFlags.extend( dep[ 'cxxflags' ] )
		
		if 'cpppath' in dep:
			for path in dep[ 'cpppath' ]:
				combinedCPPFlags.append( '-I{}'.format(
					env.Dir( '#{}'.format( path ) ).abspath ) )
		
		if 'libpath' in dep:
			for path in dep[ 'libpath' ]:
				combinedLinkFlags.append( '-L{}'.format(
					env.Dir( '#{}'.format( path ) ).abspath ) )
		
		if 'pkgconfig' in dep:
			for path in dep['pkgconfig']:
				combinedPkgConfig.append(env.Dir('#{}'.format(path)).abspath)
	
	# write flags to script
	if combinedCFlags:
		script.append( 'export CFLAGS="{}"'.format( ' '.join(
			[ env.subst( x ) for x in flatten( combinedCFlags ) ] ) ) )
	
	if combinedCPPFlags:
		script.append( 'export CPPFLAGS="{}"'.format( ' '.join(
			[ env.subst( x ) for x in flatten( combinedCPPFlags ) ] ) ) )
	
	if combinedCXXFlags:
		script.append( 'export CXXFLAGS="{}"'.format( ' '.join(
			[ env.subst( x ) for x in flatten( combinedCXXFlags ) ] ) ) )
	
	if combinedLinkFlags:
		script.append( 'export LDFLAGS="{}"'.format( ' '.join(
			[ env.subst( x ) for x in flatten( combinedLinkFlags ) ] ) ) )
	
	if combinedPkgConfig:
		script.append('export PKG_CONFIG_PATH="{}"'.format(':'.join(
			[env.subst(x) for x in flatten(combinedPkgConfig)])))
	
	# return finished script lines
	return script
