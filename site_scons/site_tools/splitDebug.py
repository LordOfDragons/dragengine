def generate(env):
	def SplitDebug(env, target):
		"""Splits target into a stripped and a debug version.
		Returns tuple (strippedTargets, debugTargets)."""
		env.AddPostAction(target, [
			"$OBJCOPY --only-keep-debug $TARGET ${TARGET}.debug",
			"$STRIP --strip-debug --strip-unneeded $TARGET",
			"$OBJCOPY --add-gnu-debuglink=${TARGET}.debug $TARGET"])
		debugTargets = []
		for t in target:
			t2 = str(t) + '.debug'
			debugTargets.append(env.File(t2))
			env.Clean(t, t2)
			env.SideEffect(t2, t)
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
	
	env.SetDefault(OBJCOPY='objcopy')
	env.SetDefault(STRIP='strip')
	
	env.AddMethod(SplitDebug, 'SplitDebug')
	env.AddMethod(SplitDebugIf, 'SplitDebugIf')

def exists(env):
	return env.WhereIs('objcopy') and env.WhereIs('strip')
