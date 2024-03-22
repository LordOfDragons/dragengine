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
import math

from .de_math import ONE_PI
from .de_porting import registerClass, appendToMenu



# Tool Project UV
###################

class TypeDETProjectUVTemplate( bpy.types.PropertyGroup ):
	name: bpy.props.StringProperty( name="Name", description="name of the template", default="" )
	scale: bpy.props.FloatVectorProperty( name="Scaling", description="Scaling in U/V direction", precision=3, size=2, default=(1.0, 1.0) )
	offset: bpy.props.FloatVectorProperty( name="Offset", description="Offset in U/V direction", precision=3, size=2, default=(0.0, 0.0) )
	rotation: bpy.props.FloatProperty( name="Rotation", description="Rotation in degrees", precision=3, default=0.0 )
	normal: bpy.props.FloatVectorProperty( name="Normal", description="Per-component multiplier for normal calculation",
		min=0.0, max=1.0, soft_min=0.0, soft_max=1.0, precision=3, size=3, default=(1.0, 1.0, 1.0) )
registerClass(TypeDETProjectUVTemplate)

class OBJECT_OT_ToolProjectUV( bpy.types.Operator ):
	bl_idname = "dragengine.projectuv"
	bl_label = "Project UV"
	bl_label_button = "Project"
	bl_options = { 'REGISTER', 'UNDO' }
	__doc__ = """Project texture coordinates"""
	
	"""def getTemplateNameList( self, context ):
		return self.templateNameList
		
	def onTemplateNameChanged( self, context ):
		print( "selected", self.templateNames )
		return None"""
		
	#templateNames: bpy.props.EnumProperty( items=getTemplateNameList, name = "Templates",
	#	description="Template to modify/use", update=onTemplateNameChanged )
	#templateNames: bpy.props.CollectionProperty( type=bpy.props.StringProperty )
	scale: bpy.props.FloatVectorProperty( name="Scaling", description="Scaling in U/V direction",
		soft_min=0.0, soft_max=10.0, precision=3, size=2, default=(1.0, 1.0) )
	offset: bpy.props.FloatVectorProperty( name="Offset", description="Offset in U/V direction",
		soft_min=-1.0, soft_max=1.0, precision=3, size=2, default=(0.0, 0.0) )
	rotation: bpy.props.FloatProperty( name="Rotation", description="Rotation in degrees",
		soft_min=-180.0, soft_max=180.0, precision=3, default=0.0 )
	normal: bpy.props.FloatVectorProperty( name="Normal", description="Per-component multiplier for normal calculation",
		min=0.0, max=1.0, soft_min=0.0, soft_max=1.0, precision=3, size=3, default=(1.0, 1.0, 1.0) )
	
	@classmethod
	def poll( cls, context ):
		#return context.active_object != None and context.active_object.type == 'MESH' and context.active_object.data.uv_loop_layers.active
		return context.active_object != None \
			and context.active_object.type == 'MESH' \
			and context.active_object.data.uv_layers.active \
			and context.tool_settings.mesh_select_mode[ 2 ]
	
	def invoke( self, context, event ):
		curtemp = context.window_manager.dragengine_tooloptions.projectUVCurrent
		self.scale = curtemp.scale
		self.offset = curtemp.offset
		self.rotation = curtemp.rotation
		self.normal = curtemp.normal
		
		"""self.templateNameList = []
		self.templateNameList.append( ( "Template 1", "Template 1", "", 0 ) )
		self.templateNameList.append( ( "Something else", "Something else", "", 1 ) )
		self.templateNameList.append( ( "Oops", "Oops", "", 2 ) )"""
		
		return self.execute( context )
	
	"""def draw( self, context ):
		layout = self.layout
		#layout.prop( self, "templateNames", expand=False )
		col = layout.column()
		col.prop( self, "scale", expand=True )
		col.prop( self, "offset", expand=True )
		col.prop( self, "rotation", expand=True )
		col.prop( self, "normal", expand=True )"""
	
	def execute( self, context ):
		curtemp = context.window_manager.dragengine_tooloptions.projectUVCurrent
		curtemp.scale = self.scale
		curtemp.offset = self.offset
		curtemp.rotation = self.rotation
		curtemp.normal = self.normal
		
		scaleU = self.scale[ 0 ] * context.scene.dragengine_scaling
		scaleV = self.scale[ 1 ] * context.scene.dragengine_scaling
		
		rotSin = math.sin( self.rotation * ONE_PI )
		rotCos = math.cos( self.rotation * ONE_PI )
		
		editmode = ( bpy.context.mode == 'EDIT_MESH' )
		if editmode:
			bpy.ops.object.mode_set( mode='OBJECT' )
		
		mesh = context.active_object.data
		#uvs = mesh.uv_loop_layers.active.data
		uvs = mesh.uv_layers.active.data
		
		for face in mesh.polygons:
			if face.select and not face.hide:
				axis = [ abs( face.normal[ 0 ] ) * self.normal[ 0 ],
					abs( face.normal[ 1 ] ) * self.normal[ 1 ],
					abs( face.normal[ 2 ] ) * self.normal[ 2 ] ]
				
				uv = []
				if axis[ 2 ] > axis[ 0 ] and axis[ 2 ] > axis[ 1 ]:
					if face.normal[ 2 ] > 0.0:
						for vertexIndex in face.vertices:
							vertex = mesh.vertices[ vertexIndex ]
							uv.append( [ vertex.co[ 0 ], vertex.co[ 1 ] ] )
					else:
						for vertexIndex in face.vertices:
							vertex = mesh.vertices[ vertexIndex ]
							uv.append( [ vertex.co[ 0 ], -vertex.co[ 1 ] ] )
				elif axis[ 0 ] > axis[ 1 ]:
					if face.normal[ 0 ] > 0.0:
						for vertexIndex in face.vertices:
							vertex = mesh.vertices[ vertexIndex ]
							uv.append( [ vertex.co[ 1 ], vertex.co[ 2 ] ] )
					else:
						for vertexIndex in face.vertices:
							vertex = mesh.vertices[ vertexIndex ]
							uv.append( [ -vertex.co[ 1 ], vertex.co[ 2 ] ] )
				else:
					if face.normal[ 1 ] > 0.0:
						for vertexIndex in face.vertices:
							vertex = mesh.vertices[ vertexIndex ]
							uv.append( [ -vertex.co[ 0 ], vertex.co[ 2 ] ] )
					else:
						for vertexIndex in face.vertices:
							vertex = mesh.vertices[ vertexIndex ]
							uv.append( [ vertex.co[ 0 ], vertex.co[ 2 ] ] )
				
				for corner in range( face.loop_total ):
					u1 = uv[ corner ][ 0 ] * scaleU + self.offset[ 0 ]
					v1 = uv[ corner ][ 1 ] * scaleV + self.offset[ 1 ]
					u2 = u1 * rotCos - v1 * rotSin
					v2 = u1 * rotSin + v1 * rotCos
					uvs[ face.loop_start + corner ].uv = [ u2, v2 ]
		
		if editmode:
			bpy.ops.object.mode_set( mode='EDIT' )
		
		return { 'FINISHED' }

registerClass(OBJECT_OT_ToolProjectUV)
appendToMenu(bpy.types.VIEW3D_MT_edit_mesh_faces, OBJECT_OT_ToolProjectUV)
