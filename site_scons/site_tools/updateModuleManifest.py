# -*- coding: utf-8 -*-
#
# Drag[en]gine Game Engine
#
# Copyright (C) 2025, Plüss Roland (roland@dragondreams)
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
from SCons.Scanner import Scanner

import os, hashlib

def generate(env):
	def buildUpdateModuleManifest(env, target, source):
		manifest = source[0].get_text_contents()
		
		for update in env['ManifestUpdates']:
			action = update['action']
			
			if action == 'filename':
				manifest = manifest.replace(update['keyword'], update['file'].name)
				
			elif action == 'filesize':
				manifest = manifest.replace(update['keyword'],
					str(os.path.getsize(update['file'].abspath)))
				
			elif action == 'filehash':
				with open(update['file'].abspath, 'rb') as f:
					hasher = hashlib.sha1()
					bytes = f.read(1024)
					while bytes:
						hasher.update(bytes)
						bytes = f.read(1024)
				manifest = manifest.replace(update['keyword'], hasher.hexdigest())
				
			elif action == 'preloadLibrary':
				manifest = manifest.replace('</library>',
					'\t<preloadLibrary>{}</preloadLibrary>\n\t</library>'.format(
						update['library'].name))
				
			elif action == 'text':
				manifest = manifest.replace(update['keyword'], update['value'])
		
		with open(target[0].abspath, 'w') as f:
			f.write(manifest)
		
		return 0
	
	def scanUpdateModuleManifest(node, env, path, arg=None):
		d = []
		for update in env['ManifestUpdates']:
			if 'file' in update:
				library = update['file']
				if library not in d:
					d.append(library)
			elif 'library' in update:
				library = update['library']
				if library not in d:
					d.append(library)
		return d
	
	env.Append(BUILDERS = {'UpdateModuleManifest': Builder(
		action = Action(buildUpdateModuleManifest, '$UPDATEMODULEMANIFESTCOMSTR'),
		suffix = '.xml',
		src_suffix = '.xml',
		target_factory = env.fs.File,
		source_scanner = Scanner(function=scanUpdateModuleManifest))})
	
	env.SetDefault(
		UPDATEMODULEMANIFESTCOM = 'Update Module Manifest "$TARGET"',
		UPDATEMODULEMANIFESTCOMSTR = 'Update Module Manifest "$TARGET"')

def exists(env):
	return True
