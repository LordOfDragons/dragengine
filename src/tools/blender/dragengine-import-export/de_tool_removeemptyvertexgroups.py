# -*- coding: utf-8 -*-
#
# Drag[en]gine Blender Scripts
#
# Copyright (C) 2021, Plüss Roland ( roland@rptd.ch )
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

from .de_porting import registerClass



# Tool Remove Empty Vertex Groups
###################################

class OBJECT_OT_ToolRemoveEmptyVertexGroups(bpy.types.Operator):
	bl_idname = "dragengine.remove_empty_vertex_groups"
	bl_label = "Remove Empty Vertex Groups"
	bl_options = {'INTERNAL'}
	__doc__ = """Remove vertex groups not used by any vertices"""
	
	@classmethod
	def poll(cls, context):
		return context.active_object != None \
			and context.active_object.type == 'MESH' \
			and context.active_object.mode in ['OBJECT', 'EDIT']
	
	def execute(self, context):
		restoreEditMode = self.inModeEdit(context)
		self.modeObject()
		try:
			self.removeEmptyVertexGroups(context)
		finally:
			if restoreEditMode:
				self.modelEdit()
		return {'FINISHED'}
	
	# selection states do not update in edit mode. flick-flack to get it working
	def modelFlickFlack(self):
		self.modeObject()
		self.modelEdit()
	
	def inModeEdit(self, context):
		return context.active_object.mode == 'EDIT'
	
	def modeObject(self):
		bpy.ops.object.mode_set(mode='OBJECT')
	
	def modelEdit(self):
		bpy.ops.object.mode_set(mode='EDIT')
	
	def removeEmptyVertexGroups(self, context):
		object = context.active_object
		mesh = object.data
		
		# find empty groups
		groups = [[g, False] for g in object.vertex_groups]
		
		for v in mesh.vertices:
			for g in v.groups:
				if g.weight: # ignore 0 weights
					groups[g.group][1] = True
		
		print(groups)
		# remove groups
		for g in groups:
			if not g[1]:
				object.vertex_groups.remove(g[0])

if bpy.app.version >= (2, 80):
	registerClass(OBJECT_OT_ToolRemoveEmptyVertexGroups)
