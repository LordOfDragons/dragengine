# -*- coding: utf-8 -*-

import os
import sys
import fnmatch
import subprocess
import tempfile
import tarfile
import zipfile
import re

import SCons



# glob files in directory relative to SConscript and adds all files matching
# pattern to result. the added files have directory prefixed.
# 
# search: Directory under SConscript directory to search
# pattern: Pattern to match files against
# result: List to add found files
# recursive: Search in sub-directories
def globFiles(env, search, pattern, result, recursive=True):
	oldcwd = os.getcwd()
	os.chdir(env.Dir('.').srcnode().abspath)
	
	for root, dirs, files in os.walk(search):
		if recursive:
			if '.svn' in dirs:
				dirs.remove('.svn')
			if '.git' in dirs:
				dirs.remove('.git')
		else:
			del dirs[:]
		
		for s in fnmatch.filter(files, pattern):
			result.append(root + os.sep + s)
	
	os.chdir(oldcwd)



# umask safe untar. the tarfile module in python is bugged not respecting umask.
# under certain conditions this can lead to problems. to avoid these prolems
# this version of untar first unpacks the archive then walks over all unpacked
# files fixing the file permissions. this is done by masking with 0077 which is
# guaranteed to never yield troubles even under specific situations
def untarArchive(target, source, umask=0o077):
	mask = ~umask
	tf = tarfile.open(source, 'r')
	tf.extractall(target)
	for m in tf.getmembers():
		os.chmod(os.path.join(target, m.name), m.mode & mask)
	tf.close()



# append library linking properties.
# env: Environment to append flags to
# library: Library to get parameters from. Has to be a parent_env type target
#          containing optionally 'libs', 'cppflags', cpppath', 'libpath'.
# libs: List to appends link libraries to
def appendLibrary(env, library, libs, withLibs=True, internalModule=False):
	if not internalModule:
		if withLibs and 'libs' in library:
			libs.extend(library['libs'])
		if 'libpath' in library:
			env.Append(LIBPATH = library['libpath'])
	if 'cpppath' in library:
		env.Append(CPPPATH = library['cpppath'])
	if 'cppflags' in library:
		env.Append(CPPFLAGS = library['cppflags'])
	if 'linkflags' in library:
		env.Append(LINKFLAGS = library['linkflags'])

# this is a ugly hack but there is no other way to get it working. we have to force
# the compiler to include the entire content of the static library. if not done like
# this the compiler optimizes out some symbols leading to link errors with users of
# the shared library. we can though not use the whole-archive for everything
# otheriwse some other libraries are included in a bad way.
def appendLibraryWholeStatic(env, library, libs, internalModule=False):
	if not internalModule and 'libs' in library:
		linkWholeArchive = library['linkWholeArchive'] if 'linkWholeArchive' in library else []
		for l in library['libs']:
			if l in linkWholeArchive:
				env.Append(LINKFLAGS = ['-Wl,--whole-archive', l, '-Wl,--no-whole-archive'])
			else:
				libs.append(l)
	if 'cpppath' in library:
		env.Append(CPPPATH = library['cpppath'])
	if 'libpath' in library:
		env.Append(LIBPATH = library['libpath'])
	if 'cppflags' in library:
		env.Append(CPPFLAGS = library['cppflags'])



# format path string to be used as define declaration on the command line.
# returns '-D{NAME}={PATH}' with path properly escaped for the operating
# system used in environment.
def cmdlineDefinePath(env, name, path):
	if env['OSWindows']:
		# replace unix path separators with windows path separators
		path = path.replace('/', '\\')
		# escape backslash. double escape is required since the python string
		# consumes one escape and the bash command line consumes the second
		path = path.replace('\\', '\\\\\\\\')
	
	return '-D{}=\\"{}\\"'.format(name, path)



# ternary variable
_TernaryVariableStringsYes = ('y', 'yes', 'true', 't', '1', 'on', 'all')
_TernaryVariableStringsNo = ('n', 'no', 'false', 'f', '0', 'off', 'none')
_TernaryVariableStringsAuto = ('auto')

TernaryVariableYes = 'yes'
TernaryVariableNo = 'no'
TernaryVariableAuto = 'auto'

def _TernaryVariableMapper(text):
	textLower = str(text).lower()
	if textLower in _TernaryVariableStringsYes:
		return TernaryVariableYes
	if textLower in _TernaryVariableStringsNo:
		return TernaryVariableNo
	if textLower in _TernaryVariableStringsAuto:
		return TernaryVariableAuto
	raise ValueError('Invalid value for ternary option: %s' % text)

def _TernaryVariableValidator(key, value, env):
	if not env[key] in (TernaryVariableYes, TernaryVariableNo, TernaryVariableAuto):
		raise SCons.Errors.UserError('Invalid value for ternary option %s: %s' % (key, env[key]))

def TernaryVariable(key, help, default=TernaryVariableAuto):
	return (key, '%s (yes|no|auto)' % help, default, _TernaryVariableValidator, _TernaryVariableMapper)



def StringVariable(key, help, default=''):
	return (key, '%s (string)' % help, default)



# library symbolic link action
def SymLinkLibrary(env, target, source):
	os.symlink(source[0].name, target[0].abspath)
	return 0 # how does os.symlink signal an error?



# write configuration file action
def WriteConfigFile(target, defines, env):
	file = open(target, 'w')
	
	file.write('/********************************************+\n')
	file.write('|| Configuration File. Do not edit since     |\n')
	file.write('|| this is a generated file and all changes  |\n')
	file.write('|| will be lost the next time scons runs.    |\n')
	file.write('`********************************************/\n')
	file.write('\n')
	
	for define in defines:
		if isinstance(defines[define], str):
			file.write('/* {} */\n'.format(define))
			file.write('#ifndef {}\n'.format(define))
			file.write('#define {} "{}"\n'.format(define, defines[define]))
		elif defines[define]:
			file.write('/* enable {} */\n'.format(define))
			file.write('#ifndef {}\n'.format(define))
			file.write('#define {} 1\n'.format(define))
		else:
			file.write('/* disable {} */\n'.format(define))
			file.write('#ifdef {}\n'.format(define))
			file.write('#undef {}\n'.format(define))
		file.write('#endif\n')
		file.write('\n')
	
	file.write('/* End of configuration file. */\n')
	file.close()
	
	return 0



# windows sucks hack (http://www.scons.org/wiki/LongCmdLinesOnWin32)
# modification by rptd. python has a problem that subprocess expects
# an environment with only string objects. unfortunately scons tends
# to add unicode objects so this fails. to handle this the directory
# is first converted to pure string objects.
def WindowsSpawnHack(envW32):
	class ourSpawn:
		def ourspawn(self, sh, escape, cmd, args, env):
			newargs = ' '.join(args[1:])
			cmdline = cmd + " " + newargs
			startupinfo = subprocess.STARTUPINFO()
			#startupinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
			"""
			print('typeof cmdline = ', type(cmdline))
			print('typeof env')
			for key, value in env.items():
				print('typeof key/value, key=', type(key), 'value=', type(value), key, value)
			test = {}
			for key, value in env.items():
				test[str(key)] = str(value)
			for key, value in test.items():
				print('typeof key/value, key=', type(key), 'value=', type(value), key, value)
			"""
			hackenv = {}
			for key, value in env.items():
				hackenv[str(key)] = str(value)
			proc = subprocess.Popen(cmdline, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
				stderr=subprocess.PIPE, startupinfo=startupinfo, shell = False, env = hackenv)
			data, err = proc.communicate()
			rv = proc.wait()
			if rv:
				print("=====")
				print(err)
				print("=====")
			return rv
	
	buf = ourSpawn()
	buf.ourenv = envW32
	envW32['SPAWN'] = buf.ourspawn

class AdvTempFileMunge(object):
	"""
	env['TEMPFILE'] = AdvTempFileMunge
	env['LINKCOM'] = "${TEMPFILE('$LINK $TARGET $SOURCES',0)}"
	env['SHLINKCOM'] = "${TEMPFILE('$LINK $TARGET $SOURCES',1)}"
	"""
	def __init__(self, cmd, targetType):
		self.cmd = cmd
		self.targetType = targetType
	
	def shlib_generator(self, target, source, env, for_signature):
		cmd = SCons.Util.CLVar(['$SHLINK', '$SHLINKFLAGS']) 
		
		dll = env.FindIxes(target, 'SHLIBPREFIX', 'SHLIBSUFFIX')
		if dll: cmd.extend(['-o', str(dll)])
		
		cmd.extend(['$SOURCES', '$_LIBDIRFLAGS', '$_LIBFLAGS'])
		
		implib = env.FindIxes(target, 'LIBPREFIX', 'LIBSUFFIX')
		if implib: cmd.append('-Wl,--out-implib,'+implib.get_string(for_signature))
		
		def_target = env.FindIxes(target, 'WINDOWSDEFPREFIX', 'WINDOWSDEFSUFFIX')
		insert_def = env.subst("$WINDOWS_INSERT_DEF")
		if not insert_def in ['', '0', 0] and def_target: \
			cmd.append('-Wl,--output-def,'+def_target.get_string(for_signature))
		
		return [cmd]
	
	def __call__(self, target, source, env, for_signature):
		if for_signature:
			# If we're being called for signature calculation, it's
			# because we're being called by the string expansion in
			# Subst.py, which has the logic to strip any $($) that
			# may be in the command line we squirreled away.  So we
			# just return the raw command line and let the upper
			# string substitution layers do their thing.
			if self.targetType == 0:
				return self.cmd
			else:
				return self.shlib_generator(target, source, env, for_signature)
		
		###print 'TempFileMunghe', 'target', target, 'source', source, 'for_signature', for_signature
		# Now we're actually being called because someone is actually
		# going to try to execute the command, so we have to do our
		# own expansion.
		if self.targetType == 0:
			cmd = env.subst_list(self.cmd, SCons.Subst.SUBST_CMD, target, source)[0]
		else:
			cmd = self.shlib_generator(target, source, env, for_signature)[0]
			cmd = env.subst_list(cmd, SCons.Subst.SUBST_CMD, target, source)[0]
		
		try:
			maxline = int(env.subst('$MAXLINELENGTH'))
		except ValueError:
			maxline = 2048
		
		length = 0
		for c in cmd:
			length += len(c)
		if length <= maxline:
			if self.targetType == 0:
				return self.cmd
			else:
				return self.shlib_generator(target, source, env, for_signature)

		# We do a normpath because mktemp() has what appears to be
		# a bug in Windows that will use a forward slash as a path
		# delimiter.  Windows's link mistakes that for a command line
		# switch and barfs.
		#
		# We use the .lnk suffix for the benefit of the Phar Lap
		# linkloc linker, which likes to append an .lnk suffix if
		# none is given.
		(fd, tmp) = tempfile.mkstemp('.lnk', text=True)
		native_tmp = SCons.Util.get_native_path(os.path.normpath(tmp))
		
		if env['SHELL'] and env['SHELL'] == 'sh':
			# The sh shell will try to escape the backslashes in the
			# path, so unescape them.
			native_tmp = native_tmp.replace('\\', r'\\\\')
			# In Cygwin, we want to use rm to delete the temporary
			# file, because del does not exist in the sh shell.
			rm = env.Detect('rm') or 'del'
		else:
			# Don't use 'rm' if the shell is not sh, because rm won't
			# work with the Windows shells (cmd.exe or command.com) or
			# Windows path names.
			rm = 'del'
		
		prefix = env.subst('$TEMPFILEPREFIX')
		if not prefix:
			prefix = '@'
		
		args = list(map(SCons.Subst.quote_spaces, cmd[1:]))
		args = list(map(lambda x: x.replace('\\', '\\\\'), args))
		os.write(fd, " ".join(args) + "\n")
		os.close(fd)
		# XXX Using the SCons.Action.print_actions value directly
		# like this is bogus, but expedient.  This class should
		# really be rewritten as an Action that defines the
		# __call__() and strfunction() methods and lets the
		# normal action-execution logic handle whether or not to
		# print/execute the action.  The problem, though, is all
		# of that is decided before we execute this method as
		# part of expanding the $TEMPFILE construction variable.
		# Consequently, refactoring this will have to wait until
		# we get more flexible with allowing Actions to exist
		# independently and get strung together arbitrarily like
		# Ant tasks.  In the meantime, it's going to be more
		# user-friendly to not let obsession with architectural
		# purity get in the way of just being helpful, so we'll
		# reach into SCons.Action directly.
		"""if SCons.Action.print_actions:
			print('Using tempfile %s for command line' % str(cmd[0]))
			#print("Using tempfile "+native_tmp+" for command line:\n"+
			#      str(cmd[0]) + " " + " ".join(args))"""
		return [cmd[0], prefix + native_tmp + '\n' + rm, native_tmp]


# init the common stuff. this determines the os type and stores
# it in the environment as variables. also sets up windows
# specific hacks if required
def InitCommon(env):
	# OS detection
	OSPosix = (env['OS_NAME'] == 'posix')
	OSWindows = (env['OS_NAME'] == 'win32') or (env['SYS_PLATFORM'] == 'win32')
	OSBeOS = (env['SYS_PLATFORM'] == 'haiku1')
	OSMacOS = (env['SYS_PLATFORM'] == 'darwin')
	
	env['OSPosix'] = OSPosix
	env['OSWindows'] = OSWindows
	env['OSBeOS'] = OSBeOS
	env['OSMacOS'] = OSMacOS
	
	env['HostOSWindows'] = os.name == 'win32' or sys.platform == 'win32'
	
	# under windows we need a temp file hack for long command lines.
	# the existing hack in SCons doesn't work though for shared
	# libraries. to handle this an advanced TempFileMunge class is
	# defined which can be used for both situations.
	#env['TEMPFILE'] = AdvTempFileMunge
	#env['LINKCOM'] = "${TEMPFILE('%s',0)}" % env['LINKCOM']
	#env['SHLINKCOM'] = "${TEMPFILE('$LINK $TARGET $SOURCES',1)}"
	# this hack fails for mingw and g++ as it uses an internal
	# call to cc1plus causing the 32k limit to strike again.
	# only solution is to group *.o files in the extreme cases
	# for windows systems. anything else just drives one nuts.
	if env['HostOSWindows']:
		WindowsSpawnHack(env)
		env['WINDOWS_INSERT_DEF'] = True;

# disable verbose compile messages
def DisableVerboseCompiling(env):
	env['CCCOMSTR'] = 'Compiling $SOURCES'
	env['CXXCOMSTR'] = 'Compiling $SOURCES'
	env['LINKCOMSTR'] = 'Linking $TARGET'
	env['SHCCCOMSTR'] = 'Compiling $SOURCES'
	env['SHCXXCOMSTR'] = 'Compiling $SOURCES'
	env['SHLINKCOMSTR'] = 'Linking $TARGET'
	env['LDMODULECOMSTR'] = 'Linking $TARGET'
	env['RANLIBCOMSTR'] = 'Indexing $TARGET'
	env['ARCOMSTR'] = 'Linking $TARGET'
	env['TARCOMSTR'] = 'Archiving $TARGET'
	env['ZIPCOMSTR'] = 'Zipping $Target'

# build help text using target list
def BuildHelpText(targets):
	maxLenTarget = 1
	maxLenDescription = 1
	
	for key in sorted(targets.keys()):
		lenTarget = len(key)
		lenDescription = len(targets[key]['name'])
		if lenTarget > maxLenTarget:
			maxLenTarget = lenTarget
		if lenDescription > maxLenDescription:
			maxLenDescription = lenDescription
	
	helpText = []
	helpText.append("scons <target> { <target> ... }:\n")
	helpText.append("    Build the target(s)\n")
	helpText.append("scons <target>_install { <target>_install ... }:\n")
	helpText.append("    Build and install target(s)\n")
	helpText.append("scons <target> { <target> ... } -c:\n")
	helpText.append("    Clear target(s) build\n")
	helpText.append("\n")
	
	helpText.append(('%s | Description\n' % 'Target'.ljust(maxLenTarget)))
	helpText.append(('%s-+-%s\n' % (''.ljust(maxLenTarget, '-'), ''.ljust(maxLenDescription, '-'))))
	for key in sorted(targets.keys()):
		helpText.append('%s | %s\n' % (key.ljust(maxLenTarget), targets[key]['name']))
	helpText.append('\n')
	
	return ''.join(helpText)

# prints out a configuration report
def PrintConfigReport(report):
	maxLenKey = 1
	maxLenValue = 1
	
	for key in sorted(report.keys()):
		lenKey = len(key)
		lenValue = len(report[key])
		if lenKey > maxLenKey:
			maxLenKey = lenKey
		if lenValue > maxLenValue:
			maxLenValue = lenValue
	
	print('')
	print('Configuration Report:')
	print('{}-+-{}'.format(''.ljust(maxLenKey, '-'), ''.ljust(maxLenValue, '-')))
	for key in sorted(report.keys()):
		print('{} | {}'.format(key.ljust(maxLenKey), report[key]))
	print('')

# CLOC builder
def BuildCLOC(env, srcDirs, targetCSV = 'clocreport.csv', excludeDirs = []):
	cmdline = 'cloc --follow-links --quiet --csv --sum-one'
	if len(excludeDirs) > 0:
		cmdline = cmdline + ' --exclude-dir={}'.format(','.join(excludeDirs))
	cmdline = cmdline + ' --out={} {}'.format(targetCSV, ' '.join(srcDirs))
	return env.Command(targetCSV, '', cmdline)

# CLOC summary builder
def BuildCLOCSummary(env, reports, targetCSV = 'clocsummary.csv'):
	def build(target, source, env):
		summary = {}
		for report in source:
			print('processing report {}...'.format(str(report)))
			headerLine = True
			f = open(str(report), 'r')
			for line in f:
				if not headerLine:
					fields = line.split(',')
					if not fields[1] in summary:
						summary[fields[1]] = [fields[1], 0, 0, 0, 0]
					summary[fields[1]][1] = summary[fields[1]][1] + int(fields[0])
					summary[fields[1]][2] = summary[fields[1]][2] + int(fields[2])
					summary[fields[1]][3] = summary[fields[1]][3] + int(fields[3])
					summary[fields[1]][4] = summary[fields[1]][4] + int(fields[4])
				headerLine = False
			f.close()
		
		f = open(str(target[0]), 'w')
		f.write('language,files,blank,comment,code\n')
		for key in summary:
			f.write(','.join([str(x) for x in summary[key]]))
			f.write('\n')
		f.close()
		print(summary)
		return None
	
	return env.Command(targetCSV, reports, build)

# Archiving
reWindowsDrive = re.compile('^([A-Z]:[/\\\\])|([A-Z][A-Z0-9]*//)', re.I)

def NormalizePath(path):
	path = os.path.normpath(path)
	
	path = os.path.splitdrive(path)[1] # return (drive, tail)
	
	# for cross compiling splitdrive wont work since python is compiled for
	# the host system and os.path methods work with host system parameters.
	# use a simple regular expression check to see if we need to cut the
	# start of the string
	match = reWindowsDrive.match(path)
	if match:
		path = path[match.end():]
	elif path[0] == '/':
		path = path[1:]  # no root path in archives
	
	return path

def ArchiveTarBz2(env, target, source):
	with tarfile.open(target[0].abspath, 'w:bz2', dereference=True) as arcfile:
		for path, node in env['ArchiveFiles'].items():
			# NOTE gettarinfo uses os.stat instead of os.lstat and thus fails to detect links
			if os.path.islink(node.abspath):
				info = tarfile.TarInfo(NormalizePath(path))
				info.type = tarfile.SYMTYPE
				info.linkname = os.readlink(node.abspath)
				info.uid = 0
				info.gid = 0
				info.uname = 'root'
				info.gname = 'root'
				arcfile.addfile(info)
			else:
				info = arcfile.gettarinfo(node.abspath, NormalizePath(path))
				info.uid = 0
				info.gid = 0
				info.uname = 'root'
				info.gname = 'root'
				with open(node.abspath, 'rb') as nf:
					arcfile.addfile(info, nf)

def ArchiveZip(env, target, source):
	with zipfile.ZipFile(target[0].abspath, 'w',
			compression=zipfile.ZIP_DEFLATED,
			compresslevel=9) as arcfile:
		for path, node in env['ArchiveFiles'].items():
			arcfile.write(node.abspath, NormalizePath(path))
