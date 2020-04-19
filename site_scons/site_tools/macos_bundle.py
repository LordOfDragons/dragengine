# -*- coding: utf-8 -*-
#
# Drag[en]gine Game Engine
#
# Copyright (C) 2017, Plüss Roland ( roland@rptd.ch )
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

from SCons.Builder import Builder
from SCons.Action import Action

import os
import shutil

def generate(env):
	def emitBundleMacOS(target, source, env):
		dirApp = target[0]
		dirContents = dirApp.Dir('Contents')
		fileInfoPList = dirContents.File('Info.plist')
		dirBinary = dirContents.Dir('MacOS')
		fileBinary = dirBinary.File(source[0].name)
		dirResources = dirContents.Dir('Resources')
		# todo include libraries and resources from env['BUNDLE_LIBS'] and env['BUNDLE_RESOURCES']
		
		newTarget = []
		#newTarget.append(dirApp)
		#newTarget.append(dirContents)
		newTarget.append(fileInfoPList)
		#newTarget.append(dirBinary)
		newTarget.append(fileBinary)
		#newTarget.append(dirResources)
		
		newSource = []
		newSource.append(source[0])
		
		return newTarget, newSource
	
	def buildBundleMacOS(env, target, source):
		with open(target[0].get_path(), 'w') as f:
			f.write('\n'.join(createInfoPList(env, source[0])))
		
		shutil.copy(source[0].get_path(), target[1].get_path())
	
	def createInfoPList(env, source):
		lines = []
		lines.append('<?xml version="1.0" encoding="UTF-8"?>')
		lines.append('<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">')
		lines.append('<plist version="1.0">')
		lines.append('<dict>')
		
		lines.append('<key>CFBundleIdentifier</key>')
		lines.append('<string>{0}</string>'.format(env.subst('$BUNDLE_ID')))
		
		lines.append('<key>CFBundleDisplayName</key>')
		lines.append('<string>{0}</string>'.format(env.subst('$BUNDLE_NAME')))
		
		lines.append('<key>CFBundleName</key>')
		lines.append('<string>{0}</string>'.format(env.subst('$BUNDLE_NAME')))
		
		lines.append('<key>CFBundleVersion</key>')
		lines.append('<string>{0}</string>'.format(env.subst('$BUNDLE_VERSION')))
		
		lines.append('<key>CFBundleInfoDictionaryVersion</key>')
		lines.append('<string>6.0</string>')
		
		lines.append('<key>CFBundlePackageType</key>')
		lines.append('<string>APPL</string>')
		
		lines.append('<key>CFBundleExecutable</key>')
		lines.append('<string>{0}</string>'.format(source.name))
		
		lines.append('<key>CFBundleSignature</key>')
		lines.append('<string>{0}</string>'.format(env.subst('$BUNDLE_SIGNATURE')))
		
		lines.append('</dict>')
		lines.append('</plist>')
		return lines
	
	env.Append(BUILDERS = {'BundleMacOS': Builder(
		action = Action(buildBundleMacOS, '$BUNDLEMACOSCOMSTR'),
		emitter = emitBundleMacOS,
		suffix = '.App',
		src_suffix = '',
		target_factory = env.fs.Dir)})
	env.SetDefault(
		BUNDLEMACOSCOM = 'Bundle MacOS "$TARGET"',
		BUNDLEMACOSCOMSTR = 'Bundle MacOS "$TARGET"',
		BUNDLE_SIGNATURE = 'RPTD')

def exists(env):
	return True
