# -*- coding: utf-8 -*-
#
# Drag[en]gine Blender Scripts
#
# Copyright (C) 2016, Plüss Roland (roland@rptd.ch)
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
import blf
import bgl
import os
import re
import math
import time
import struct
import mathutils

from bpy_extras.io_utils import ImportHelper
from mathutils import Vector, Matrix, Euler

from .de_math import transformPosition, transformBonePosition, transformPositionInv, transformBonePositionInv
from .de_math import convertMatrix, convertMatrixBone, ONE_PI
from .de_math import vector_by_matrix, matrixToEuler, vecLength, vecSub, quatDot, quatNegate
from .de_configuration import Configuration
from .de_resources import Mesh, Armature
from .de_porting import registerClass, matmul
from .de_helpers import ActionFCurvesBuilder



class OBJECT_OT_ImportAnimation(bpy.types.Operator, ImportHelper):
	bl_idname = "dragengine.import_animation"
	bl_label = "Animations (.deanim)"
	__doc__ = """Import as Drag[en]gine Animation Resource"""
	filename_ext = ".deanim"
	filter_glob: bpy.props.StringProperty(default="*.deanim", options={'HIDDEN'})
	debug_level: bpy.props.EnumProperty(items = (
		('0', "None", "Output no debug messages."),
		('1', "Basic", "Output basic amount of debug messages."),
		('2', "Verbose", "Output lots of debug messages."),
		('3', "Debug", "Output very large amount of debug messages."),
		), name = "Logging", description = "Choose amount of logging", default = '1')
	import_mode: bpy.props.EnumProperty(items = (
		('0', "Import All", "Import all moves."),
		('1', "Import Single", "Import single move."),
		), name = "Import Mode", description = "Import mode", default = '0')
	#import_move: bpy.props.EnumProperty(items=importActionList)
	import_move: bpy.props.StringProperty(default="")
	
	def __init__(self):
		self.mesh = None
		self.armature = None
		self.moves = []
		self.debugLevel = 1
		self.importMode = 0
	
	
	
	def draw(self, context):
		layout = self.layout
		
		box = layout.box()
		column = box.column(align=True)
		column.prop(self, "debug_level", expand=False)
		
		layout.row(align=True).label(text="Move importing")
		box = layout.box()
		column = box.column(align=True)
		row = column.row(align=True)
		row.prop(self, "import_mode", expand=True)
		
		row = column.row(align=True)
		row.prop(self, "import_move", expand=True, text="")
		row.enabled = (self.import_mode == '1')
	
	
	
	@classmethod
	def poll(cls, context):
		return context.active_object != None
	
	def execute(self, context):
		timeBegin = time.time()
		
		self.debugLevel = int(self.debug_level)
		self.importMode = int(self.import_mode)
		
		self.report({'INFO'}, "import from '%s'..." % (self.filepath))
		
		configuration = Configuration("deimport.config")
		if configuration.parseConfig():
			configuration.printConfig()
		
		self.scaling = context.scene.dragengine_scaling
		self.scalePosition = Matrix(((self.scaling,0,0,0), (0,self.scaling,0,0), (0,0,self.scaling,0), (0,0,0,1)))
		self.scalePositionInv = Matrix(((1/self.scaling,0,0,0), (0,1/self.scaling,0,0), (0,0,1/self.scaling,0), (0,0,0,1)))
		self.transformScalePosition = matmul(transformPositionInv, self.scalePositionInv)
		self.transformBoneScalePosition = matmul(transformBonePositionInv, self.scalePositionInv)
		
		self.animFPS = context.scene.dragengine_fps
		self.transformTime = 1.0 / float(self.animFPS)
		self.animLimitVarScale = configuration.getValueFor("animation.limits.variable.scale", 0.01)
		
		self.importFile(context)
		
		self.report({'INFO'}, "finished in {:.1f}s".format(time.time() - timeBegin))
		
		return {'FINISHED'}
	
	def importFile(self, context):
		self.initImporterObjects(context)
		if not self.checkInitState(context):
			return False
		
		f = open(self.filepath, "rb")
		try:
			result = self.safeImport(context, f)
		finally:
			f.close()
		return result
	
	def initFindMeshArmRef(self, context):
		for object in context.selected_objects:
			if object.type == 'MESH':
				if not self.mesh:
					self.mesh = Mesh(object)
					if object.parent and object.parent.type == 'ARMATURE':
						if not self.armature:
							self.armature = Armature(object.parent)
			elif object.type == 'ARMATURE':
				if not self.armature:
					self.armature = Armature(object)
		if self.mesh and self.armature:
			self.mesh.armature = self.armature
	
	def initImporterObjects(self, context):
		self.initFindMeshArmRef(context)
		if self.armature:
			self.armature.initAddBones()
	
	def checkInitState(self, context):
		if not self.armature and not self.mesh:
			self.report({'INFO', 'ERROR'}, "There is no Armature or Mesh selected. Select at last one Armature or Mesh parented to an armature")
			return False
		return True
	
	def safeImport(self, context, f):
		factor = 1.0 / 3.0
		
		if not self.prepareBones():
			return False
		
		if not self.readHeader(f):
			return False
		
		if not self.readAnimation(f):
			return False
		
		if not self.readBones(f):
			return False
		
		if not self.readMoves(f):
			return False
		
		if not self.readTrailer(f):
			return False
		
		return True
	
	def roundToInt(self, value):
		return int(value + 0.5)
	
	# prepare bones
	def prepareBones(self):
		if self.debugLevel > 0:
			print("prepare bones...")
		
		for bone in self.armature.bones:
			realBone = bone.bone
			bone.rigMat = convertMatrixBone(realBone.matrix_local)
			bone.restMat = bone.rigMat
			if realBone.parent:
				bone.restMat = matmul(convertMatrixBone(realBone.parent.matrix_local).inverted(), bone.restMat)
			bone.pos = vector_by_matrix(matmul(self.scalePosition, bone.restMat), Vector((0,0,0)))
			bone.rot = matrixToEuler(bone.restMat)
		
		return True
	
	def readHeader(self, f):
		if f.read(24) != bytes("Drag[en]gine Animation  ", 'UTF-8'):
			raise "Signature wrong."
		self.importFlags = struct.unpack("<BB", f.read(2))
		return True
	
	def readAnimation(self, f):
		return True
	
	def readBones(self, f):
		if self.debugLevel > 0:
			print("reading bones...")
		
		boneCount = struct.unpack("<B", f.read(1))[0]
		self.importBones = []
		
		for i in range(boneCount):
			bone = {}
			nameLen = struct.unpack("<B", f.read(1))[0]
			bone["name"] = f.read(nameLen).decode("UTF-8")
			if self.debugLevel > 1:
				print("- bone", bone["name"])
			self.importBones.append(bone)
		
		return True
	
	# tests if a sequence of keyframes is linear
	def areKeyframesLinear(self, keyframes, firstIndex, lastIndex):
		kf1 = keyframes[firstIndex]
		kf2 = keyframes[lastIndex]
		
		# calculate differences
		mulTime = 1.0 / (kf2.time - kf1.time)
		diffPos = kf2.pos - kf1.pos
		diffRot = kf2.rot - kf1.rot
		diffScale = kf2.scale - kf1.scale
		
		# test all keyframes in between against this differences
		for index in range(1, lastIndex):
			kf = keyframes[index]
			factor = (kf.time - kf1.time) * mulTime
			expectedPos = kf1.pos + diffPos * factor
			expectedRot = kf1.rot + diffRot * factor
			expectedScale = kf1.scale + diffScale * factor
			
			if (kf.pos - expectedPos).length > self.armature.thresholdPosition \
			or (kf.rot - expectedRot).length > self.armature.thresholdRotation \
			or (kf.scale - expectedScale).length > self.armature.thresholdScale:
				return False
		
		return True
	
	# optimize the keyframes by dropping keyframes inside linear changes
	def optimizeKeyframes(self, moveBones):
		for moveBone in moveBones:
			if len(moveBone.keyframes) > 0:
				keyframes = [moveBone.keyframes[0]]
				firstIndex = 0
				
				if len(moveBone.keyframes) > 1:
					for index in range(1, len(moveBone.keyframes)):
						if not self.areKeyframesLinear(moveBone.keyframes, firstIndex, index):
							keyframes.append(moveBone.keyframes[index])
							firstIndex = index
				
				moveBone.keyframes = keyframes
	
	# read moves
	def readMoves(self, f):
		if self.debugLevel > 0:
			print("read moves...")
		
		# read moves from file
		moveCount = struct.unpack("<H", f.read(2))[0]
		
		# activate armature object
		#self.armature.object.select = True
		#bpy.context.scene.objects.active = self.armature.object
		
		# process moves
		progressCounter = 0
		
		for i in range(moveCount):
			self.report({'INFO'}, "{}/{} ({:.2f}%): Reading Move {}...".format(progressCounter, \
				moveCount, float(progressCounter) / float(moveCount), i))
			
			# determine the time scaling
			self.timeScale = self.transformTime
			
			# read move informations
			nameLen = struct.unpack("<B", f.read(1))[0]
			moveName = f.read(nameLen).decode("UTF-8")
			
			playTimeSeconds = struct.unpack("<f", f.read(4))[0]
			playTimeFrames = struct.unpack("<H", f.read(2))[0]
			
			if self.debugLevel > 0:
				print("- move", moveName, "playtime", playTimeSeconds)
			
			# read bones
			moveBones = []
			
			for importBone in self.importBones:
				flags = struct.unpack("<B", f.read(1))[0]
				
				hasVarPos = (flags & 0x01) == 0x01
				hasVarRot = (flags & 0x02) == 0x02
				hasVarScale = (flags & 0x04) == 0x04
				hasFewKeyframes = (flags & 0x08) == 0x08
				hasKeyFrames = (flags & 0x10) != 0x10
				shortFormat = (flags & 0x20) != 0x20
				
				moveBone = Armature.MoveBone(importBone)
				moveBone.hasPos = False
				moveBone.hasRot = False
				moveBone.hasScale = False
				
				if hasKeyFrames:
					moveBone.hasPos = hasVarPos
					moveBone.hasRot = hasVarRot
					moveBone.hasScale = hasVarScale
					
					if hasVarPos or hasVarRot or hasVarScale:
						countKeyframes = playTimeFrames
						if hasFewKeyframes:
							countKeyframes = struct.unpack("<H", f.read(2))[0]
						
						# read keyframes
						for j in range(countKeyframes):
							kfFrame = struct.unpack("<H", f.read(2))[0]
							keyframe = Armature.Keyframe(kfFrame, Vector(), Vector(), Vector((1,1,1)))
							
							# if we have a variable position read it. if short format is used encoded as signed 16-bit.
							if hasVarPos:
								if shortFormat:
									keyframe.pos = Vector([x / 1000.0 for x in struct.unpack("<hhh", f.read(6))])
								else:
									keyframe.pos = Vector(struct.unpack("<fff", f.read(12)))
							
							# if we have a variable rotation read it. if short format is used encoded as signed 16-bit.
							if hasVarRot:
								if shortFormat:
									keyframe.rot = Vector([x / 100.0 for x in struct.unpack("<hhh", f.read(6))])
								else:
									keyframe.rot = Vector([x / ONE_PI for x in struct.unpack("<fff", f.read(12))])
							
							# if we have a variable scale read it. if short format is used encoded as signed 16-bit.
							if hasVarScale:
								if shortFormat:
									keyframe.scale = Vector([x / 1000.0 for x in struct.unpack("<hhh", f.read(6))])
								else:
									keyframe.scale = Vector(struct.unpack("<fff", f.read(12)))
							
							moveBone.keyframes.append(keyframe)
							
							if self.debugLevel > 1:
								print("- bone", importBone["name"], "keyframe", float(kfFrame) * self.timeScale,
									"pos", keyframe.pos, "rot", keyframe.rot, "scale", keyframe.scale)
				
				moveBones.append(moveBone)
			
			# import if selected
			arm = self.armature.armature
			
			if self.import_mode == '0' or moveName == self.import_move:
				action = bpy.data.actions.new("imported " + moveName)
				action.dragengine_export = False
				action.dragengine_exportname = moveName
				action.use_fake_user=True
				
				if not self.armature.object.animation_data:
					self.armature.object.animation_data_create()
				
				self.armature.object.animation_data.action = action
				bpy.ops.object.mode_set(mode='POSE')
				
				pose = self.armature.object.pose
				agroups = action.groups
				
				for moveBone in moveBones:
					if not moveBone.keyframes:
						continue
					
					boneName = moveBone.bone["name"]
					print("- bone '{}'".format(boneName))
					agroups.new(boneName)
					
					# create fcurves using build. reduces import time roughly to 20%
					builder = ActionFCurvesBuilder(action, boneName)
					
					if moveBone.hasPos:
						for keyframe in moveBone.keyframes:
							builder.addPosition(keyframe.time, vector_by_matrix(self.transformBoneScalePosition, keyframe.pos))
					else:
						builder.addPosition(0, Vector())
					
					if moveBone.hasRot:
						for keyframe in moveBone.keyframes:
							rotation = [keyframe.rot[0] * ONE_PI, keyframe.rot[2] * ONE_PI, keyframe.rot[1] * ONE_PI]
							builder.addRotation(keyframe.time, Euler(rotation, 'YXZ').to_quaternion())
					else:
						builder.addRotation(0, Quaternion())
					
					if moveBone.hasScale:
						for keyframe in moveBone.keyframes:
							builder.addScale(keyframe.time, keyframe.scale)
					else:
						builder.addScale(0, Vector((1,1,1)))
					
					builder.build()
					
					"""
					poseBone = pose.bones[boneName]
					
					if moveBone.hasPos:
						for keyframe in moveBone.keyframes:
							poseBone.location = vector_by_matrix(self.transformBoneScalePosition, keyframe.pos)
							poseBone.keyframe_insert(data_path="location", frame=keyframe.time, group=boneName)
					
					if moveBone.hasRot:
						for keyframe in moveBone.keyframes:
							rotation = [keyframe.rot[0] * ONE_PI, keyframe.rot[2] * ONE_PI, keyframe.rot[1] * ONE_PI]
							poseBone.rotation_quaternion = Euler(rotation, 'YXZ').to_quaternion()
							poseBone.keyframe_insert(data_path="rotation_quaternion", frame=keyframe.time, group=boneName)
							#poseBone.rotation_euler = Euler(rotation, 'YXZ').to_quaternion()
							#poseBone.keyframe_insert(data_path="rotation_euler", frame=keyframe.time, group=boneName)
					
					if moveBone.hasScale:
						for keyframe in moveBone.keyframes:
							poseBone.scale = keyframe.scale
							poseBone.keyframe_insert(data_path="scale", frame=keyframe.time, group=boneName)
					"""
			
			# next round
			progressCounter = progressCounter + 1
			
		return True
	
	# read trailer
	def readTrailer(self, f):
		return True
registerClass(OBJECT_OT_ImportAnimation)
