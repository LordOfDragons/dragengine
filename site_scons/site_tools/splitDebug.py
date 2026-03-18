def generate(env):
	def splitDebugAction(target, source, env):
		libPath = str(target[0])
		debugPath = libPath + '.debug'
		
		commands = [
			"objcopy --only-keep-debug {} {}".format(libPath, debugPath),
			"strip --strip-debug --strip-unneeded {}".format(libPath),
			"objcopy --add-gnu-debuglink={} {}".format(debugPath, libPath)]
		
		return 0 if all(env.Execute(c) == 0 for c in commands) else 1
	
	def SplitDebug(env, target):
		"""Splits target into a stripped and a debug version.
		Returns tuple (strippedTargets, debugTargets)."""
		env.AddPostAction(target, splitDebugAction)
		debugTargets = []
		for t in target:
			t2 = str(t) + ".debug"
			debugTargets.append(env.File(t2))
			env.Clean(t, t2)
		return (target, debugTargets)
	
	def SplitDebugIf(env, target):
		"""Splits target into a stripped and a debug version if debug split is enabled.
		Returns tuple (strippedTargets, debugTargets) if debug split is enabled,
		otherwise (target, []). If OS is not Unix based stripping is disabled."""
		if not env['with_debug_split']:
			return (target, [])
		if not env['OSPosix']:
			return (target, [])
		return SplitDebug(env, target)
	
	env.AddMethod(SplitDebug, 'SplitDebug')
	env.AddMethod(SplitDebugIf, 'SplitDebugIf')

def exists(env):
	return env.WhereIs('objcopy') and env.WhereIs('strip')
