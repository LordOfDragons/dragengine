import os

from SCons.Builder import Builder
from SCons.Action import Action

# Provides support to use Haiku 'rc' tool to compile '*.rdef' into '*.rsrc'.
# 
# Adds builder RDef()

# SCons Tools required this method to be present. It is called if exists() returns true
def generate(env):
	def builderRdef(env, target, source):
		if os.system('/bin/rc -d -o "{}" "{}"'.format(target[0].abspath, source[0].abspath)):
			raise Exception('Failed building')
	
	env.Append(BUILDERS={'RDef': Builder(action=Action(
		builderRdef, '$RDEFCOMSTR'), suffix='.rsrc', src_suffix='.rdef')})
	env['RDEFCOM'] = 'Compiling Resource "$TARGET"'
	env['RDEFCOMSTR'] = env['RDEFCOM']

# SCons Tools required this method to be present. Checks if tool is applicable
def exists(env):
	return True
