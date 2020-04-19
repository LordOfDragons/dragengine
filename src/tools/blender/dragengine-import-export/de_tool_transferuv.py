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

from .de_math import ONE_PI, vecSub, vecLength
from .de_porting import registerClass



# Tool Transfer UV
####################

class OBJECT_OT_ToolTransferUV( bpy.types.Operator ):
	bl_idname = "dragengine.transferuv"
	bl_label = "Transfer UV"
	bl_options = { 'REGISTER', 'UNDO' }
	__doc__ = """Transfer texture coordinates from vertices of the active object to vertices of the selected objects"""
	
	
	
	@classmethod
	def poll( cls, context ):
		return context.active_object != None \
			and context.active_object.type == 'MESH' \
			and context.active_object.data.uv_layers.active \
			and len( context.selected_objects ) > 1
	
	def execute( self, context ):
		editmode = ( bpy.context.mode == 'EDIT_MESH' )
		if editmode:
			bpy.ops.object.mode_set( mode='OBJECT' )
		
		sourceMesh = context.active_object.data
		sourceUVs = sourceMesh.uv_layers.active.data
		targetMeshes = [ x.data for x in context.selected_objects if x.type == 'MESH' and not x.data == sourceMesh ]
		
		for targetMesh in targetMeshes:
			targetUVs = targetMesh.uv_layers.active.data
			
			for targetFace in targetMesh.polygons:
				if targetFace.select and not targetFace.hide:
					targetFaceCenter = targetFace.center
					
					for corner in range( targetFace.loop_total ):
						vertexPosition = targetMesh.vertices[ targetMesh.loops[ targetFace.loop_start + corner ].vertex_index ].co
						
						closestSourceVertex = None
						closestSourceVertexDistance = 0.0
						for sourceVertex in sourceMesh.vertices:
							testDistance = vecLength( vecSub( vertexPosition, sourceVertex.co ) )
							if not closestSourceVertex or testDistance < closestSourceVertexDistance:
								closestSourceVertex = sourceVertex
								closestSourceVertexDistance = testDistance
						
						if closestSourceVertex:
							closestSourceFace = None
							closestSourceFaceDistance = 0.0
							for sourceFace in sourceMesh.polygons:
								if closestSourceVertex.index in sourceFace.vertices:
									testDistance = vecLength( vecSub( targetFaceCenter, sourceFace.center ) )
									if not closestSourceFace or testDistance < closestSourceFaceDistance:
										closestSourceFace = sourceFace
										closestSourceFaceDistance = testDistance
							
							if closestSourceFace:
								sourceUVIndex = closestSourceFace.loop_start + closestSourceFace.vertices[:].index( closestSourceVertex.index )
								targetUVs[ targetFace.loop_start + corner ].uv = sourceUVs[ sourceUVIndex ].uv[:]
		
		if editmode:
			bpy.ops.object.mode_set( mode='EDIT' )
		
		return { 'FINISHED' }
registerClass(OBJECT_OT_ToolTransferUV)
