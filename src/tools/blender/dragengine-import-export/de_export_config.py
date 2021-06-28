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
from .de_porting import registerClass



class OBJECT_OT_ExportConfig( bpy.types.Operator, ExportHelper ):
	bl_idname = "dragengine.export_config"
	bl_label = "Configuration (.txt)"
	__doc__ = """Export configuration as text file"""
	filename_ext = ".txt"
	filter_glob: bpy.props.StringProperty( default="*.txt", options={ 'HIDDEN' } )
	
	@classmethod
	def poll( cls, context ):
		return True
	
	def writeGroup( self, f, typeName, objectName ):
		f.write( "%s %s\n" % ( typeName, objectName ) )
	
	def writeProperty( self, f, object, propertyName ):
		if object.is_property_set( propertyName ):
			propertyValue = object.get( propertyName )
			if type( propertyValue ) is str:
				f.write( "\tstring %s %s\n" % ( propertyName, propertyValue ) )
			elif type( propertyValue ) is float:
				f.write( "\tfloat %s %g\n" % ( propertyName, propertyValue ) )
			elif type( propertyValue ) is int:
				f.write( "\tint %s %i\n" % ( propertyName, propertyValue ) )
			elif type( propertyValue ) is bool:
				f.write( "\tbool %s %s\n" % ( propertyName, 'true' if value else 'false' ) )
			elif type( propertyValue ) is list:
				f.write( "\tlist %s\n" % propertyName )
				for entry in propertyValue:
					self.writePropertyDictionary( f, 2, entry.to_dict() )
			else:
				raise Exception( 'unknown property type' )
	
	def writePropertyDictionary( self, f, indent, dictionary ):
		indentStr = ''.join( [ "\t" ] * indent )
		for key, value in dictionary.items():
			if type( value ) is str:
				f.write( "%sstring %s %s\n" % ( indentStr, key, value ) )
			elif type( value ) is float:
				f.write( "%sfloat %s %g\n" % ( indentStr, key, value ) )
			elif type( value ) is int:
				f.write( "%sint %s %i\n" % ( indentStr, key, value ) )
			elif type( value ) is bool:
				f.write( "%sbool %s %s\n" % ( indentStr, key, 'true' if value else 'false' ) )
			elif type( value ) is list:
				f.write( "%slist %s\n" % ( indentStr, key ) )
				for entry in value:
					self.writePropertyDictionary( f, indent + 1, entry )
			else:
				raise Exception( 'unknown property type' )
	
	def writeEmptyLine( self, f ):
		f.write( "\n" )
	
	def getSetPropertyList( self, object ):
		return [ prop[ 0 ] for prop in object.items() if prop[ 0 ][ 0:11 ] == 'dragengine_' and object.is_property_set( prop[ 0 ] ) ]
	
	def execute( self, context ):
		propertiesScene = [ "dragengine_scaling", "dragengine_fps", "dragengine_movesets" ]
		propertiesMesh = [ "dragengine_scaling", "dragengine_lodmesh", "dragengine_physics" ]
		propertiesMaterial = [ "dragengine_exportname", "dragengine_doublesided" ]
		propertiesAction = [ "dragengine_export", "dragengine_exportname", "dragengine_timescaling" ]
		propertiesArmature = [ "dragengine_bonefilters" ]
		
		f = open( self.filepath, "w" )
		try:
			for scene in bpy.data.scenes:
				propertyNameList = self.getSetPropertyList( scene )
				if len( propertyNameList ) > 0:
					self.writeGroup( f, "scene", scene.name )
					for propertyName in propertyNameList:
						self.writeProperty( f, scene, propertyName )
					self.writeEmptyLine( f )
			
			for object in bpy.data.objects:
				if object.type == 'MESH':
					propertyNameList = self.getSetPropertyList( object )
					if len( propertyNameList ) > 0:
						self.writeGroup( f, "object", object.name )
						for propertyName in propertyNameList:
							self.writeProperty( f, object, propertyName )
						self.writeEmptyLine( f )
			
			for material in bpy.data.materials:
				propertyNameList = self.getSetPropertyList( material )
				if len( propertyNameList ) > 0:
					self.writeGroup( f, "material", material.name )
					for propertyName in propertyNameList:
						self.writeProperty( f, material, propertyName )
					self.writeEmptyLine( f )
			
			for action in bpy.data.actions:
				propertyNameList = self.getSetPropertyList( action )
				if len( propertyNameList ) > 0:
					self.writeGroup( f, "action", action.name )
					for propertyName in propertyNameList:
						self.writeProperty( f, action, propertyName )
					self.writeEmptyLine( f )
			
			for armature in bpy.data.armatures:
				propertyNameList = self.getSetPropertyList( armature )
				if len( propertyNameList ) > 0:
					self.writeGroup( f, "armature", armature.name )
					for propertyName in propertyNameList:
						self.writeProperty( f, armature, propertyName )
					self.writeEmptyLine( f )
			
		finally:
			f.close()
		
		return { 'FINISHED' }
registerClass(OBJECT_OT_ExportConfig)
