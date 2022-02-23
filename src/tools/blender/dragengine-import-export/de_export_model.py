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
import os
import re
import math
import time
import struct

from bpy_extras.io_utils import ExportHelper
from mathutils import Vector, Matrix

from .de_math import transformPosition, convertMatrix, convertMatrixBone
from .de_math import vector_by_matrix, matrixToEuler, writeTexel
from .de_configuration import Configuration
from .de_resources import Mesh, Armature
from .de_helpers import ProgressDisplay
from .de_porting import registerClass, matmul



class OBJECT_OT_ExportModel(bpy.types.Operator, ExportHelper):
	bl_idname = "dragengine.export_model"
	bl_label = "Model (.demodel)"
	__doc__ = """Export as Drag[en]gine Model Resource"""
	filename_ext = ".demodel"
	filter_glob: bpy.props.StringProperty(default="*.demodel", options={ 'HIDDEN' })
	debug_level: bpy.props.EnumProperty(items = (
		('0', "None", "Output no debug messages."),
		('1', "Basic", "Output basic amount of debug messages."),
		('2', "Verbose", "Output lots of debug messages."),
		('3', "Debug", "Output very large amount of debug messages."),
		), name = "Logging", description = "Choose amount of logging", default = '1')
	
	def __init__(self):
		self.mesh = None
		self.armature = None
		self.debugLevel = 1
		self.progress = None
	
	@classmethod
	def poll(cls, context):
		return context.active_object != None
	
	def execute(self, context):
		self.debugLevel = int(self.debug_level)
		self.report({ 'INFO' }, "export to '%s'..." % (self.filepath))
		
		configuration = Configuration("deexport.config")
		if configuration.parseConfig():
			configuration.printConfig()
		
		self.scaling = context.scene.dragengine_scaling
		self.scalePosition = Matrix(((self.scaling,0,0,0), (0,self.scaling,0,0), (0,0,self.scaling,0), (0,0,0,1)))
		self.transformScalePosition = matmul(self.scalePosition, transformPosition)
		
		self.ignoreBones = []
		for f in configuration.getArrayFor("animation.bones.ignore", []):
			self.ignoreBones.append(re.compile(f[0]))
		
		self.export(context)
		
		return { 'FINISHED' }
	
	def export(self, context):
		if not self.initFindMeshArmRef(context):
			return False
		if not self.initChecksEarly(context):
			return False
		self.prepareProgress(context)
		try:
			self.initExporterObjects(context)
			if self.checkInitState(context):
				self.printInfos(context)
				with open(self.filepath, "wb") as f:
					self.safeExport(context, f)
		finally:
			self.progress.hide()
	
	def initFindMeshArmRef(self, context):
		for object in context.selected_objects:
			if object.type == 'MESH':
				if not self.mesh:
					self.mesh = Mesh(object)
					if object.parent and object.parent.type == 'ARMATURE' and not self.armature:
						self.armature = Armature(object.parent)
			elif object.type == 'ARMATURE':
				if not self.armature:
					self.armature = Armature(object)
		if self.mesh and self.armature:
			self.mesh.armature = self.armature
		return self.initLODs(context)
	
	def initLODs(self, context):
		if self.mesh:
			loopProtection = [self.mesh.object.name]
			testmesh = self.mesh
			prevLodLevelHasLodError = False
			predLodError = 0
			while testmesh and testmesh.object and testmesh.object.dragengine_lodmesh in bpy.data.objects:
				if testmesh.object.dragengine_lodmesh in loopProtection:
					self.report({ 'INFO', 'ERROR' }, "Loop in LOD meshes!")
					return False
				loopProtection.append(testmesh.object.dragengine_lodmesh)
				
				testmesh.lodMesh = Mesh(bpy.data.objects[testmesh.object.dragengine_lodmesh])
				
				if testmesh.lodMesh.object.dragengine_hasloderror:
					testmesh.lodMesh.lodError = testmesh.lodMesh.object.dragengine_loderror
					prevLodLevelHasLodError = True
					if testmesh.lodMesh.lodError < predLodError:
						self.report({'INFO', 'ERROR'}, ("LOD mesh '{}' has lower custom LOD error "
							+ "than previous LOD mesh!").format(testmesh.object.name))
						return False
					predLodError = testmesh.lodMesh.lodError
				elif prevLodLevelHasLodError:
					self.report({'INFO', 'ERROR'}, ("LOD mesh '{}' has custom LOD error disabled "
						+ "but previous LOD mesh has LOD error enabled!").format(testmesh.object.name))
					return False
				
				if testmesh.lodMesh.object.parent:
					if (testmesh.lodMesh.object.parent.type == 'ARMATURE'
						and testmesh.lodMesh.object.parent.name != self.armature.object.name):
							self.report({ 'INFO', 'ERROR' }, "LOD mesh has not the same armature as the base mesh!")
							return False
				testmesh.lodMesh.armature = self.armature
				
				testmesh = testmesh.lodMesh
		return True
	
	def initExporterObjects(self, context):
		class Timer:
			def __init__(self):
				from time import time
				self.time = time
				self.timer = self.time()
			def logTime(self, text):
				timer = self.time()
				#print('{} = {}'.format(text, timer - self.timer))
				self.timer = timer
		timer = Timer()
		
		if self.armature:
			self.armature.ignoreBones = self.ignoreBones
			self.armature.initAddBones()
			timer.logTime('armature.initAddBones')
		self.lodMeshCount = 0
		
		mesh = self.mesh
		while mesh:
			self.lodMeshCount = self.lodMeshCount + 1
			
			mesh.initAddTextures()
			timer.logTime('mesh.initAddTextures')
			self.progress.advance("Prepare textures")
			
			mesh.initAddTexCoordSets()
			timer.logTime('mesh.initAddTexCoordSets')
			self.progress.advance("Prepare texture coordinate sets")
			
			mesh.initAddVertices()
			timer.logTime('mesh.initAddVertices')
			self.progress.advance("Prepare vertices")
			
			mesh.initAddEdges()
			timer.logTime('mesh.initAddEdges')
			self.progress.advance("Prepare edges")
			
			mesh.initAddFaces()
			timer.logTime('mesh.initAddFaces')
			self.progress.advance("Prepare faces")
			
			mesh.initCalcCornerNormals()
			timer.logTime('mesh.initCalcCornerNormals')
			self.progress.advance("Calculate corner normals")
			
			mesh.initCalcCornerTangents()
			timer.logTime('mesh.initCalcCornerTangents')
			self.progress.advance("Calculate corner tangents")
			
			mesh.groupTexCoords()
			timer.logTime('mesh.groupTexCoords')
			self.progress.advance("Group texture coordinates")
			
			mesh.initCalcInfoNumbers()
			timer.logTime('mesh.initCalcInfoNumbers')
			self.progress.advance("Calculate info numbers")
			
			mesh = mesh.lodMesh
	
	def prepareProgress(self, context):
		pm = 3  # bones, textures, tex-coord-sets
		mesh = self.mesh
		while mesh:
			pm = pm + 11  # weights, vertices, faces, prepare(8)
			mesh = mesh.lodMesh
		self.progress = ProgressDisplay(pm, self)
		self.progress.show()
	
	def initChecksEarly(self, context):
		if any(x.type == 'MIRROR' for x in self.mesh.object.modifiers):
			self.report({'INFO', 'ERROR'}, ("Mirror modifier found on object '{}'."
				+ " Apply mirror before export otherwise only one half is exported.").format(self.mesh.object.name))
			return False
		
		lodMesh = self.mesh.lodMesh
		while lodMesh:
			if any(x.type == 'MIRROR' for x in lodMesh.object.modifiers):
				self.report({'INFO', 'ERROR'}, ("LOD Mesh '{}': Mirror modifier."
					+ " Apply mirror before export otherwise only one half is exported.").format(lodMesh.object.name))
				return False
			
			lodMesh = lodMesh.lodMesh
		
		return True
	
	def checkInitState(self, context):
		if not self.mesh:
			self.report({ 'INFO', 'ERROR' }, "There is no Mesh selected. Select at last a Mesh and optional an Armature")
			return False
		if self.mesh.multiFoldMesh:
			self.report({ 'INFO', 'ERROR' }, "Can not Multi-Fold meshes (Edges used by more than 2 faces).")
			return False
		if self.mesh.degeneratedFaces:
			self.report({ 'INFO', 'ERROR' }, "Degenerated Faces found.")
			return False
		if self.mesh.ngons:
			self.report({ 'INFO', 'ERROR' }, "NGon faces found.")
			return False
		if not self.mesh.textures:
			self.report({ 'INFO', 'ERROR' }, "No materials found. Add at last one material.")
			return False
		dupName = frozenset([x.exportName for x in self.mesh.textures if
			 len(frozenset([y for y in self.mesh.textures if y.exportName == x.exportName])) > 1])
		if dupName:
			self.report({ 'INFO', 'ERROR' }, "Duplicate texture export names: {}".format(
				", ".join(["'{}'".format(x) for x in dupName])))
			"""if(len(self.mesh.getUVLayerNames()) == 0):
				alertUser('No UV-Layers found. Add at least one UV-Layer.', 'Error')
			else: # len(self.mesh.getUVLayerNames()) > 2
				alertUser('Using more than 1 UV-Layer the default UV-Layer has to be named "default".', 'Error')"""
			return False
		
		if not self.mesh.hasUVSeams and not self.mesh.object.dragengine_hasnoseams:
			self.report({ 'INFO', 'ERROR' }, "No UV-Seams found. Add at least one UV-Seam for Tanget-Calculation to work.")
			return False
		
		self.mesh.largeModel = (self.mesh.normalCount > 65000
			or self.mesh.tangentCount > 65000
			or len(self.mesh.weights) > 65000
			or len(self.mesh.vertices) > 65000
			or len(self.mesh.faces) > 65000
			or any(len(tcs.texCoords) > 65000 for tcs in self.mesh.texCoordSets))
		
		lodMesh = self.mesh.lodMesh
		while lodMesh:
			if lodMesh.multiFoldMesh:
				self.report({'INFO', 'ERROR'}, ("LOD Mesh '{}': Can not Multi-Fold meshes"
					" (Edges used by more than 2 faces).").format(lodMesh.object.name))
				return False
			
			if lodMesh.degeneratedFaces:
				self.report({'INFO', 'ERROR'}, "LOD Mesh '{}': Degenerated Faces found.".format(lodMesh.object.name))
				return False
			
			if lodMesh.ngons:
				self.report({'INFO', 'ERROR'}, "LOD Mesh '{}': NGon faces found.".format(lodMesh.object.name))
				return False
			
			if not self.checkLODMeshSameTextures(lodMesh):
				self.report({'INFO', 'ERROR'}, "LOD Mesh '{}': Textures do not match base mesh '{}' textures.".format(
					lodMesh.object.name, self.mesh.object.name))
				return False
			
			if not self.checkLODMeshSameTexCoordSets(lodMesh):
				self.report({'INFO', 'ERROR'}, ("LOD Mesh '{}': Texture coordinate sets do not"
					+ " match base mesh '{}' texture coordinate sets.").format(lodMesh.object.name, self.mesh.object.name))
				return False
			
			lodMesh.largeModel = (lodMesh.normalCount > 65000
				or lodMesh.tangentCount > 65000
				or len(lodMesh.weights) > 65000
				or len(lodMesh.vertices) > 65000
				or len(lodMesh.faces) > 65000
				or any(len(tcs.texCoords) > 65000 for tcs in lodMesh.texCoordSets))
			
			if not lodMesh.hasUVSeams and not lodMesh.object.dragengine_hasnoseams:
				self.report({'INFO', 'ERROR'}, ("LOD Mesh '{}': No UV-Seams. Add at least"
					+ " one UV-Seam for Tanget-Calculation to work.").format(lodMesh.object.name))
				return False
			
			lodMesh = lodMesh.lodMesh
		
		return True
	
	def checkLODMeshSameTextures(self, lodMesh):
		if len(lodMesh.textures) != len(self.mesh.textures):
			return False
		for i in range(len(self.mesh.textures)):
			if lodMesh.textures[i].name != self.mesh.textures[i].name:
				return False
		return True
	
	def checkLODMeshSameTexCoordSets(self, lodMesh):
		if len(lodMesh.texCoordSets) != len(self.mesh.texCoordSets):
			return False
		for i in range(len(self.mesh.texCoordSets)):
			if lodMesh.texCoordSets[i].name != self.mesh.texCoordSets[i].name:
				return False
		return True
	
	def printInfos(self, context):
		if self.mesh:
			self.mesh.printInfos()
		if self.armature:
			self.armature.printInfos()
	
	def safeExport(self, context, f):
		if not self.writeHeader(f):
			return False
		if not self.writeBones(f):
			return False
		if not self.writeTextures(f):
			return False
		if not self.writeTexCoordSets(f):
			return False
		
		mesh = self.mesh
		while mesh:
			if not self.writeMesh(f, mesh):
				return False
			if not self.writeWeights(f, mesh):
				return False
			if not self.writeVertices(f, mesh):
				return False
			if not self.writeTexCoords(f, mesh):
				return False
			if not self.writeFaces(f, mesh):
				return False
			mesh = mesh.lodMesh
		return True
	
	# write header to file
	def writeHeader(self, f):
		f.write(bytes("Drag[en]gine Model", 'UTF-8'))
		flags = 0
		
		f.write(struct.pack("<HH", 5, flags)) # version, flags
		if self.armature:
			f.write(struct.pack("<H", len(self.armature.bones)))
		else:
			f.write(struct.pack("<H", 0)) # bone count
		f.write(struct.pack("<H", len(self.mesh.textures)))
		f.write(struct.pack("<H", len(self.mesh.texCoordSets)))
		f.write(struct.pack("<H", self.lodMeshCount))
		return True
	
	# write bones
	def writeBones(self, f):
		if self.debugLevel > 0:
			print("saving bones...")
		if self.armature:
			for bone in self.armature.bones:
				realBone = bone.bone
				bone.rigMat = convertMatrixBone(realBone.matrix_local)
				bone.restMat = bone.rigMat
				if realBone.parent:
					bone.restMat = matmul(convertMatrixBone(realBone.parent.matrix_local).inverted(), bone.restMat)
				bone.pos = vector_by_matrix(matmul(self.scalePosition, bone.restMat), Vector((0,0,0)))
				bone.rot = matrixToEuler(bone.restMat)
				# name
				f.write(struct.pack("<B", len(bone.name))) #  len of name
				f.write(bytes(bone.name, 'UTF-8'))
				# position
				f.write(struct.pack("<fff", bone.pos.x, bone.pos.y, bone.pos.z))
				# rotation
				f.write(struct.pack("<fff", bone.rot.x, bone.rot.y, bone.rot.z))
				# parent
				if bone.parent:
					f.write(struct.pack("<H", bone.parent.index + 1))
				else:
					f.write(struct.pack("<H", 0))
				# debug
				"""if self.debugLevel > 1:
					print("- bone %s parent %s pos %s rot %s" % (bone.name, bone.parent, str(bone.pos), str(bone.rot)))"""
		self.progress.advance("Writing bones")
		return True
	
	# write textures
	def writeTextures(self, f):
		if self.debugLevel > 0:
			print("saving textures...")
		for texture in self.mesh.textures:
			"""if self.debugLevel > 1:
				print("- material %s" % (texture.name))"""
			
			# write texture
			f.write(struct.pack("<B", len(texture.exportName))) # length of name
			f.write(bytes(texture.exportName, 'UTF-8')) # name
			
			# build flags
			flags = 0
			if texture.material.dragengine_doublesided:
				flags = flags | 0x1
			if texture.material.dragengine_decal:
				flags = flags | 0x2
			
			# write to file
			f.write(struct.pack("<HH", texture.width, texture.height)) # width, height
			f.write(struct.pack("<H", flags)) # flags
			
			if texture.material.dragengine_decal:
				f.write(struct.pack("<B", texture.material.dragengine_decaloffset)) # decal offset
		self.progress.advance("Writing textures")
		return True
	
	# write texture coordinates sets
	def writeTexCoordSets(self, f):
		if self.debugLevel > 0:
			print("saving texture coordinates sets...")
		for tcs in self.mesh.texCoordSets:
			f.write(struct.pack("<B", len(tcs.name))) # length of name
			f.write(bytes(tcs.name, 'UTF-8')) # name
		self.progress.advance("Writing texture coordinate sets")
		return True
	
	# write mesh
	def writeMesh(self, f, mesh):
		flags = 0
		if mesh.lodError != None:
			flags |= 0x1
		if mesh.largeModel:
			flags |= 0x2
		f.write(struct.pack("<B", flags))
		
		if mesh.lodError != None:
			f.write(struct.pack("<f", mesh.lodError))
		
		f.write(struct.pack(("i" if mesh.largeModel else "H")*6,
			mesh.normalCount, mesh.tangentCount, len(mesh.weights),
			len(mesh.vertices), len(mesh.triangles), len(mesh.quads)))
		return True
	
	# write weights
	def writeWeights(self, f, mesh):
		if self.debugLevel > 0:
			print("saving weights...")
		for weights in mesh.weights:
			f.write(struct.pack("<B", len(weights.weights))) # number of weights
			for weight in weights.weights:
				f.write(struct.pack("<HH", weight.bone.index, int(weight.weight * 1000))) # bone, weight
		self.progress.advance("Writing weights: '{}'".format(mesh.object.name))
		return True
	
	# write vertices
	def writeVertices(self, f, mesh):
		if self.debugLevel > 0:
			print("saving vertices...")
		for vertex in mesh.vertices:
			f.write(struct.pack("<i" if mesh.largeModel else "H", vertex.weights.index + 1))
			# write position using [x,y,z]
			wpos = vector_by_matrix(self.transformScalePosition, vertex.vertex.co)
			f.write(struct.pack("<fff", wpos.x, wpos.y, wpos.z)) # position
			"""if self.debugLevel > 1:
				print("- vertex %i: position=%s" % (vertex.index, str(wpos)))"""
		self.progress.advance("Writing vertices: '{}'".format(mesh.object.name))
		return True
	
	# write texture coordinates
	def writeTexCoords(self, f, mesh):
		if self.debugLevel > 0:
			print("saving texture coordinates...")
		for tcs in mesh.texCoordSets:
			f.write(struct.pack("<i" if mesh.largeModel else "H", len(tcs.texCoords)))
			for texCoord in tcs.texCoords:
				writeTexel(f, texCoord.uv)
		self.progress.advance("Writing texture coordinates: '{}'".format(mesh.object.name))
		return True
	
	# write faces
	def writeFaces(self, f, mesh):
		if self.debugLevel > 0:
			print("saving faces...")
		fmt = "iii" if mesh.largeModel else "HHH"
		for face in mesh.triangles:
			"""if self.debugLevel > 1:
				print("- tri %i: %i %i %i" % (face.face.material_index, face.vertices[0], face.vertices[1], face.vertices[2]))"""
			f.write(struct.pack("<H", face.face.material_index))
			f.write(struct.pack(fmt, face.vertices[2], face.vertices[1], face.vertices[0]))
			f.write(struct.pack(fmt, face.normals[2], face.normals[1], face.normals[0]))
			f.write(struct.pack(fmt, face.tangents[2], face.tangents[1], face.tangents[0]))
			for tcs in face.texCoordSets:
				f.write(struct.pack(fmt, tcs[2].index, tcs[1].index, tcs[0].index))
		
		fmt = "iiii" if mesh.largeModel else "HHHH"
		for face in mesh.quads:
			"""if self.debugLevel > 1:
				print("- quad %i: %i %i %i %i" % (face.face.material_index, face.vertices[3], face.vertices[2], face.vertices[1], face.vertices[0]))"""
			f.write(struct.pack("<H", face.face.material_index))
			f.write(struct.pack(fmt, face.vertices[3], face.vertices[2], face.vertices[1], face.vertices[0]))
			f.write(struct.pack(fmt, face.normals[3], face.normals[2], face.normals[1], face.normals[0]))
			f.write(struct.pack(fmt, face.tangents[3], face.tangents[2], face.tangents[1], face.tangents[0]))
			for tcs in face.texCoordSets:
				f.write(struct.pack(fmt, tcs[3].index, tcs[2].index, tcs[1].index, tcs[0].index))
		
		self.progress.advance("Writing faces: '{}'".format(mesh.object.name))
		return True
registerClass(OBJECT_OT_ExportModel)
