# -*- coding: utf-8 -*-
#
# Drag[en]gine Blender Scripts
#
# Copyright (C) 2021, Plüss Roland (roland@rptd.ch)
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
import mathutils
import xml.etree.ElementTree as ET

from bpy_extras.io_utils import ImportHelper
from mathutils import Vector, Matrix, Euler, Quaternion

from .de_math import transformPositionInv, transformBonePositionInv
from .de_math import convertMatrixBoneInv
from .de_math import deRotationMatrix, deTranslationMatrix
from .de_configuration import Configuration
from .de_porting import registerClass, matmul


class OBJECT_OT_ImportRig(bpy.types.Operator, ImportHelper):
	bl_idname = "dragengine.import_rig"
	bl_label = "Rigs (.derig)"
	__doc__ = """Import as Drag[en]gine Rig Resource"""
	filename_ext = ".derig"
	filter_glob: bpy.props.StringProperty(default="*.derig", options={'HIDDEN'})
	debug_level: bpy.props.EnumProperty(items = (
		('0', "None", "Output no debug messages."),
		('1', "Basic", "Output basic amount of debug messages."),
		('2', "Verbose", "Output lots of debug messages."),
		('3', "Debug", "Output very large amount of debug messages."),
		), name = "Logging", description = "Choose amount of logging", default = '1')
	bone_length: bpy.props.FloatProperty(name="Bone Length", description="Length of bones", default=0.1, min=0, soft_max=1)
	
	def __init__(self):
		self.debugLevel = 1
		self.boneLength = 0.1
	
	def draw(self, context):
		layout = self.layout
		
		box = layout.box()
		column = box.column(align=True)
		column.prop(self, "debug_level", expand=False)
	
		column = box.column(align=True)
		column.prop(self, "bone_length", expand=False)
	
	def execute(self, context):
		self.debugLevel = int(self.debug_level)
		self.boneLength = self.bone_length
		
		self.report({'INFO'}, "import from '{}'...".format(self.filepath))
		
		configuration = Configuration("deimport.config")
		if configuration.parseConfig():
			configuration.printConfig()
		
		self.scaling = context.scene.dragengine_scaling
		self.scalePosition = Matrix(((self.scaling,0,0,0), (0,self.scaling,0,0), (0,0,self.scaling,0), (0,0,0,1)))
		self.scalePositionInv = Matrix(((1/self.scaling,0,0,0), (0,1/self.scaling,0,0), (0,0,1/self.scaling,0), (0,0,0,1)))
		self.transformScalePosition = matmul(transformPositionInv, self.scalePositionInv)
		self.transformBoneScalePosition = matmul(transformBonePositionInv, self.scalePositionInv)
		
		self.importFile(context)
		return {'FINISHED'}
		
	def importFile(self, context):
		tree = ET.parse(self.filepath)
		
		# create armature
		bpy.ops.object.armature_add(enter_editmode=True, align='WORLD', location=(0, 0, 0), scale=(1, 1, 1))
		objArm = context.active_object
		objArm.animation_data_create()
		objArm.name = "imported rig"
		armature = objArm.data
		armature.name = "imported rig"
		bones = armature.edit_bones
		
		# remove all bones
		while len(bones) > 0:
			bones.remove(bones[0])
		
		# iterate bones and add them
		root = tree.getroot()
		for tagBone in root.iter("bone"):
			bone = bones.new(tagBone.get("name"))
			
			position = tagBone.find("position")
			if position != None:
				position = (float(position.attrib.get("x")), float(position.get("y")), float(position.get("z")))
			else:
				position = Vector((0, 0, 0))
			
			rotation = tagBone.find("rotation")
			if rotation != None:
				rotation = tuple([float(rotation.get(x)) for x in ["x", "y", "z"]])
			else:
				rotation = Vector((0, 0, 0))
			
			matrix = matmul(deTranslationMatrix(position), deRotationMatrix(rotation))
			matrix = convertMatrixBoneInv(matmul(self.scalePositionInv, matrix))
			
			bone.tail = Vector((0, self.boneLength, 0))  # required or bone is deleted
			bone.matrix = matrix  # only this sets tail and roll correctly
			
			# TODO IK Limits, Shapes
			
		return True
registerClass(OBJECT_OT_ImportRig)
