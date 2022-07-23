# -*- coding: utf-8 -*-
#
# Drag[en]gine Blender Scripts
#
# Copyright (C) 2011, Plüss Roland (roland@rptd.ch)
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
import math

from .de_math import ONE_PI, vecSub, vecLength, projectToPlane
from .de_porting import registerClass



# Tool Transfer UV
####################

class OBJECT_OT_ToolShapePropertyFromTexture(bpy.types.Operator):
	bl_idname = "dragengine.shapropfromtex"
	bl_label = "Shape property from texture"
	bl_options = {'REGISTER', 'UNDO'}
	__doc__ = """Set shape property string from nearest face material texture name"""
	
	
	
	@classmethod
	def poll(cls, context):
		return context.active_object != None \
			and context.active_object.type == 'MESH'
	
	def execute(self, context):
		# find reference object to map against
		refMesh = [x for x in context.selected_objects
			 if x.rigid_body or x.dragengine_physics == 'NONE']
		if len(refMesh) != 1:
			self.report({'INFO', 'ERROR'}, "Select exactly one non-physics object to map physics objects against.")
			return False
		
		refMesh = refMesh[0]
		rminvmat = refMesh.matrix_world.inverted()
		rmfaces = refMesh.data.polygons
		rmverts = refMesh.data.vertices
		rmloops = refMesh.data.loops
		
		# process physics objects
		for obj in context.selected_objects:
			# operate only on physics objects. this skips the reference object
			if obj.rigid_body or obj.dragengine_physics == 'NONE':
				continue
			
			# calculate position relative to reference mesh object
			searchPos = (rminvmat * obj.matrix_world).to_translation()
			
			# find closes face in parent object
			closestFace = None
			closestDistance = 0.0
			
			for face in rmfaces:
				faceNormal = face.normal
				
				# project point to face
				facePos = projectToPlane(searchPos, faceNormal, face.center)
				
				cornerPosLast = rmverts[rmloops[face.loop_start + face.loop_total - 1].vertex_index].co
				for corner in range(face.loop_total):
					cornerPosNext = rmverts[rmloops[face.loop_start + corner].vertex_index].co
					testNormal = faceNormal.cross(cornerPosNext - cornerPosLast).normalized()
					distance = (facePos - cornerPosLast).dot(testNormal)
					if distance < 0.0:
						facePos = facePos - testNormal * distance
					cornerPosLast = cornerPosNext
				
				# face wins if the distance to the closest point is closer than anything found so far.
				# for speed reason we use only distance squared since this works too
				distance = (facePos - searchPos).length_squared
				if not closestFace or distance < closestDistance:
					closestFace = face
					closestDistance = distance
			
			# get texture name of face
			material = refMesh.data.materials[closestFace.material_index]
			name = material.dragengine_exportname
			if not name:
				name = material.name
			obj.dragengine_shapeproperty = name
		
		return {'FINISHED'}
registerClass(OBJECT_OT_ToolShapePropertyFromTexture)
