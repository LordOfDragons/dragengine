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

from .de_math import transformPosition, vector_by_matrix
from .de_resources import Mesh
from .de_porting import registerClass, matmul



class OBJECT_OT_ExportNavigationSpace( bpy.types.Operator, ExportHelper ):
	bl_idname = "dragengine.export_navspace"
	bl_label = "Navigation Space (.denavspace))"
	bl_label_button = "Navigation Space"
	bl_icon = 'ARROW_LEFTRIGHT'
	__doc__ = """Export as Drag[en]gine Navigation Space"""
	filename_ext = ".denavspace"
	filter_glob: bpy.props.StringProperty( default="*.denavspace", options={ 'HIDDEN' } )
	debug_level: bpy.props.EnumProperty( items = (
		( '0', "None", "Output no debug messages." ),
		( '1', "Basic", "Output basic amount of debug messages." ),
		( '2', "Verbose", "Output lots of debug messages." ),
		( '3', "Debug", "Output very large amount of debug messages." ),
		), name = "Logging", description = "Choose amount of logging", default = '1' )
	
	def __init__( self ):
		self.mesh = None
		self.debugLevel = 1
	
	@classmethod
	def poll( cls, context ):
		return context.active_object != None
	
	def execute( self, context ):
		self.debugLevel = int( self.debug_level )
		self.report( { 'INFO' }, "export to '%s'..." % ( self.filepath ) )
		
		self.scaling = context.scene.dragengine_scaling
		self.scalePosition = Matrix( ( (self.scaling,0,0,0), (0,self.scaling,0,0), (0,0,self.scaling,0), (0,0,0,1) ) )
		self.transformScalePosition = matmul(self.scalePosition, transformPosition)
		
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
	
	def initExporterObjects( self, context ):
		self.initFindMeshArmRef( context )
		if self.mesh:
			self.mesh.initAddTextures()
			self.mesh.initAddVertices()
			self.mesh.initAddEdges()
			self.mesh.initAddFaces()
			self.mesh.initNavType()
			self.mesh.initCalcInfoNumbers()
			self.cornerCount = 0
			self.doubleSidedFaceCount = 0
			for face in self.mesh.faces:
				self.cornerCount = self.cornerCount + len( face.vertices )
				if face.texture and face.texture.material.dragengine_doublesided:
					self.doubleSidedFaceCount = self.doubleSidedFaceCount + 1
	
	def checkInitState( self, context ):
		if not self.mesh:
			self.report( { 'INFO', 'ERROR' }, "There is no Mesh selected" )
			return False
		if self.mesh.degeneratedFaces:
			self.report( { 'INFO', 'ERROR' }, "Degenerated Faces found." )
			return False
		return True
	
	def printInfos( self, context ):
		if self.mesh:
			self.mesh.printInfos()
	
	def safeExport( self, context, f ):
		if not self.writeHeader( f ):
			return False
		if not self.writeVertices( f ):
			return False
		if not self.writeEdges( f ):
			return False
		if not self.writeCorners( f ):
			return False
		if not self.writeFaces( f ):
			return False
		return True
	
	# write header to file
	def writeHeader( self, f ):
		f.write( bytes( "Drag[en]gine Navigation Space", 'UTF-8' ) )
		f.write( struct.pack( "<HH", 1, 0 ) ) # version, flags
		
		# vertex count
		f.write( struct.pack( "<H", len( self.mesh.vertices ) ) )
		
		# edge count
		if self.mesh.navspacetype == 'GRID':
			f.write( struct.pack( "<H", len( self.mesh.edges ) ) )
		else:
			f.write( struct.pack( "<H", 0 ) )
		
		# corner count
		if self.mesh.navspacetype == 'MESH':
			f.write( struct.pack( "<H", self.cornerCount ) )
		else:
			f.write( struct.pack( "<H", 0 ) )
		
		# face count
		f.write( struct.pack( "<H", len( self.mesh.faces ) ) ) # face count
		
		# wall count, room count
		if self.mesh.navspacetype == 'VOLUME':
			f.write( struct.pack( "<H", 0 ) )
			f.write( struct.pack( "<H", 0 ) )
		else:
			f.write( struct.pack( "<H", 0 ) )
			f.write( struct.pack( "<H", 0 ) )
		
		# type
		if self.mesh.navspacetype == 'GRID':
			f.write( struct.pack( "<H", 0 ) )
		elif self.mesh.navspacetype == 'MESH':
			f.write( struct.pack( "<H", 1 ) )
		else: # 'VOLUME'
			f.write( struct.pack( "<H", 2 ) )
		
		return True
	
	def writeVertices( self, f ):
		if self.debugLevel > 0:
			print( 'saving vertices...' )
		for vertex in self.mesh.vertices:
			wpos = vector_by_matrix( self.transformScalePosition, vertex.vertex.co )
			# position[x,y,z]
			f.write( struct.pack( "<fff", wpos.x, wpos.y, wpos.z ) )
		return True
	
	def writeEdges( self, f ):
		if self.mesh.navspacetype == 'GRID':
			if self.debugLevel > 0:
				print( 'saving edges...' )
			for edge in self.mesh.edges:
				# vertex1, vertex2
				f.write( struct.pack( "<HH", edge.vertices[ 0 ].index, edge.vertices[ 1 ].index ) )
				# type1, type2
				f.write( struct.pack( "<HH", edge.navtype, edge.navtype ) )
		return True
	
	def writeCorners( self, f ):
		if self.mesh.navspacetype == 'MESH':
			if self.debugLevel > 0:
				print( 'saving corners...' )
			for face in self.mesh.faces:
				for i in reversed( range( len( face.vertices ) ) ):
					vertex = face.vertices[ i ]
					edge = self.mesh.edges[ face.edges[ i ] ]
					# vertex, type
					f.write( struct.pack( "<HH", vertex, edge.navtype ) )
		return True
	
	def writeFaces( self, f ):
		if self.mesh.navspacetype == 'MESH':
			if self.debugLevel > 0:
				print( 'saving faces...' )
			for face in self.mesh.faces:
				navtype = 0
				if face.texture:
					navtype = face.texture.navtype
				# cornerCount, type
				f.write( struct.pack( "<HH", len( face.vertices ), navtype ) )
		return True
registerClass(OBJECT_OT_ExportNavigationSpace)
