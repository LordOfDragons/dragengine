# -*- coding: utf-8 -*-
#
# Drag[en]gine Blender Scripts
#
# Copyright (C) 2011, Plüss Roland ( roland@rptd.ch )
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
# NOTE: For the GPL copy see http://www.gnu.org/licenses/gpl.html
#

import bpy



# Progress display. To use create instance and call update. The instance
# will update the UI whenever it is useful.
class ProgressDisplay:
	## Create progress bar.
	def __init__(self, maximum, operator=None):
		self.maximum = max(maximum, 1)
		self.progress = 0
		self.text = ''
		self.operator = operator
	
	## Show progress bar
	def show(self):
		bpy.context.window_manager.progress_begin(0.0, 100.0)
	
	## Update progress value
	def update(self, progress, text=''):
		self.progress = min(max(progress, 0), self.maximum)
		self.text = text
		
		bpy.context.window_manager.progress_update(float(self.progress) / float(self.maximum))
		
		infotext = "{:.0f}%: {}".format(self.progress * 100 / self.maximum, self.text)
		if self.operator:
			self.operator.report({'INFO'}, infotext)
		#print(infotext)
	
	## Advance progress by 1 and update progress
	def advance(self, text=''):
		self.update(self.progress + 1, text)
	
	## Hide progress bar
	def hide(self):
		bpy.context.window_manager.progress_end()
