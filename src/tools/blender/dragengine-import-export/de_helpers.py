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
import time


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
	def __init__(self, ash, action, rna, rna_index=0, action_group=None):
		if ash.has_slots:
			self.fcurve = ash.fcurves(action).new(rna, index=rna_index)
			if action_group:
				self.fcurve.group = ash.ensure_group(action, action_group)
		else:
			self.fcurve = action.fcurves.new(rna, index=rna_index,
				action_group=(action_group or ""))
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
	def __init__(self, ash, action, action_group):
		self.rnaBase = "pose.bones[\"{}\"].".format(action_group)
		
		rna = self.rnaBase + "location"
		self.fcurveLocationX = FCurveBuilder(ash, action, rna, 0, action_group)
		self.fcurveLocationY = FCurveBuilder(ash, action, rna, 1, action_group)
		self.fcurveLocationZ = FCurveBuilder(ash, action, rna, 2, action_group)
		
		rna = self.rnaBase + "rotation_quaternion"
		self.fcurveRotationX = FCurveBuilder(ash, action, rna, 1, action_group)
		self.fcurveRotationY = FCurveBuilder(ash, action, rna, 2, action_group)
		self.fcurveRotationZ = FCurveBuilder(ash, action, rna, 3, action_group)
		self.fcurveRotationW = FCurveBuilder(ash, action, rna, 0, action_group)
		
		rna = self.rnaBase + "scale"
		self.fcurveScaleX = FCurveBuilder(ash, action, rna, 0, action_group)
		self.fcurveScaleY = FCurveBuilder(ash, action, rna, 1, action_group)
		self.fcurveScaleZ = FCurveBuilder(ash, action, rna, 2, action_group)
		
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


# Helps with animation slot handling introduced in Blender 4.4
class ActionSlotHelper:
	def __init__(self, object):
		self.object = object
		try:
			slot = object.animation_data.action_slot
			self.has_slots = True
			if slot:
				self.slot_id = slot.identifier
				self.slot_type = object.type
			else:
				self.slot_id = object.name
				self.slot_type = 'OBJECT'
		except:
			self.slot_id = None
			self.slot_type = None
			self.has_slots = False
	
	def ensure_slot(self, action):
		sid = '{}{}'.format(self.slot_type[0:2], self.slot_id)
		if sid in action.slots:
			slot = action.slots[sid]
		else:
			slot = action.slots.new(self.slot_type, self.slot_id)
		
		if len(action.layers) > 0:
			layer = action.layers[0]
		else:
			layer = action.layers.new("Layer")
		
		if len(layer.strips) > 0:
			strip = layer.strips[0]
		else:
			strip = layer.strips.new(type='KEYFRAME')
		
		return slot
	
	def groups(self, action):
		if self.has_slots:
			try:
				slot = self.ensure_slot(action)
				return action.layers[0].strips[0].channelbag(slot, ensure=True).groups
			except:
				return action.groups
		else:
			return action.groups
	
	def fcurves(self, action):
		if self.has_slots:
			try:
				slot = self.ensure_slot(action)
				return action.layers[0].strips[0].channelbag(slot, ensure=True).fcurves
			except:
				return action.fcurves
		else:
			return action.fcurves
	
	def ensure_group(self, action, group_name):
		groups = self.groups(action)
		if group_name in groups:
			return groups[group_name]
		else:
			return groups.new(group_name)

class Timer:
	def __init__(self):
		self.timer = time.time()
		
	def elapsed(self, peek=False):
		timer = time.time()
		diff = timer - self.timer
		if not peek:
			self.timer = timer
		return diff
		
	def log(self, text, peek=False):
		e = self.elapsed(peek)
		print('[{:4d}.{:01d}] {}'.format(int(e), int(e * 10) % 10, text))
