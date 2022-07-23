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



# Tool Mirror Animation
#########################

class OBJECT_OT_DEToolMirrorAnimation( bpy.types.Operator ):
	bl_idname = "dragengine.mirroranimation"
	bl_label = "Drag[en]gine Mirror Animation"
	bl_options = { 'REGISTER', 'UNDO' }
	__doc__ = """Mirror animation"""
	
	#digits: bpy.props.IntProperty( name="Digits", description="Digits to round to", soft_min=0, soft_max=5, default=5 )
	
	@classmethod
	def poll( cls, context ):
		return context.active_object != None and context.active_object.type == 'ARMATURE' \
			and bpy.context.mode == 'POSE' and context.active_object.animation_data \
			and context.active_object.animation_data.action
	
	def execute( self, context ):
		pose = context.active_object.pose
		action = context.active_object.animation_data.action
		
		for posebone in pose.bones:
			if posebone.bone.select:
				if posebone.bone.name in action.groups:
					group = action.groups[ posebone.bone.name ]
					# location and rotation_quaterion can be mirrored channel wise directly
					for fcurve in group.channels:
						if fcurve.data_path[ -9: ] == ".location":
							if fcurve.array_index == 0: # mirror x
								self.flipFCurve( fcurve )
						if fcurve.data_path[ -20: ] == ".rotation_quaternion":
							if fcurve.array_index == 2 or fcurve.array_index == 3: # mirror y and z
								self.flipFCurve( fcurve )
						if fcurve.data_path[ -15: ] == ".rotation_euler":
							if fcurve.array_index == 1 or fcurve.array_index == 2: # mirror y and z
								self.flipFCurve( fcurve )
					# euler rotation are diffult since we can mirror them only as a whole euler
					# not component wise like quaterions or the location unless the bone orientation
					# is specific. for the time being this is not done and left for the user to
					# handle on his own.
					# 
					# eulers could be calculate like this if we have an entire Euler object:
					# 1) construct Euler from channels rotation_euler[0..2] with rotation_mode used
					#    from the posebone: e = Euler( (x, y, z), rotation_mode )
					# 2) calculate matrix: m = e.to_matrix()
					# 3) flip a couple of values in the matrix:
					#    m[0][1], m[0][2], m[1][0], m[2][0] = -m[0][1], -m[0][2], -m[1][0], -m[2][0]
					# 4) retrieve new euler: e = m.to_euler( rotation_mode )
					# 5) set the channel values using e
					#
					# using the convention though that the main rotation of a bone is along the x axis
					# and the secondary around the z axis and the rotation_mode is YXZ then a
					# component wise flipping seems valid in most cases. this one is used above
		return { 'FINISHED' }
	
	# flips the y values of an fcurve around the 0 position
	def flipFCurve( self, fcurve ):
		#print( "flip f-curve %s[%i]" % ( fcurve.data_path, fcurve.array_index ) )
		for keyframe in fcurve.keyframe_points:
			keyframe.co = [ keyframe.co[ 0 ], -keyframe.co[ 1 ] ]
			keyframe.handle_left = [ keyframe.handle_left[ 0 ], -keyframe.handle_left[ 1 ] ]
			keyframe.handle_right = [ keyframe.handle_right[ 0 ], -keyframe.handle_right[ 1 ] ]
registerClass(OBJECT_OT_DEToolMirrorAnimation)
