# -*- coding: utf-8 -*-
#
# Drag[en]gine Blender Scripts
#
# Copyright (C) 2022, Plüss Roland (roland@rptd.ch)
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
import re

from mathutils import Vector

from .de_porting import registerClass


# Tool Spread Animation
#########################

bpy.types.Scene.dragengine_spreadanimtarget = bpy.props.StringProperty(
	name="Target", description="Target Action (Reg-Ex)", default="move\..+")

class OBJECT_OT_DEToolSpreadAnimation(bpy.types.Operator):
	bl_idname = "dragengine.spreadanim"
	bl_label = "Drag[en]gine Spread Animation"
	bl_options = {'INTERNAL'}
	__doc__ = """Spread Animation. Copies animation data of selected bones to all actions matching target regular expression"""
	
	@classmethod
	def poll(cls, context):
		return (context.active_object.pose
			and context.active_object.animation_data.action
			and context.selected_pose_bones)
	
	def execute(self, context):
		pose = context.active_object.pose
		if not pose:
			return {'FINISHED'}
		
		action = context.active_object.animation_data.action
		if not action:
			return {'FINISHED'}
		
		selectedBones = context.selected_pose_bones
		if not selectedBones:
			return {'FINISHED'}
		
		reTarget = re.compile(context.scene.dragengine_spreadanimtarget)
		targets = [x for x in bpy.data.actions if not x == action and reTarget.match(x.name)]
		
		for bone in selectedBones:
			if not bone.name in action.groups:
				continue
			group = action.groups[bone.name]
			#print('spread animation for bone: {}'.format(bone.name))
			
			# remove first all fcurves in all targets linked to bone name
			for target in targets:
				if bone.name in target.groups:
					tgroup = target.groups[bone.name]
					delgroups = [x for x in target.fcurves if x.group == tgroup]
					for delgroup in delgroups:
						target.fcurves.remove(delgroup)
			
			# create copy of all fcurves in all targets
			fcurves = [x for x in action.fcurves if x.group == group]
			
			for target in targets:
				#print('- copy to action: {}'.format(target.name))
				
				for fcurve in fcurves:
					tfcurve = target.fcurves.new(fcurve.data_path, index=fcurve.array_index, action_group=bone.name)
					
					tfcurve.auto_smoothing = fcurve.auto_smoothing
					tfcurve.color = fcurve.color
					tfcurve.color_mode = fcurve.color_mode
					tfcurve.extrapolation = fcurve.extrapolation
					tfcurve.hide = fcurve.hide
					tfcurve.mute = fcurve.mute
					tfcurve.lock = fcurve.lock
					
					tfcurve.keyframe_points.add(len(fcurve.keyframe_points))
					
					for i in range(len(fcurve.keyframe_points)):
						kfp = fcurve.keyframe_points[i]
						tkfp = tfcurve.keyframe_points[i]
						
						tkfp.co = kfp.co
						tkfp.handle_left = kfp.handle_left
						tkfp.handle_right = kfp.handle_right
						tkfp.handle_left_type = kfp.handle_left_type
						tkfp.handle_right_type = kfp.handle_right_type
						tkfp.interpolation = kfp.interpolation
					
					tfcurve.update()
		
		return {'FINISHED'}
registerClass(OBJECT_OT_DEToolSpreadAnimation)
