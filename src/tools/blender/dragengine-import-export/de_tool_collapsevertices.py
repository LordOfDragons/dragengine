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

from .de_porting import registerClass



# Tool Collapse Vertices
##########################

class TypeDETVertex( bpy.types.PropertyGroup ):
	index = bpy.props.IntProperty( name="Index", description="Vertex index", default=0 )
registerClass(TypeDETVertex)

class OBJECT_OT_ToolCollapseVerticesSetTargets( bpy.types.Operator ):
	bl_idname = "dragengine.collapsevertices_settargets"
	bl_label = "Collapse Vertices Set Targets"
	bl_options = { 'INTERNAL' }
	__doc__ = """Sets the targets for vertex collapsing"""
	
	@classmethod
	def poll( cls, context ):
		return context.active_object != None \
			and context.active_object.type == 'MESH'
	
	def execute( self, context ):
		targetList = context.window_manager.dragengine_tooloptions.collapseVerticesTargets
		mesh = context.active_object.data
		
		editmode = ( bpy.context.mode == 'EDIT_MESH' )
		if editmode:
			bpy.ops.object.mode_set( mode='OBJECT' )
		
		while len( targetList ) > 0:
			targetList.remove( len( targetList ) - 1 )
		for v in mesh.vertices:
			if v.select:
				targetList.add().index = v.index
		
		if editmode:
			bpy.ops.object.mode_set( mode='EDIT' )
		return { 'FINISHED' }
registerClass(OBJECT_OT_ToolCollapseVerticesSetTargets)

class OBJECT_OT_ToolCollapseVerticesApply( bpy.types.Operator ):
	bl_idname = "dragengine.collapsevertices_apply"
	bl_label = "Collapse Vertices"
	bl_options = { 'INTERNAL' }
	__doc__ = """Collapse vertices to the previously set targets"""
	
	@classmethod
	def poll( cls, context ):
		return context.active_object != None \
			and context.active_object.type == 'MESH'
	
	def execute( self, context ):
		object = context.active_object
		mesh = object.data
		
		editmode = ( context.mode == 'EDIT_MESH' )
		if editmode:
			bpy.ops.object.mode_set( mode='OBJECT' )
		
		# build list of target vertices and vertices to collapse
		targetList = [ mesh.vertices[ t.index ] for t in context.window_manager.dragengine_tooloptions.collapseVerticesTargets ]
		collapseList = [ [ v, [] ] for v in mesh.vertices if v.select ]
		
		# determine for each collapse vertex the vertices connexted to it by edges
		for e in mesh.edges:
			for c in collapseList:
				if e.vertices[ 0 ] == c[ 0 ].index:
					c[ 1 ].append( e.vertices[ 1 ] )
				elif e.vertices[ 1 ] == c[ 0 ].index:
					c[ 1 ].append( e.vertices[ 0 ] )
		
		# collapse vertices
		for c in collapseList:
			# determine the closest target vertex to the current collapse edge that is connected by an edge
			closestTarget = None
			closestDistance = 0.0
			for t in targetList:
				if t.index in c[ 1 ]:
					distance = ( c[ 0 ].co - t.co ).length
					if not closestTarget or distance < closestDistance:
						closestTarget = t
						closestDistance = distance
			
			# if found set the collapse vertex position to the target vertex position
			if closestTarget:
				c[ 0 ].co = closestTarget.co
		
		# create two groups to temporarily store the collapse and target vertices
		vgroupCollaps = object.vertex_groups.new( name="#DETVC@COLLAPS#" )
		vgroupCollaps.add( index=[ v[ 0 ].index for v in collapseList ], weight=1.0, type='REPLACE' )
		
		vgroupTarget = object.vertex_groups.new( name="#DETVC@TARGET#" )
		vgroupTarget.add( index=[ v.index for v in targetList ], weight=1.0, type='REPLACE' )
		
		# ensure edit mode and vertex selection mode
		bpy.ops.object.mode_set( mode='EDIT' )
		context.tool_settings.mesh_select_mode[ 0 ] = True
		context.tool_settings.mesh_select_mode[ 1 ] = False
		context.tool_settings.mesh_select_mode[ 2 ] = False
		
		# select all collapse and target vertices and remove duplicates in them
		bpy.ops.mesh.select_all( action='DESELECT' )
		object.vertex_groups.active_index = vgroupCollaps.index
		bpy.ops.object.vertex_group_select()
		object.vertex_groups.active_index = vgroupTarget.index
		bpy.ops.object.vertex_group_select()
		bpy.ops.mesh.remove_doubles( threshold=1e-6 )
		
		# select only target vertices to restore the list of target vertices with the changed vertex indices.
		# this allowds to collapse another set of vertices without setting the target again
		bpy.ops.mesh.select_all( action='DESELECT' )
		object.vertex_groups.active_index = vgroupTarget.index
		bpy.ops.object.vertex_group_select()
		
		bpy.ops.object.mode_set( mode='OBJECT' )
		
		newTargetList = context.window_manager.dragengine_tooloptions.collapseVerticesTargets
		while len( newTargetList ) > 0:
			newTargetList.remove( len( newTargetList ) - 1 )
		for v in mesh.vertices:
			if v.select:
				newTargetList.add().index = v.index
		
		# select only the collapse vertices that have not been collapsed. this allows to apply the collapse
		# again on vertices that had no direct connection to target vertices right before the collapsing
		# but do so afterwards
		bpy.ops.object.mode_set( mode='EDIT' )
		bpy.ops.mesh.select_all( action='DESELECT' )
		object.vertex_groups.active_index = vgroupCollaps.index
		bpy.ops.object.vertex_group_select()
		object.vertex_groups.active_index = vgroupTarget.index
		bpy.ops.object.vertex_group_deselect()
		bpy.ops.object.mode_set( mode='OBJECT' )
		
		# remove the temporary groups and restore the edit mode
		object.vertex_groups.remove( vgroupTarget )
		object.vertex_groups.remove( vgroupCollaps )
		
		if editmode:
			bpy.ops.object.mode_set( mode='EDIT' )
		return { 'FINISHED' }
registerClass(OBJECT_OT_ToolCollapseVerticesApply)
