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

from .de_porting import registerClass, appendToMenu



# Tool Mirror Vertices
########################

class OBJECT_OT_ToolMirrorVertices(bpy.types.Operator):
	bl_idname = "dragengine.mirror_vertices"
	bl_label = "Mirror Vertices"
	bl_options = {'REGISTER', 'UNDO'}
	__doc__ = """Mirror vertices (location and weights)"""
	
	@classmethod
	def poll(cls, context):
		return context.active_object != None \
			and context.active_object.type == 'MESH' \
			and context.active_object.mode == 'EDIT'
	
	def execute(self, context):
		self.modelFlickFlack()
		if any(v.select for v in context.active_object.data.vertices):
			params = self.storeParameters(context)
			try:
				self.mirrorVertices(context)
				self.mirrorWeights(context)
			finally:
				self.modelEdit()
				self.restoreParameters(context, params)
		return {'FINISHED'}
	
	# selection states do not update in edit mode. flick-flack to get it working
	def modelFlickFlack(self):
		self.modeObject()
		self.modelEdit()
	
	def modeObject(self):
		bpy.ops.object.mode_set(mode='OBJECT')
	
	def modelEdit(self):
		bpy.ops.object.mode_set(mode='EDIT')
	
	def mirrorVertices(self, context):
		object = context.active_object
		scene = context.scene
		mesh = object.data
		
		# set cursor to correct location for mirroring
		scene.cursor.location = object.location
		
		# switch off auto-mirroring to be on the safe side
		mesh.use_mirror_x = False
		mesh.use_mirror_y = False
		mesh.use_mirror_z = False
		
		# set pivot to cursor
		context.tool_settings.transform_pivot_point = 'CURSOR'
		
		# duplicate selected vertices
		bpy.ops.mesh.duplicate_move(MESH_OT_duplicate={"mode":1})
		
		# mirror vertices
		bpy.ops.transform.mirror(orient_type='LOCAL', orient_matrix_type='LOCAL',
			constraint_axis=(True, False, False))
	
	def mirrorWeights(self, context):
		object = context.active_object
		scene = context.scene
		mesh = object.data
		
		# find mirrored groups
		replaces = self.getVGroupSubPatterns()
		
		groups = []
		for g in object.vertex_groups:
			name = g.name
			m = None
			for r in replaces:
				s = r[0].subn(r[1], name)
				if s[1]: # substitution count is larger than 0
					if s[0] in object.vertex_groups:
						m = object.vertex_groups[s[0]]
					break
			groups.append((g, m))
		
		# mirror weights of duplicated vertices.
		# VertexGroup.remove()/add() required object mode
		self.modeObject()
		vertices = [v for v in mesh.vertices if v.select]
		
		for v in vertices:
			# store the weights aside since modify them changes the internal list
			weights = [(g.group, g.weight) for g in v.groups]
			
			# mirror weights
			for w in weights:
				g = groups[w[0]]
				if g[1]: # mirror group found
					indices = (v.index,)
					g[0].remove(indices)
					g[1].add(indices, w[1], 'REPLACE')
	
	# store parameters
	def storeParameters(self, context):
		object = context.active_object
		mesh = object.data
		
		return {'cursor': context.scene.cursor.location,
			'mirror_x': mesh.use_mirror_x,
			'mirror_y': mesh.use_mirror_y,
			'mirror_z': mesh.use_mirror_z,
			'pivot_point': context.tool_settings.transform_pivot_point}
	
	# restore parameters
	def restoreParameters(self, context, params):
		object = context.active_object
		mesh = object.data
		
		context.scene.cursor.location = params['cursor']
		mesh.use_mirror_x = params['mirror_x']
		mesh.use_mirror_y = params['mirror_y']
		mesh.use_mirror_z = params['mirror_z']
		context.tool_settings.transform_pivot_point = params['pivot_point']
	
	# get list of vertex groups substitution patterns
	def getVGroupSubPatterns(self):
		replaces = []
		
		replaces.append((re.compile(r"^(.*[. ])l$"), r"\1r"))
		replaces.append((re.compile(r"^(.*[. ])r$"), r"\1l"))
		
		replaces.append((re.compile(r"^(.*[. ])L$"), r"\1R"))
		replaces.append((re.compile(r"^(.*[. ])R$"), r"\1L"))
		
		replaces.append((re.compile(r"^(.*[. ])left([. ].+)?$"), r"\1right\2"))
		replaces.append((re.compile(r"^(.*[. ])right([. ].+)?$"), r"\1left\2"))
		
		replaces.append((re.compile(r"^(.*[. ])Left([. ].+)?$"), r"\1Right\2"))
		replaces.append((re.compile(r"^(.*[. ])Right([. ].+)?$"), r"\1Left\2"))
		
		replaces.append((re.compile(r"^(.*[. ])LEFT([. ].+)?$"), r"\1RIGHT\2"))
		replaces.append((re.compile(r"^(.*[. ])RIGHT([. ].+)?$"), r"\1LEFT\2"))
		
		return replaces

if bpy.app.version >= (2, 80):
	registerClass(OBJECT_OT_ToolMirrorVertices)
	appendToMenu(bpy.types.VIEW3D_MT_edit_mesh_vertices, OBJECT_OT_ToolMirrorVertices)