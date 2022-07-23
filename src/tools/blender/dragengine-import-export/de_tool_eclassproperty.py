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

from mathutils import Vector, Matrix

from .de_math import vector_by_matrix, float2String, convertMatrix
from .de_resources import Mesh, Armature
from .de_porting import registerClass, matmul



# Element class properties
############################

class OBJECT_OT_DEToolEClassProperty(bpy.types.Operator):
	bl_idname = "dragengine.eclassproperty"
	bl_label = "Drag[en]gine EClass Property"
	bl_options = {'REGISTER', 'UNDO'}
	__doc__ = """Element class property value from selection into Text object"""
	
	relative3DCursor: bpy.props.BoolProperty(name="Relative 3D-Cursor", description="Relative to 3D cursor", default=False)
	
	@classmethod
	def poll(cls, context):
		return len(context.selected_objects) > 0
	
	def execute(self, context):
		if "DE Element Class Properties" in bpy.data.texts:
			text = bpy.data.texts["DE Element Class Properties"]
		else:
			text = bpy.data.texts.new("DE Element Class Properties")
		
		text.clear()
		
		# gather some parameters
		scaling = context.scene.dragengine_scaling
		scalePosition = Matrix(((scaling,0,0,0), (0,scaling,0,0), (0,0,scaling,0), (0,0,0,1)))
		
		if self.relative3DCursor:
			if hasattr(context.scene, "cursor"):
				relativeMatrix = Matrix.Translation(-context.scene.cursor.location)
			else:
				relativeMatrix = Matrix.Translation(-context.scene.cursor_location)
		
		# shapes as shape
		shapes = []
		specialShapes = []
		
		for obj in context.selected_objects:
			if obj.type != 'MESH':
				continue
			if obj.rigid_body or obj.dragengine_physics == 'NONE':
				continue
			
			volume = Armature.CollisionVolume(obj, scalePosition)
			if self.relative3DCursor:
				volume.setMatrix(convertMatrix(matmul(relativeMatrix, volume.object.matrix_world)), scalePosition)
			
			pos = volume.position
			rot = volume.rotation
			hext = Vector((volume.scaling.x * 0.5, volume.scaling.y * 0.5, volume.scaling.z * 0.5))
			
			parameters = []
			
			rbshape = ''
			if obj.rigid_body:
				rbshape = obj.rigid_body.collision_shape
			
			if rbshape == 'SPHERE' or obj.dragengine_physics == 'SPHERE':
				parameters.append("sphere")
				if pos.length > 1e-5:
					parameters.append("position,{},{},{}".format(float2String(pos.x),
						float2String(pos.y), float2String(pos.z)))
				parameters.append("radius,{}".format(float2String(scaling.x * 0.5)))
				
			elif rbshape == 'BOX' or obj.dragengine_physics == 'BOX':
				parameters.append("box")
				if pos.length > 1e-5:
					parameters.append("position,{},{},{}".format(float2String(pos.x),
						float2String(pos.y), float2String(pos.z)))
				parameters.append("extends,{},{},{}".format(float2String(hext.x),
					float2String(hext.y), float2String(hext.z)))
				if rot.length > 1e-4:
					parameters.append("rotation,{},{},{}".format(float2String(rot.x),
						float2String(rot.y), float2String(rot.z)))
				
				special = ['box']
				if pos.length > 1e-5:
					special.append("<vector key='position' x='{}' y='{}' z='{}'/>".format(
						float2String(pos.x), float2String(pos.y), float2String(pos.z)))
				special.append("<vector key='extends' x='{}' y='{}' z='{}'/>".format(
					float2String(hext.x), float2String(hext.y), float2String(hext.z)))
				if rot.length > 1e-4:
					special.append("<vector key='rotation' x='{}' y='{}' z='{}'/>".format(
						float2String(rot.x), float2String(rot.y), float2String(rot.z)))
				specialShapes.append(special)
				
			elif rbshape == 'CYLINDER' or obj.dragengine_physics == 'CYLINDER':
				pass # todo
				
			elif rbshape == 'CAPSULE' or obj.dragengine_physics == 'CAPSULE':
				pass # todo
				
			elif rbshape == 'CONE':
				pass # todo
				
			elif rbshape == 'CONVEX_HULL':
				pass # todo
				
			elif rbshape == 'MESH':
				pass # todo
			
			if parameters:
				shapes.append(':'.join(parameters))
		
		if shapes:
			text.write("<!-- single shapes -->\n")
			for shape in shapes:
				text.write("<string name='shape'>{}</string>\n".format(shape))
			text.write("\n")
			
			if len(shapes) > 1:
				text.write("<!-- shape-list -->\n")
				text.write("<string name='shapes'>{}</string>\n".format(' '.join(shapes)))
				text.write("\n")
		
		if specialShapes:
			text.write("<!-- special shapes -->\n")
			for shape in specialShapes:
				text.write("<map name='{}'>\n".format(shape[0]))
				for entry in shape[1:]:
					text.write("\t{}\n".format(entry))
				text.write("</map>\n")
			text.write("\n")
		
		# curves as vector list
		curves = []
		
		for obj in context.selected_objects:
			if obj.type != 'CURVE':
				continue
			
			volume = Armature.CollisionVolume(obj, scalePosition)
			if self.relative3DCursor:
				volume.setMatrix(convertMatrix(matmul(relativeMatrix, volume.object.matrix_world)), scalePosition)
			
			points = []
			
			for spline in obj.data.splines:
				for point in spline.points:
					pos = vector_by_matrix(volume.matrix, point.co)
					points.append("<vector x='{}' y='{}' z='{}'/>".format(float2String(pos.x),
						float2String(pos.y), float2String(pos.z)))
			
			if points:
				curves.append(points)
		
		if curves:
			text.write("<!-- curves -->\n")
			
			for curve in curves:
				text.write("<list name='curve'>\n")
				for point in curve:
					text.write("\t{}\n".format(point))
				text.write("</list>\n")
			text.write("\n")
		
		# empties as vector
		empties = []
		
		for obj in context.selected_objects:
			if obj.type != 'EMPTY':
				continue
			
			volume = Armature.CollisionVolume(obj, scalePosition)
			if self.relative3DCursor:
				volume.setMatrix(convertMatrix(matmul(relativeMatrix, volume.object.matrix_world)), scalePosition)
			
			pos = volume.position
			rot = volume.rotation
			
			parameters = []
			parameters.append("<vector key='position' x='{}' y='{}' z='{}'/>".format(
				float2String(pos.x), float2String(pos.y), float2String(pos.z)))
			if rot.length > 1e-4:
				parameters.append("<vector key='rotation' x='{}' y='{}' z='{}'/>".format(
					float2String(rot.x), float2String(rot.y), float2String(rot.z)))
			empties.append(parameters)
		
		if empties:
			text.write("<!-- empties -->\n")
			
			for empty in empties:
				text.write("<map name='empty'>\n")
				for parameter in empty:
					text.write("\t{}\n".format(parameter))
				text.write("</map>\n")
			text.write("\n")
		
		# move to top so text is well visible
		text.current_line_index = 0
		
		return {'FINISHED'}
registerClass(OBJECT_OT_DEToolEClassProperty)
