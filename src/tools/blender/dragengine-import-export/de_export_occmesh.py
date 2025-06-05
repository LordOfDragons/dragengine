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
import os
import re
import math
import time
import struct
import mathutils

from bpy_extras.io_utils import ExportHelper
from mathutils import Vector, Matrix

from .de_math import transformPosition, convertMatrix, convertMatrixBone, vector_by_matrix, matrixToQuaternion, writeTexel
from .de_configuration import Configuration
from .de_resources import Mesh, Armature
from .de_porting import registerClass, matmul



class OBJECT_OT_ExportOcclusionMesh( bpy.types.Operator, ExportHelper ):
	bl_idname = "dragengine.export_occmesh"
	bl_label = "Occlusion Mesh (.deoccmesh)"
	bl_label_button = "Occlusion Mesh"
	bl_icon = 'GHOST_ENABLED'
	__doc__ = """Export as Drag[en]gine Occlusion Mesh Resource"""
	filename_ext = ".deoccmesh"
	filter_glob: bpy.props.StringProperty( default="*.deoccmesh", options={ 'HIDDEN' } )
	debug_level: bpy.props.EnumProperty( items = (
		( '0', "None", "Output no debug messages." ),
		( '1', "Basic", "Output basic amount of debug messages." ),
		( '2', "Verbose", "Output lots of debug messages." ),
		( '3', "Debug", "Output very large amount of debug messages." ),
		), name = "Logging", description = "Choose amount of logging", default = '1' )
	
	def __init__(self, *args, **kwargs):
		super().__init__(*args, **kwargs)
		self.mesh = None
		self.armature = None
		self.debugLevel = 1
	
	@classmethod
	def poll( cls, context ):
		return context.active_object != None
	
	def execute( self, context ):
		self.debugLevel = int( self.debug_level )
		self.report( { 'INFO' }, "export to '%s'..." % ( self.filepath ) )
		
		configuration = Configuration( "deexport.config" )
		if configuration.parseConfig():
			configuration.printConfig()
		
		self.scaling = context.scene.dragengine_scaling
		self.scalePosition = Matrix( ( (self.scaling,0,0,0), (0,self.scaling,0,0), (0,0,self.scaling,0), (0,0,0,1) ) )
		self.transformScalePosition = matmul(self.scalePosition, transformPosition)
		
		self.ignoreBones = []
		for f in configuration.getArrayFor( "animation.bones.ignore", [] ):
			self.ignoreBones.append( re.compile( f[ 0 ] ) )
		
		self.export( context )
		
		return { 'FINISHED' }
	
	def export( self, context ):
		self.initExporterObjects( context )
		if not self.checkInitState( context ):
			return False
		f = open( self.filepath, "wb" )
		try:
			result = self.safeExport( context, f )
		finally:
			f.close()
		self.printInfos( context )
		return result
	
	def initFindMeshArmRef( self, context ):
		for object in context.selected_objects:
			if object.type == 'MESH':
				if not self.mesh:
					self.mesh = Mesh( object )
					if object.parent and object.parent.type == 'ARMATURE':
						if not self.armature:
							self.armature = Armature( object.parent )
			elif object.type == 'ARMATURE':
				if not self.armature:
					self.armature = Armature( object )
		if self.mesh and self.armature:
			self.mesh.armature = self.armature
	
	def initExporterObjects( self, context ):
		self.initFindMeshArmRef( context )
		if self.armature:
			self.armature.ignoreBones = self.ignoreBones
			self.armature.initAddBones()
		if self.mesh:
			self.mesh.initAddTextures()
			self.mesh.initAddVertices()
			self.mesh.initAddEdges()
			self.mesh.initAddFaces()
			self.mesh.initCalcInfoNumbers()
			self.cornerCount = 0
			self.doubleSidedFaceCount = 0
			for face in self.mesh.faces:
				self.cornerCount = self.cornerCount + len( face.vertices )
				if face.texture and face.texture.material.dragengine_doublesided:
					self.doubleSidedFaceCount = self.doubleSidedFaceCount + 1
	
	def checkInitState( self, context ):
		if not self.mesh:
			self.report( { 'INFO', 'ERROR' }, "There is no Mesh selected. Select at last a Mesh and optional an Armature" )
			return False
		if self.mesh.degeneratedFaces:
			self.report( { 'INFO', 'ERROR' }, "Degenerated Faces found." )
			return False
		return True
	
	def printInfos( self, context ):
		if self.mesh:
			self.mesh.printInfos()
		if self.armature:
			self.armature.printInfos()
	
	def safeExport( self, context, f ):
		if not self.writeHeader( f ):
			return False
		if not self.writeBones( f ):
			return False
		if not self.writeWeights( f ):
			return False
		if not self.writeVertices( f ):
			return False
		if not self.writeFaces( f ):
			return False
		return True
	
	# write header to file
	def writeHeader( self, f ):
		f.write( bytes( "Drag[en]gine Occlusion Mesh", 'UTF-8' ) )
		f.write( struct.pack( "<HH", 1, 0 ) ) # version, flags
		if self.armature:
			f.write( struct.pack( "<H", len( self.armature.bones ) ) ) # bone count
		else:
			f.write( struct.pack( "<H", 0 ) ) # bone count
		f.write( struct.pack( "<H", len( self.mesh.weights ) ) ) # weights count
		f.write( struct.pack( "<H", len( self.mesh.vertices ) ) ) # vertex count
		f.write( struct.pack( "<H", self.cornerCount ) ) # corner count
		f.write( struct.pack( "<H", len( self.mesh.faces ) ) ) # face count
		f.write( struct.pack( "<H", self.doubleSidedFaceCount ) ) # double sided face count
		return True
	
	# write bones
	def writeBones( self, f ):
		if self.debugLevel > 0:
			print( "saving bones..." )
		if self.armature:
			for bone in self.armature.bones:
				realBone = bone.bone
				if realBone.parent:
					bone.restMat = matmul(convertMatrixBone(realBone.parent.matrix_local).inverted(),
						convertMatrixBone(realBone.matrix_local))
				else:
					bone.restMat = convertMatrix( realBone.matrix_local )
				bone.position = vector_by_matrix(matmul(self.scalePosition, bone.restMat), Vector((0,0,0)))
				bone.orientation = matrixToQuaternion( bone.restMat )
				# name
				f.write( struct.pack( "<B", len( bone.name ) ) ) #  len of name
				f.write( bytes( bone.name, 'UTF-8' ) )
				# position
				f.write( struct.pack( "<fff", bone.position.x, bone.position.y, bone.position.z ) )
				# orientation
				f.write( struct.pack( "<ffff", bone.orientation.x, bone.orientation.y, bone.orientation.z, bone.orientation.w ) )
				# parent
				if bone.parent:
					f.write( struct.pack( "<H", bone.parent.index + 1 ) )
				else:
					f.write( struct.pack( "<H", 0 ) )
				# debug
				if self.debugLevel > 1:
					print( "- bone %s parent %s position %s orientation %s" % ( bone.name, bone.parent, str( bone.position ), str( bone.orientation ) ) )
		return True
	
	# write weights
	def writeWeights( self, f ):
		if self.debugLevel > 0:
			print( "saving weights..." )
		for weights in self.mesh.weights:
			f.write( struct.pack( "<B", len( weights.weights ) ) ) # number of weights
			for weight in weights.weights:
				f.write( struct.pack( "<HH", weight.bone.index, int( weight.weight * 1000.0 ) ) ) # bone, weight
		return True
	
	# write vertices
	def writeVertices( self, f ):
		if self.debugLevel > 0:
			print( "saving vertices..." )
		for vertex in self.mesh.vertices:
			f.write( struct.pack( "<H", vertex.weights.index + 1 ) ) # weights index
			# write position using [x,y,z]
			wpos = vector_by_matrix( self.transformScalePosition, vertex.vertex.co )
			f.write( struct.pack( "<fff", wpos.x, wpos.y, wpos.z ) ) # position
			if self.debugLevel > 1:
				print( "- vertex %i: position=%s" % ( vertex.index, str( wpos ) ) )
		return True
	
	# write faces
	def writeFaces( self, f ):
		if self.debugLevel > 0:
			print( "saving faces..." )
		# single sided faces
		for face in self.mesh.faces:
			if not face.texture or not face.texture.material.dragengine_doublesided:
				cornerCount = len( face.vertices )
				f.write( struct.pack( "<B", cornerCount ) )
				for c in range( cornerCount - 1, -1, -1 ):
					f.write( struct.pack( "<H", face.vertices[ c ] ) )
		# double sided faces
		for face in self.mesh.faces:
			if face.texture and face.texture.material.dragengine_doublesided:
				cornerCount = len( face.vertices )
				f.write( struct.pack( "<B", cornerCount ) )
				for c in range( cornerCount - 1, -1, -1 ):
					f.write( struct.pack( "<H", face.vertices[ c ] ) )
		return True
registerClass(OBJECT_OT_ExportOcclusionMesh)
