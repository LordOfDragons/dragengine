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



# Progress display. To use create instance and call update. The instance
# will update the UI whenever it is useful.
class ProgressDisplay:
	## Create progress bar.
	def __init__(self, maximum, operator=None):
		self.maximum = max(maximum, 1)
		self.progress = 0
		self.text = ''
		self.operator = operator
	
	## Show progress bar
	def show(self):
		bpy.context.window_manager.progress_begin(0.0, 100.0)
	
	## Update progress value
	def update(self, progress, text=''):
		self.progress = min(max(progress, 0), self.maximum)
		self.text = text
		
		bpy.context.window_manager.progress_update(float(self.progress) / float(self.maximum))
		
		infotext = "{:.0f}%: {}".format(self.progress * 100 / self.maximum, self.text)
		if self.operator:
			self.operator.report({'INFO'}, infotext)
		#print(infotext)
		
		#bpy.ops.wm.redraw_timer(type='DRAW_WIN_SWAP', iterations=1) # not working
	
	## Advance progress by 1 and update progress
	def advance(self, text=''):
		self.update(self.progress + 1, text)
	
	## Hide progress bar
	def hide(self):
		bpy.context.window_manager.progress_end()


# FCurve builder
class FCurveBuilder:
	## Create builder
	def __init__(self, action, rna, rnaIndex, actionGroup):
		self.fcurve = action.fcurves.new(rna, index=rnaIndex, action_group=actionGroup)
		self.keyframes = []
		
	## Add keyframe
	def add(self, frame, value):
		self.keyframes.append((frame, value))
	
	## Build curve
	def build(self):
		if not self.keyframes:
			return
		
		kfpoints = self.fcurve.keyframe_points
		kfpoints.add(len(self.keyframes))
		
		for i in range(len(self.keyframes)):
			kf = self.keyframes[i]
			kfp = kfpoints[i]
			kfp.co = kf
			kfp.handle_left = kf
			kfp.handle_right = kf
			kfp.handle_left_type = 'AUTO_CLAMPED'
			kfp.handle_right_type = 'AUTO_CLAMPED'
			kfp.interpolation = 'BEZIER'
		
		self.fcurve.update()

class ActionFCurvesBuilder:
	## Create builder
	def __init__(self, action, actionGroup):
		self.rnaBase = "pose.bones[\"{}\"].".format(actionGroup)
		
		rna = self.rnaBase + "location"
		self.fcurveLocationX = FCurveBuilder(action, rna, 0, actionGroup)
		self.fcurveLocationY = FCurveBuilder(action, rna, 1, actionGroup)
		self.fcurveLocationZ = FCurveBuilder(action, rna, 2, actionGroup)
		
		rna = self.rnaBase + "rotation_quaternion"
		self.fcurveRotationX = FCurveBuilder(action, rna, 1, actionGroup)
		self.fcurveRotationY = FCurveBuilder(action, rna, 2, actionGroup)
		self.fcurveRotationZ = FCurveBuilder(action, rna, 3, actionGroup)
		self.fcurveRotationW = FCurveBuilder(action, rna, 0, actionGroup)
		
		rna = self.rnaBase + "scale"
		self.fcurveScaleX = FCurveBuilder(action, rna, 0, actionGroup)
		self.fcurveScaleY = FCurveBuilder(action, rna, 1, actionGroup)
		self.fcurveScaleZ = FCurveBuilder(action, rna, 2, actionGroup)
		
		self.lastRotation = None
	
	## Add position
	def addPosition(self, frame, position):
		self.fcurveLocationX.add(frame, position[0])
		self.fcurveLocationY.add(frame, position[1])
		self.fcurveLocationZ.add(frame, position[2])
	
	## Add rotation
	def addRotation(self, frame, rotation):
		if self.lastRotation and self.lastRotation.dot(rotation) < 0:
			rotation = rotation.copy()
			rotation.negate()
		self.lastRotation = rotation
		
		self.fcurveRotationX.add(frame, rotation[1])
		self.fcurveRotationY.add(frame, rotation[2])
		self.fcurveRotationZ.add(frame, rotation[3])
		self.fcurveRotationW.add(frame, rotation[0])
	
	## Add scale
	def addScale(self, frame, scale):
		self.fcurveScaleX.add(frame, scale[0])
		self.fcurveScaleY.add(frame, scale[1])
		self.fcurveScaleZ.add(frame, scale[2])
	
	## Add all
	def addAll(self, frame, position, rotation, scale):
		self.addPosition(frame, position)
		self.addRotation(frame, rotation)
		self.addScale(frame, scale)
	
	## Build curves
	def build(self):
		self.fcurveLocationX.build()
		self.fcurveLocationY.build()
		self.fcurveLocationZ.build()
		
		self.fcurveRotationX.build()
		self.fcurveRotationY.build()
		self.fcurveRotationZ.build()
		self.fcurveRotationW.build()
		
		self.fcurveScaleX.build()
		self.fcurveScaleY.build()
		self.fcurveScaleZ.build()
