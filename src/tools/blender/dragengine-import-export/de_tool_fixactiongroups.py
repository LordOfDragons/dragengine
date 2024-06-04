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

import bpy, re

from .de_porting import registerClass, appendToMenu



# Tool Fix Action Groups
##########################

class OBJECT_OT_DEToolFixActionGroups(bpy.types.Operator):
	bl_idname = "dragengine.fixactiongroups"
	bl_label = "Auto-group actions"
	bl_icon = 'ACTION'
	bl_options = { 'REGISTER', 'UNDO' }
	__doc__ = """Automatically group actions by bones"""
	
	@classmethod
	def poll( cls, context ):
		return context.active_object != None and context.active_object.type == 'ARMATURE' \
			and bpy.context.mode == 'POSE' and context.active_object.animation_data \
			and context.active_object.animation_data.action
	
	def execute( self, context ):
		pose = context.active_object.pose
		action = context.active_object.animation_data.action
		
		reExtractBoneName = re.compile("^pose\.bones\[\"([^\"]+)\"\]\..+")
		
		for curve in action.fcurves:
			if curve.group:
				continue
			
			# determine the group name. data path is formated like this for bones:
			#   pose.bones["bone"].location
			# 
			# extract the bone part if possible
			match = reExtractBoneName.match(curve.data_path)
			if not match:
				continue
			
			# ensure action group exists
			boneName = match.group(1)
			if not boneName in action.groups:
				action.groups.new(boneName)
			
			# assign curve to action group
			curve.group = action.groups[boneName]
		
		return { 'FINISHED' }

registerClass(OBJECT_OT_DEToolFixActionGroups)
appendToMenu(bpy.types.DOPESHEET_MT_channel, OBJECT_OT_DEToolFixActionGroups)
