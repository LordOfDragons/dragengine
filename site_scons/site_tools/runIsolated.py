import sys, re

# Provides running calls protected by a synchronization lock to ensure only one call runs
# at the same time. A typical use for this is wrapping os.system() calls.
# 
# Required for certain build commands required to run non-threaded even if the user has
# threaded building enabled by default. For example CMake based projects can dead-loop if
# run in parallel for example using cross-platform builds running in parallel.
# 
# A typical use is like this:
# 
# env.Tool('runIsolated')
# 
# env.RunIsolated(function)
# env.RunIsolatedSystem('binary argument')  # using os.system()
# env.RunIsolatedSystem(['binary1 argument1', 'binary2 argument2'])  # using os.system()
# 
# Calls return True on success or False otherwise. Raising exceptions is allowed
# 

# SCons Tools required this method to be present. It is called if exists() returns true
def generate(env):
	from os import system
	from threading import Lock
	
	lockGlobal = Lock()
	
	groups = {}
	lockGroups = Lock()
	
	def runIsolated(function, group=None):
		if group:
			with lockGroups as locked:
				if not group in groups:
					groups[group] = Lock()
				lock = groups[group]
		else:
			lock = lockGlobal
		
		with lock as locked:
			try:
				return not function()
			except:
				print('runIsolated failed for function: {}'.format(function))
				raise
	
	def runIsolatedSystem(commands, group=None):
		if group:
			with lockGroups as locked:
				if not group in groups:
					groups[group] = Lock()
				lock = groups[group]
		else:
			lock = lockGlobal
		
		with lock as locked:
			if isinstance(commands, list):
				try:
					return not system('\n'.join(commands))
				except:
					print('runIsolatedSystem failed for script:')
					for c in commands:
						print(c)
					raise
			else:
				try:
					return not system(commands)
				except:
					print('runIsolatedSystem failed for script:')
					print(command)
					raise
	
	env.RunIsolated = runIsolated
	env.RunIsolatedSystem = runIsolatedSystem

# SCons Tools required this method to be present. Checks if tool is applicable
def exists(env):
	return True
