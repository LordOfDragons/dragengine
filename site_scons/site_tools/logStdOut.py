# -*- coding: utf-8 -*-
#
# Drag[en]gine Game Engine
#
# Copyright (C) 2018, Plüss Roland ( roland@rptd.ch )
# 
# This program is free software; you can redistribute it and/or 
# modify it under the terms of the GNU General Public License 
# as published by the Free Software Foundation; either 
# version 2 of the License, or (at your option) any later 
# version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
import sys, re

# Log output of std-out to a log file.
# 
# The tool is automatically loaded and redirects std-out to a file-like object. This object
# prints any calls to write() out to the std-out used to run scons. Without any further
# modifications this is identifical to not using this redirect at all. To enable logging to
# a file set the environment parameter 'LOG_STD_OUT_FILE' to a file-like object. When write()
# is called and LOG_STD_OUT_FILE is not None the output is also written to the file-like
# object. A typical use is like this:
# 
# env.Tool('logStdOut')
# env['LOG_STD_OUT_FILE'] = open('build.log', 'w')
# 
# To stop logging remove the parameter or set it to None.
# 
# By default text send to the file-like object is processed to remove escape sequences. If
# you need the escape sequences to be present set LOG_STD_OUT_KEEP_ESCSEQ = True .
# 
class _LogStdOutStream():
	# Create logger
	def __init__(self, env):
		if not env:
			raise Exception('env is invalid')
		self.env = env
		self.stdout = sys.stdout
		self.reStripEscSeq = re.compile(r'\x1B\[[0-?]*[ -/]*[@-~]')
		if sys.platform == 'win32':
			sys.stdout.reconfigure(encoding='utf-8')
	
	# Write message
	def write(self, message):
		self.stdout.write(message)
		stream = self.env.get('LOG_STD_OUT_FILE', None)
		if stream:
			if self.env.get('LOG_STD_OUT_KEEP_ESCSEQ', False):
				stream.write(message)
			else:
				stream.write(self.reStripEscSeq.sub('', message))
	
	# Flush is required for python 3 compatibility. SCons runs on python 2 but you never know.
	def flush(self):
		stream = self.env.get('LOG_STD_OUT_FILE', None)
		if stream:
			stream.flush()
	
	# Check if stream is a console
	def isatty(self):
		return self.stdout.isatty()

# SCons Tools required this method to be present. It is called if exists() returns true
def generate(env):
	sys.stdout = _LogStdOutStream(env)

# SCons Tools required this method to be present. Checks if tool is applicable
def exists(env):
	return True
