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



# Tool Rounding
#################

class OBJECT_OT_DEToolRounding( bpy.types.Operator ):
	bl_idname = "dragengine.rounding"
	bl_label = "Drag[en]gine Rounding"
	bl_options = { 'REGISTER', 'UNDO' }
	__doc__ = """Round coordinates"""
	
	digits: bpy.props.IntProperty( name="Digits", description="Digits to round to", soft_min=0, soft_max=5, default=5 )
	
	@classmethod
	def poll( cls, context ):
		return context.active_object != None \
			and context.active_object.type == 'MESH'
	
	def execute( self, context ):
		editmode = ( bpy.context.mode == 'EDIT_MESH' )
		if editmode:
			bpy.ops.object.mode_set( mode='OBJECT' )
		
		for vertex in context.active_object.data.vertices:
			if vertex.select:
				vertex.co.x = round( vertex.co.x, self.digits )
				vertex.co.y = round( vertex.co.y, self.digits )
				vertex.co.z = round( vertex.co.z, self.digits )
		
		if editmode:
			bpy.ops.object.mode_set( mode='EDIT' )
		
		return { 'FINISHED' }
registerClass(OBJECT_OT_DEToolRounding)
