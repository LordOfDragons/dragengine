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
import blf
import bgl
import os
import re
import math
import time
import struct
import mathutils

from bpy_extras.io_utils import ExportHelper
from mathutils import Vector, Matrix

from .de_math import transformPosition, transformBonePosition, convertMatrixBone, ONE_PI
from .de_math import vector_by_matrix, matrixToEuler, vecLength, vecSub, quatDot, quatNegate
from .de_configuration import Configuration
from .de_resources import Mesh, Armature
from .de_porting import registerClass, matmul


class OBJECT_OT_ExportAnimation( bpy.types.Operator, ExportHelper ):
	def exportActionList( self, context ):
		actionFilters = []
		if context.scene.dragengine_movesetidx == -1:
			actionFilters.append( re.compile( "^.+$" ) )
		else:
			for f in context.scene.dragengine_movesets[ context.scene.dragengine_movesetidx ].filters:
				actionFilters.append( re.compile( "^%s$" % f.name ) )
		
		actions = []
		for action in bpy.data.actions.values():
			if len( action.groups ) == 0:
				continue
			if not action.dragengine_export:
				continue
			if action.library: # linked actions can cause lots of troubles
				continue
			
			exportAction = False
			for f in actionFilters:
				if f.match( action.name ):
					exportAction = True
					break
			if not exportAction:
				continue
			
			actions.append( action.name )
		
		return [ ( x, x, x ) for x in actions ]
	
	bl_idname = "dragengine.export_animation"
	bl_label = "Animations (.deanim)"
	__doc__ = """Export as Drag[en]gine Animation Resource"""
	filename_ext = ".deanim"
	filter_glob: bpy.props.StringProperty( default="*.deanim", options={ 'HIDDEN' } )
	debug_level: bpy.props.EnumProperty( items = (
		( '0', "None", "Output no debug messages." ),
		( '1', "Basic", "Output basic amount of debug messages." ),
		( '2', "Verbose", "Output lots of debug messages." ),
		( '3', "Debug", "Output very large amount of debug messages." ),
		), name = "Logging", description = "Choose amount of logging", default = '1' )
	export_mode: bpy.props.EnumProperty( items = (
		( '0', "Export All", "Export all moves. File is completely overwritten." ),
		( '1', "Update Single", "Export single move. File is updated with exported move." ),
		), name = "Export Mode", description = "Export mode", default = '0' )
	#export_move: bpy.props.EnumProperty( items=exportActionList )
	export_move: bpy.props.StringProperty( default="" )
	
	def __init__( self ):
		self.mesh = None
		self.armature = None
		self.moves = []
		self.debugLevel = 1
		self.exportMode = 0
	
	
	
	def draw( self, context ):
		layout = self.layout
		
		box = layout.box()
		column = box.column(align=True)
		column.prop( self, "debug_level", expand=False )
		
		layout.row(align=True).label(text="Move exporting")
		box = layout.box()
		column = box.column(align=True)
		row = column.row(align=True)
		row.prop( self, "export_mode", expand=True )
		
		row = column.row(align=True)
		#row.prop( self, "export_move", expand=False, text="" )
		row.prop_search( self, "export_move", bpy.data, "actions", text="" )
		row.enabled = ( self.export_mode == '1' )
	
	
	
	@classmethod
	def poll( cls, context ):
		return context.active_object != None
	
	def execute( self, context ):
		self.debugLevel = int( self.debug_level )
		self.exportMode = int( self.export_mode )
		if self.exportMode == 1:
			self.exportMove = Armature.Move( bpy.data.actions[ self.export_move ] )
		
		self.report( { 'INFO' }, "export to '%s'..." % ( self.filepath ) )
		
		configuration = Configuration( "deexport.config" )
		if configuration.parseConfig():
			configuration.printConfig()
		
		self.scaling = context.scene.dragengine_scaling
		self.scalePosition = Matrix( ( (self.scaling,0,0,0), (0,self.scaling,0,0), (0,0,self.scaling,0), (0,0,0,1) ) )
		self.transformScalePosition = matmul(self.scalePosition, transformPosition)
		self.transformBoneScalePosition = matmul(self.scalePosition, transformBonePosition)
		
		self.animFPS = context.scene.dragengine_fps
		self.transformTime = 1.0 / float( self.animFPS )
		self.animLimitVarScale = configuration.getValueFor( "animation.limits.variable.scale", 0.01 )
		
		self.actionFilters = []
		if context.scene.dragengine_movesetidx == -1:
			if self.debugLevel > 0:
				print( "animation actions: add filter '.+'" )
			self.actionFilters.append( re.compile( "^.+$" ) )
		else:
			for f in context.scene.dragengine_movesets[ context.scene.dragengine_movesetidx ].filters:
				if self.debugLevel > 0:
					print( "animation actions: add filter '%s'" % f.name )
				self.actionFilters.append( re.compile( "^%s$" % f.name ) )
		
		self.export( context )
		
		print( "*** WARNING ***" )
		print( "animation exporting uses short for positions and rotations. for position this" )
		print( "is multiplied by 1000 causing a maximal precision of 1mm. this can lead to" )
		print( "problems. the code has to be changed to float to fix this. while being at it" )
		print( "adding support for bezier curves would help to store animations at higher" )
		print( "quality as well as reducing file size" )
		
		return { 'FINISHED' }
	
	def export( self, context ):
		self.initExporterObjects( context )
		if not self.checkInitState( context ):
			return False
		
		retainContent = None
		if self.exportMode == 1:
			if os.path.exists( self.filepath ):
				f = open( self.filepath, "rb" )
				try:
					retainContent = self.loadRetainedContent( context, f )
				finally:
					f.close()
			else:
				retainContent = []
		
		f = open( self.filepath, "wb" )
		try:
			result = self.safeExport( context, f, retainContent )
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
			self.armature.ignoreBones = []
			for f in self.armature.armature.dragengine_bonefilters:
				self.armature.ignoreBones.append( re.compile( "^%s$" % f.name ) )
			self.armature.initAddBones()
	
	def checkInitState( self, context ):
		if not self.armature and not self.mesh:
			self.report( { 'INFO', 'ERROR' }, "There is no Armature or Mesh selected. Select at last one Armature or Mesh parented to an armature" )
			return False
		return True
	
	def printInfos( self, context ):
		if self.armature:
			self.armature.printInfos()
	
	def loadRetainedContent( self, context, f ):
		content = {}
		
		# header
		content[ "dataHeader" ] = f.read( 26 )
		if content[ "dataHeader" ][ 0:24 ] != bytes( "Drag[en]gine Animation  ", 'UTF-8' ):
			raise Exception( "Invalid file format" )
		content[ "flags" ] = struct.unpack( "<BB", content[ "dataHeader" ][ 24:26 ] )
		
		# bones
		filepos = f.tell()
		boneCount = struct.unpack( "<B", f.read( 1 ) )[ 0 ]
		if boneCount != len( self.armature.bones ):
			raise Exception( "Bone count mismatch (is {} should {})".format( len( self.armature.bones ), boneCount ) )
		for i in range( boneCount ):
			nameLen = struct.unpack( "<B", f.read( 1 ) )[ 0 ]
			f.read( nameLen )
		datasize = f.tell() - filepos
		f.seek( filepos )
		content[ "dataBones" ] = f.read( datasize )
		
		# moves
		content[ "dataMoves" ] = []
		countMoves = struct.unpack( "<H", f.read( 2 ) )[ 0 ]
		for i in range( countMoves ):
			filepos = f.tell()
			
			nameLen = struct.unpack( "<B", f.read( 1 ) )[ 0 ]
			name = f.read( nameLen ).decode( "utf-8" )
			
			f.read( 4 ) # play time in seconds (float)
			playframes = struct.unpack( "<H", f.read( 2 ) )[ 0 ] # play time in frames
			for j in range( boneCount ):
				moveFlags = struct.unpack( "<B", f.read( 1 ) )[ 0 ]
				
				hasNoBones = ( ( moveFlags & 0x10 ) == 0x10 )
				if hasNoBones:
					continue
				
				hasVarPos = ( ( moveFlags & 0x01 ) == 0x01 )
				hasVarRot = ( ( moveFlags & 0x02 ) == 0x02 )
				hasVarScale = ( ( moveFlags & 0x04 ) == 0x04 )
				hasFewKeyframes = ( ( moveFlags & 0x08 ) == 0x08 )
				shortFormat = ( ( moveFlags & 0x20 ) != 0x20 )
				
				if not hasVarPos and not hasVarRot and not hasVarScale:
					continue
				
				keyframeCount = struct.unpack( "<H", f.read( 2 ) )[ 0 ] if hasFewKeyframes else playframes
				
				dataSize = 2 # keyframe time
				if hasVarPos:
					dataSize = dataSize + ( 6 if shortFormat else 12 ) # struct hhh / fff
				if hasVarRot:
					dataSize = dataSize + ( 6 if shortFormat else 12 ) # struct hhh / fff
				if hasVarScale:
					dataSize = dataSize + ( 6 if shortFormat else 12 ) # struct hhh / fff
				
				f.read( dataSize * keyframeCount )
			
			if name == self.exportMove.name:
				continue
			
			datasize = f.tell() - filepos
			f.seek( filepos )
			content[ "dataMoves" ].append( f.read( datasize ) )
		
		return content
	
	def safeExport( self, context, f, retainContent ):
		factor = 1.0 / 3.0
		
		if not self.prepareBones():
			return False
		
		if not self.scanMoves():
			return False
		
		if not self.writeHeader( f ):
			return False
		
		if retainContent:
			f.write( retainContent[ "dataBones" ] )
			
		else:
			if not self.writeAnimation( f ):
				return False
			
			if not self.writeBones( f ):
				return False
		
		if not self.writeMoves( f, retainContent ):
			return False
		
		if not self.writeTrailer( f ):
			return False
		
		return True
	
	def roundToInt( self, value ):
		return int( value + 0.5 )
	
	# helper function that fills a list with the time
	# positions where knots occure in an ipo curve.
	def buildTimeList( self, fcurve, times ):
		for keyframe in fcurve.keyframe_points:
			timeToCheck = self.roundToInt( keyframe.co.x )
			if not timeToCheck in times:
				times.append( timeToCheck )
		times.sort()
	
	# prepare bones
	def prepareBones( self ):
		if self.debugLevel > 0:
			print( "prepare bones..." )
		
		for bone in self.armature.bones:
			realBone = bone.bone
			bone.rigMat = convertMatrixBone( realBone.matrix_local )
			bone.restMat = bone.rigMat
			if realBone.parent:
				bone.restMat = matmul(convertMatrixBone(realBone.parent.matrix_local).inverted(), bone.restMat)
			bone.pos = vector_by_matrix(matmul(self.scalePosition, bone.restMat), Vector((0,0,0)))
			bone.rot = matrixToEuler( bone.restMat )
		
		return True
	
	# build list of moves to export
	def scanMoves( self ):
		# determine actions to use
		for action in bpy.data.actions.values():
			if len( action.groups ) == 0:
				continue
			if not action.dragengine_export:
				continue
			if action.library: # linked actions can cause lots of troubles
				continue
			
			if self.exportMode == 1 and action != self.exportMove.action:
				continue
			
			exportAction = False
			for f in self.actionFilters:
				if f.match( action.name ):
					exportAction = True
					break
			if not exportAction:
				continue
			
			move = Armature.Move( action )
			if [ x for x in self.moves if x.name == move.name ]:
				self.report( { 'INFO', 'ERROR' }, "Tried to export two moves with duplicate export names ({}).".format(
					", ".join( [ "'{}'".format( x.action.name ) for x in self.moves if x.name == move.name ] ) ) )
				return False
			self.moves.append( Armature.Move( action ) )
		return True
	
	def writeHeader( self, f ):
		f.write( bytes( "Drag[en]gine Animation  ", 'UTF-8' ) )
		f.write( struct.pack( "<BB", 1, 0xf0 ) ) # hack flag until all files are converted
		return True
	
	def writeAnimation( self, f ):
		return True
	
	def writeBones( self, f ):
		if self.debugLevel > 0:
			print( "saving bones..." )
		
		f.write( struct.pack( "<B", len( self.armature.bones ) ) ) # count of bones
		
		for bone in self.armature.bones:
			f.write( struct.pack( "<B", len( bone.name ) ) ) # length name
			f.write( bytes( bone.name, 'UTF-8' ) ) # name
			
			if self.debugLevel > 1:
				print( "- bone", bone.name )
		return True
	
	# fix flipping for example when constraint bones cause negated quaterions
	""" these are no quaterions but euler angles!
	def fixQuaternionFlipping( self, moveBones ):
		for moveBone in moveBones:
			if len( moveBone.keyframes ) < 2:
				continue
			
			for index in range( 1, len( moveBone.keyframes ) ):
				rot1 = moveBone.keyframes[ index - 1 ].rot
				rot2 = moveBone.keyframes[ index ].rot
				if quatDot( rot1, rot2 ) < 0.0:
					moveBone.keyframes[ index ].rot = quatNegate( moveBone.keyframes[ index ].rot )
	"""
	
	# tests if a sequence of keyframes is linear
	def areKeyframesLinear( self, keyframes, firstIndex, lastIndex ):
		kf1 = keyframes[ firstIndex ]
		kf2 = keyframes[ lastIndex ]
		
		# calculate differences
		mulTime = 1.0 / ( kf2.time - kf1.time )
		diffPos = kf2.pos - kf1.pos
		diffRot = kf2.rot - kf1.rot
		diffScale = kf2.scale - kf1.scale
		
		# test all keyframes in between against this differences
		for index in range( firstIndex + 1, lastIndex ):
			kf = keyframes[ index ]
			factor = ( kf.time - kf1.time ) * mulTime
			expectedPos = kf1.pos + diffPos * factor
			expectedRot = kf1.rot + diffRot * factor
			expectedScale = kf1.scale + diffScale * factor
			
			if ( kf.pos - expectedPos ).length > self.armature.thresholdPosition \
			or ( kf.rot - expectedRot ).length > self.armature.thresholdRotation \
			or ( kf.scale - expectedScale ).length > self.armature.thresholdScale:
				return False
		
		return True
	
	# optimize the keyframes by dropping keyframes inside linear changes
	def optimizeKeyframes( self, moveBones ):
		for moveBone in moveBones:
			if len( moveBone.keyframes ) > 0:
				keyframes = [ moveBone.keyframes[ 0 ] ]
				firstIndex = 0
				for index in range( 2, len( moveBone.keyframes ) ):
					if not self.areKeyframesLinear( moveBone.keyframes, firstIndex, index ):
						firstIndex = index -1
						keyframes.append( moveBone.keyframes[ firstIndex ] )
				if firstIndex < len( moveBone.keyframes ) - 1:
					keyframes.append( moveBone.keyframes[ -1 ] )
				moveBone.keyframes = keyframes
	
	# write moves
	def writeMoves( self, f, retainContent ):
		if self.debugLevel > 0:
			print( "saving moves..." )
		
		pose = self.armature.object.pose
		
		# write moves to file
		countMoves = len( self.moves )
		if retainContent:
			countMoves = countMoves + len( retainContent[ "dataMoves" ] )
		f.write( struct.pack( "<H", countMoves ) ) # number of moves
		
		# process moves
		progressCounter = 0
		
		if retainContent:
			for data in retainContent[ "dataMoves" ]:
				f.write( data )
			progressCounter = len( retainContent[ "dataMoves" ] )
		
		for move in self.moves:
			self.report( { 'INFO' }, "%i/%i (%.2f%%): Writing Move %s..." % ( progressCounter, \
				len( self.moves ), float( progressCounter ) / float( countMoves ), move.name ) )
			"""bgl.glColor3f( 0.34, 0.50, 0.76 )
			blf.position( 0, 0, 0, 0 )
			blf.draw( 0, move.name )"""
			#Blender.Window.DrawProgressBar( 0.2 + progressCounter * 0.8 / countMoves, "Writing Move %s..." % move.name )
			playtime = 0
			moveBones = []
			agroups = move.action.groups
			
			# determine the time scaling
			self.timeScale = self.transformTime * move.action.dragengine_timescaling
			
			# determine keyframe time locations for all bones
			validDPParams = []
			validDPParams.append( "location" )
			validDPParams.append( "rotation_quaternion" )
			validDPParams.append( "rotation_euler" )
			validDPParams.append( "scale" )
			
			for bone in self.armature.bones:
				moveBone = Armature.MoveBone( bone )
				moveBone.times.append( 0 )
				if bone.name in agroups:
					boneAGroup = agroups[ bone.name ]
					moveBone.used = True
					for fcurve in boneAGroup.channels:
						dpParams = fcurve.data_path.rsplit( ".", 1 )
						if len( dpParams ) == 2 and dpParams[ 1 ] in validDPParams:
							self.buildTimeList( fcurve, moveBone.times )
				timeCount = len( moveBone.times )
				if timeCount > 0:
					bonePlayTime = moveBone.times[ -1 ]
					if bonePlayTime > playtime:
						playtime = bonePlayTime
				moveBones.append( moveBone )
			
			# determine the frames to export. this can be altered by properties
			if move.automaticRange:
				move.firstFrame = 0
				move.lastFrame = playtime
				
			else:
				playtime = move.lastFrame - move.firstFrame
			
			# write move informations
			f.write( struct.pack( "<B", len( move.name ) ) ) # length name
			f.write( bytes( move.name, 'UTF-8' ) ) # name
			
			#f.write( struct.pack( "<B", 1.0 / self.timeScale ) ) # animFPS ) ) # frames per second ( bugged due to rounding errors )
			f.write( struct.pack( "<f", float( playtime ) * self.timeScale ) ) # play time in seconds
			f.write( struct.pack( "<H", playtime ) ) # play time in frames
			
			# switch action
			self.armature.object.animation_data.action = move.action
			
			# blender fails to properly update bone states after changing the action. to fix this bug
			# the scene frame is first set to 1 after the initial frame to save. this way the frame
			# has to change for the first written frame forcing an update in all cases
			bpy.context.scene.frame_set( move.firstFrame + 1 )
			
			# fetch keyframe values
			for time in range( move.firstFrame, move.lastFrame + 1 ):
				bpy.context.scene.frame_set( time )
				
				# retrieve keyframe data for all bones having one. this will be changed since constraints are now
				# part of the show. currently all keyframes are saved which is a waste. later on only keyrames
				# will be saved if a change occured.
				for moveBone in moveBones:
					if moveBone.used:
						# determine if a keyframe is required
						requiresKeyframe = True
						
						# add a keyframe if required
						if requiresKeyframe: #time in moveBone.times:
							bone = moveBone.bone
							poseBone = pose.bones[ bone.bone.name ]
							refMat = bone.restMat
							keyframes = moveBone.keyframes
							
							restMat = convertMatrixBone( poseBone.matrix )
							if poseBone.parent:
								restMat = matmul(matmul(convertMatrixBone(poseBone.parent.matrix), refMat).inverted(), restMat)
							else:
								restMat = matmul(refMat.inverted(), restMat)
							pos = vector_by_matrix(matmul(self.scalePosition, restMat), Vector((0,0,0)))
							rot = matrixToEuler( restMat )
							scale = restMat.decompose()[ 2 ]
							
							moveBone.keyframes.append( Armature.Keyframe( time - move.firstFrame, pos, rot, scale ) )
			
			# fix flipping for example when constraint bones cause negated quaterions
			#self.fixQuaternionFlipping( moveBones )
			
			# optimize the keyframes by dropping keyframes inside linear changes
			#for DUMMY in moveBone.keyframes: print( DUMMY )
			self.optimizeKeyframes( moveBones )
			
			if self.debugLevel > 0:
				print( "- move", move.name, "playtime", float( playtime ) * self.timeScale )
			
			# write bones
			for moveBone in moveBones:
				# the bone is stored if there are keyframes
				if len( moveBone.keyframes ) > 0:
					hasVarPos = False
					hasVarRot = False
					hasVarScale = False
					
					for keyframe in moveBone.keyframes:
						if keyframe.pos.length > self.armature.thresholdPosition:
							hasVarPos = True
						if keyframe.rot.length > self.armature.thresholdRotation:
							hasVarRot = True
						if ( keyframe.scale - Vector( ( 1.0, 1.0, 1.0 ) ) ).length > self.armature.thresholdScale:
							hasVarScale = True
					
					hasFewKeyframes = True #( len( moveBone.keyframes ) / playtime < 0.25 )
					
					# flags
					flags = 0
					if hasVarPos:
						flags = flags | 0x01
					if hasVarRot:
						flags = flags | 0x02
					if hasVarScale:
						flags = flags | 0x04
					if hasFewKeyframes:
						flags = flags | 0x08
					if not self.armature.shortFormat:
						flags = flags | 0x20
					
					f.write( struct.pack( "<B", flags ) )
					
					# only store the keyframes if we have a variable position, rotation or scaling
					if hasVarPos or hasVarRot or hasVarScale:
						# number of keyframes if we have only a few
						if hasFewKeyframes:
							f.write( struct.pack( "<H", len( moveBone.keyframes ) ) )
						
						# write keyframes
						for keyframe in moveBone.keyframes:
							# if we have few keyframes write the time in frames
							f.write( struct.pack( "<H", keyframe.time ) )
							
							# if we have a variable position write it. if short format is used encode as signed 16-bit.
							if hasVarPos:
								if self.armature.shortFormat:
									f.write( struct.pack( "<hhh", int( keyframe.pos.x * 1000.0 ), \
										int( keyframe.pos.y * 1000.0 ), int( keyframe.pos.z * 1000.0 ) ) )
								else:
									f.write( struct.pack( "<fff", keyframe.pos.x, keyframe.pos.y, keyframe.pos.z ) )
							
							# if we have a variable rotation write it. if short format is used encode as signed 16-bit.
							if hasVarRot:
								if self.armature.shortFormat:
									f.write( struct.pack( "<hhh", int( keyframe.rot.x * 100.0 ), \
										int( keyframe.rot.y * 100.0 ), int( keyframe.rot.z * 100.0 ) ) )
								else:
									f.write( struct.pack( "<fff", keyframe.rot.x * ONE_PI, keyframe.rot.y * ONE_PI, keyframe.rot.z * ONE_PI ) )
							
							# if we have a variable scale write it. if short format is used encode as signed 16-bit.
							if hasVarScale:
								if self.armature.shortFormat:
									f.write( struct.pack( "<hhh", int( keyframe.scale.x * 1000.0 ), \
										int( keyframe.scale.y * 1000.0 ), int( keyframe.scale.z * 1000.0 ) ) )
								else:
									f.write( struct.pack( "<fff", keyframe.scale.x, keyframe.scale.y, keyframe.scale.z ) )
							
							if self.debugLevel > 1:
								print( "- bone", moveBone.bone.name, "keyframe", float( keyframe.time ) * self.timeScale, "pos", keyframe.pos, "rot", keyframe.rot, "scale", keyframe.scale )
					
					#if self.debugLevel > 0: print( "- bone", moveBone.bone.name, "hasVarPos", hasVarPos, "hasVarRot", hasVarRot, "hasVarScale", hasVarScale, "keyframes", len( moveBone.keyframes ) )
				
				# otherwise the bone is ignored
				else:
					f.write( struct.pack( "<B", 0x10 ) )
					#if self.debugLevel > 0: print( "- bone", moveBone.bone.name, "ignored", "keyframes", len( moveBone.keyframes ) )
			
			# next round
			progressCounter = progressCounter + 1
		return True
	
	# write trailer
	def writeTrailer( self, f ):
		return True

registerClass(OBJECT_OT_ExportAnimation)
