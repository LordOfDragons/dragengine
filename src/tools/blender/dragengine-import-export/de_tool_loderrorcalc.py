# -*- coding: utf-8 -*-
#
# Drag[en]gine Blender Scripts
#
# Copyright (C) 2011, Plüss Roland (roland@rptd.ch)
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

from mathutils import Vector

from .de_math import Octree, closestPointOnTriangle, closestPointOnQuad, bvecL, bvecLE, bvecG, bvecGE
from .de_resources import MeshOctree
from .de_porting import registerClass



# Tool LOD Error Calculation
##############################

bpy.types.Scene.dragengine_lodmaxerror = bpy.props.FloatProperty(name="Max Error",
	description="Result max error", precision=3, default=0)
bpy.types.Scene.dragengine_lodavgerror = bpy.props.FloatProperty(name="Average Error",
	description="Result average error", precision=3, default=0)

def calculateTestLod(prop, context):
	fov = context.scene.dragengine_lodtestfov
	screenSize = context.scene.dragengine_lodtestscreensize
	pixelError = context.scene.dragengine_lodtestpixelerror
	error = context.scene.dragengine_lodtesterror
	context.scene.dragengine_lodtestresultdistance = (
		error * (float(screenSize) * 0.5) /
			(float(pixelError) * math.tan(math.radians(fov * 0.5))))

bpy.types.Scene.dragengine_lodtestfov = bpy.props.IntProperty(name="Camera FOV",
	description="Test camera field of view in degrees", default=90,
	min=1, soft_min=60, soft_max=90, max=179, update=calculateTestLod)
bpy.types.Scene.dragengine_lodtestscreensize = bpy.props.IntProperty(name="Screen Size",
	description="Size of test screen in pixels", default=1680,
	min=1, soft_min=1024, soft_max=1920, update=calculateTestLod)
bpy.types.Scene.dragengine_lodtestpixelerror = bpy.props.IntProperty(name="Pixel Error",
	description="Test maximum screen pixel error", default=2, min=1, soft_max=10, update=calculateTestLod)
bpy.types.Scene.dragengine_lodtesterror = bpy.props.FloatProperty(name="Error",
	description="Test LOD error to calculate distance for", precision=3,
	default=0.01, min=0, soft_min=0.001, soft_max=0.01, update=calculateTestLod)
bpy.types.Scene.dragengine_lodtestresultdistance = bpy.props.FloatProperty(name="Distance",
	description="Test result lod switch distance", precision=2, default=4.2)

class OBJECT_OT_DEToolLODInfo(bpy.types.Operator):
	class Visitor(Octree.Visitor):
		def __init__(self, vertices):
			self.vertices = vertices
			self.boxMinExtend = Vector()
			self.boxMaxExtend = Vector()
			self.lodPoint = Vector()
			self.minDistance = 0.0
			self.abordDistance = 0.0
			self.visitNext = True
			
		def visitNode(self, node, intersection):
			if not self.visitNext:
				return
			
			nodeInside = intersection == Octree.INSIDE
			for face in node.faces:
				if not nodeInside and not (
					bvecLE(self.boxMinExtend, face.maxextend)
					and bvecGE(self.boxMaxExtend, face.minextend)):
						continue
				
				"""
				faceVertices = face.face.vertices
				vcount = len(faceVertices)
				if vcount == 3:
					co1 = self.vertices[faceVertices[0]].co
					co2 = self.vertices[faceVertices[1]].co
					co3 = self.vertices[faceVertices[2]].co
					self.minDistance = min(self.minDistance, (self.lodPoint
						- closestPointOnTriangle(co1, co2, co3, self.lodPoint)).length)
					if self.minDistance < self.abordDistance:
						self.visitNext = False
						return
				elif vcount == 4:
					co1 = self.vertices[faceVertices[0]].co
					co2 = self.vertices[faceVertices[1]].co
					co3 = self.vertices[faceVertices[2]].co
					co4 = self.vertices[faceVertices[3]].co
					self.minDistance = min(self.minDistance, (self.lodPoint
						- closestPointOnQuad(co1, co2, co3, co4, self.lodPoint)).length)
					if self.minDistance < self.abordDistance:
						self.visitNext = False
						return
				else:
					co1 = self.vertices[faceVertices[0]].co
					co3 = self.vertices[faceVertices[1]].co
					for i in range(2, len(faceVertices)):
						co2 = co3
						co3 = self.vertices[faceVertices[i]].co
						self.minDistance = min(self.minDistance, (self.lodPoint
							- closestPointOnTriangle(co1, co2, co3, self.lodPoint)).length)
						if self.minDistance < self.abordDistance:
							self.visitNext = False
							return
				"""
				self.minDistance = min(self.minDistance,
					(self.lodPoint - face.closestPoint(self.lodPoint)).length)
				if self.minDistance >= self.abordDistance:
					continue
				
				self.visitNext = False
				return
	
	bl_idname = "dragengine.lodinfo"
	bl_label = "Drag[en]gine LOD Info"
	bl_options = { 'INTERNAL' }
	__doc__ = """LOD Information"""
	
	calcMaxError = bpy.props.BoolProperty(name="Calc Max Error",
		description="Calculate maximum error", default=False)
	calcAvgError = bpy.props.BoolProperty(name="Calc Avg Error",
		description="Calculate average error", default=False)
	
	@classmethod
	def poll(cls, context):
		return True
	
	def execute(self, context):
		selection = [o for o in context.selected_objects if o.type == 'MESH']
		
		if len(selection) == 2:
			self.calculateLod(context, selection[1], selection[0])
		else:
			context.scene.dragengine_lodmaxerror = 0
			context.scene.dragengine_lodavgerror = 0
		
		return { 'FINISHED' }
	
	def calculateLod(self, context, objBase, objLod):
		if not self.calcMaxError and not self.calcAvgError:
			context.scene.dragengine_lodmaxerror = 0
			context.scene.dragengine_lodavgerror = 0
			return
		
		scaling = context.scene.dragengine_scaling
		invScaling = 1.0 / scaling
		boxExtends = Vector((1,1,1)) * invScaling
		initLODDistance = boxExtends.length
		
		print("baseMesh='{0}' lodMesh='{1}'".format(objBase.name, objLod.name))
		print("initLODDistance={0:.6f}".format(initLODDistance * scaling))
		
		maxError = 0
		avgError = 0
		
		octree = MeshOctree.createFromMesh(objLod.data)
		#octree.printTree("", 0)
		visitor = OBJECT_OT_DEToolLODInfo.Visitor(objLod.data.vertices)
		
		for vertex in objBase.data.vertices:
			visitor.lodPoint = vertex.co
			visitor.boxMinExtend = visitor.lodPoint - boxExtends
			visitor.boxMaxExtend = visitor.lodPoint + boxExtends
			visitor.minDistance = initLODDistance
			if not self.calcAvgError:
				visitor.abordDistance = maxError
				visitor.visitNext = True
			
			octree.visitNodesColliding(visitor, visitor.boxMinExtend, visitor.boxMaxExtend)
			
			if self.calcMaxError:
				maxError = max(maxError, visitor.minDistance)
			if self.calcAvgError:
				avgError = avgError + visitor.minDistance
		
		octree.clearTree(True)
		
		if self.calcAvgError and len(objBase.data.vertices) > 0:
			avgError = avgError / float(len(objBase.data.vertices))
		
		context.scene.dragengine_lodmaxerror = maxError * scaling
		context.scene.dragengine_lodavgerror = avgError * scaling
registerClass(OBJECT_OT_DEToolLODInfo)

'''
class OBJECT_OT_DEToolLODTest(bpy.types.Operator):
	bl_idname = "dragengine.lodtest"
	bl_label = "Drag[en]gine LOD Test"
	bl_options = { 'INTERNAL' }
	__doc__ = """LOD Testing calculating switch distance for LOD configuration"""
	
	@classmethod
	def poll(cls, context):
		return True
	
	def execute(self, context):
		fov = context.scene.dragengine_lodtestfov
		screenSize = context.scene.dragengine_lodtestscreensize
		pixelError = context.scene.dragengine_lodtestpixelerror
		error = context.scene.dragengine_lodtesterror
		
		context.scene.dragengine_lodtestresultdistance = (
			(error * (float(screenSize) * 0.5) / float(pixelError))
				/ math.tan(math.radians(fov * 0.5)))
		return { 'FINISHED' }
registerClass(OBJECT_OT_DEToolLODTest)
'''
