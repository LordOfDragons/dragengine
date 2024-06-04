import sys, re
from os import system

# Provides running calls protected by a synchronization lock to ensure only one call runs
# at the same time. A typical use for this is wrapping os.system() calls.
# 
# Required for certain build commands required to run non-threaded even if the user has
# run external command
# 
# A typical use is like this:
# 
# env.Tool('runExternalCommand')
# 
# env.RunExternalCommand('binary argument')  # using os.system()
# env.RunExternalCommand(['binary1 argument1', 'binary2 argument2'])  # using os.system()
# 
# Calls return True on success or False otherwise. Raising exceptions is allowed

# SCons Tools required this method to be present. It is called if exists() returns true
def generate(env):
	def runExternalCommand(commands):
		if isinstance(commands, list):
			try:
				if system('\n'.join(commands)):
					print('RunExternalCommand failed for script:')
					for c in commands:
						print(c)
					return False
			except:
				print('RunExternalCommand failed for script:')
				for c in commands:
					print(c)
				raise
		else:
			try:
				if system(commands):
					print('RunExternalCommand failed for script:')
					print(command)
					return False
			except:
				print('RunExternalCommand failed for script:')
				print(command)
				raise
		return True
	
	env.RunExternalCommand = runExternalCommand

# SCons Tools required this method to be present. Checks if tool is applicable
def exists(env):
	return True
