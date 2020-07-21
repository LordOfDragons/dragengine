import os

from SCons.Builder import Builder
from SCons.Action import Action

# Provides support to use Haiku 'rc' tool to compile '*.rdef' into '*.rsrc'.
# Provides support to use Haiku 'xres' tool to append '*.rsrc' to binary.
# 
# Adds builder RDef() and Rsrc()
# 
# Note about xres. Xres modifies binary in-place which is a problem since SCons
# expects all builders to produce unique files so dependencies can be tracked.
# To solve this we create first a copy of the binary and then apply xres to it.
# For this to work the binary has to be used as first source and the output
# name has to be given explicitly.

# SCons Tools required this method to be present. It is called if exists() returns true
def generate(env):
	def builderRdef(env, target, source):
		if os.system('/bin/rc -o "{}" "{}"'.format(target[0].abspath, source[0].abspath)):
			raise Exception('Failed building')
	
	def builderRsrc(env, target, source):
		if os.system('cp -a "{}" "{}"'.format(source[0].abspath, target[0].abspath)):
			raise Exception('Failed building')
		if os.system('/bin/xres -o "{}" "{}"'.format(target[0], source[1].abspath)):
			raise Exception('Failed building')
	
	env.Append(BUILDERS={'RDef': Builder(action=Action(
		builderRdef, '$RDEFCOMSTR'), suffix='.rsrc', src_suffix='.rdef')})
	env['RDEFCOM'] = 'Compiling Resource RDef "$TARGET"'
	env['RDEFCOMSTR'] = env['RDEFCOM']
	
	env.Append(BUILDERS={'Rsrc': Builder(action=Action(
		builderRsrc, '$RSRCCOMSTR'), suffix='', src_suffix='.rsrc')})
	env['RSRCCOM'] = 'Compiling Resource Rsrc "$TARGET"'
	env['RSRCCOMSTR'] = env['RSRCCOM']

# SCons Tools required this method to be present. Checks if tool is applicable
def exists(env):
	return True
