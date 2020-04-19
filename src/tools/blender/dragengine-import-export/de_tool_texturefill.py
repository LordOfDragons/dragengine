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

from .de_porting import registerClass



# Texture Fill
################

class OBJECT_OT_ToolTextureFill( bpy.types.Operator ):
	bl_idname = "dragengine.texturefill"
	bl_label = "Texture Fill"
	__doc__ = """Texture Fill"""
	
	class Tile:
		def __init__( self, x, y, sizex, sizey ):
			self.x = x
			self.y = y
			self.sizex = sizex
			self.sizey = sizey
			self.pixelCount = sizex * sizey
			self.remainingPixelCount = 0
			self.sleeping = True
			self.noNeighborWakeUpYet = True
			self.tileLeft = None
			self.tileTop = None
			self.tileRight = None
			self.tileBottom = None
			
		def wakeUp( self ):
			self.sleeping = False
			if self.noNeighborWakeUpYet:
				if self.tileLeft:
					self.tileLeft.sleeping = False
				if self.tileTop:
					self.tileTop.sleeping = False
					if self.tileTop.tileLeft:
						self.tileTop.tileLeft.sleeping = False
					if self.tileTop.tileRight:
						self.tileTop.tileRight.sleeping = False
				if self.tileRight:
					self.tileRight.sleeping = False
				if self.tileBottom:
					self.tileBottom.sleeping = False
					if self.tileBottom.tileLeft:
						self.tileBottom.tileLeft.sleeping = False
					if self.tileBottom.tileRight:
						self.tileBottom.tileRight.sleeping = False
			self.noNeighborWakeUpYet = False
	
	
	@classmethod
	def poll( cls, context ):
		return context.area.type == 'IMAGE_EDITOR' \
			and context.space_data.image \
			and context.space_data.image.depth == 32 \
			and context.space_data.image.size[0] > 0 \
			and context.space_data.image.size[1] > 0
	
	def execute( self, context ):
		image = context.space_data.image
		width, height = image.size
		pixelCount = width * height
		pcompCount = pixelCount * 4
		pcompStride = width * 4
		
		timeStart = time.time()
		pixels = [ x for x in image.pixels ]
		print( "create working copy of {0}x{1} image in {2:.3f}s".format( width, height, time.time() - timeStart ) )
		
		# mark all transparent pixels with a 0 and all others with 1
		timeStart = time.time()
		for i in range( 3, pcompCount, 4 ):
			pixels[ i ] = 0 if pixels[ i ] < 0.5 else 1
		print( "prepare image in {0:.3f}s".format( time.time() - timeStart ) )
		
		# build list of tiles to speed up the process. the total number of tiles is limited to avoid trashing
		# memory
		timeStart = time.time()
		tileCountLimit = 128 * 128
		tileSizeX = 2
		tileSizeY = 2
		while True:
			tileCountX = int( ( width - 1 ) / tileSizeX ) + 1
			tileCountY = int( ( height - 1 ) / tileSizeY ) + 1
			if tileCountX * tileCountY <= tileCountLimit:
				break
			if tileSizeX > tileSizeY:
				tileSizeY = tileSizeY + 1
			else:
				tileSizeX = tileSizeX + 1
		
		tiles = []
		print( "tileCount={0}({1}x{2}) tileSize={3}x{4} ".format( tileCountX * tileCountY, tileCountX, tileCountY, tileSizeX, tileSizeY ) )
		
		for y in range( 0, height, tileSizeY ):
			useTileSizeY = tileSizeY
			if y + useTileSizeY > height:
				useTileSizeY = height - y
			for x in range( 0, width, tileSizeX ):
				useTileSizeX = tileSizeX
				if x + useTileSizeX > width:
					useTileSizeX = width - x
				tiles.append( OBJECT_OT_ToolTextureFill.Tile( x, y, useTileSizeX, useTileSizeY ) )
		
		# link tiles
		for y in range( tileCountY ):
			baseOffset = tileCountX * y
			canUp = ( y > 0 )
			canDown = ( y < tileCountY - 1 )
			for x in range( tileCountX ):
				offset = baseOffset + x
				tile = tiles[ offset ]
				if canUp:
					tile.tileTop = tiles[ offset - tileCountX ]
				if canDown:
					tile.tileBottom = tiles[ offset + tileCountX ]
				if x > 0:
					tile.tileLeft = tiles[ offset - 1 ]
				if x < tileCountX - 1:
					tile.tileRight = tiles[ offset + 1 ]
		
		# prepare the counter of remaining pixels for all tiles and the image
		remainingPixelCount = 0
		for tile in tiles:
			for y in range( tile.y, tile.y + tile.sizey ):
				baseOffset = pcompStride * y
				for x in range( tile.x, tile.x + tile.sizex ):
					offset = baseOffset + x * 4
					if pixels[ offset + 3 ] == 0:
						tile.remainingPixelCount = tile.remainingPixelCount + 1
			remainingPixelCount = remainingPixelCount + tile.remainingPixelCount
			if tile.remainingPixelCount < tile.pixelCount:
				tile.wakeUp()
		
		# remove tiles with no more remaining pixels left
		tiles = [ tile for tile in tiles if tile.remainingPixelCount > 0 ]
		print( "build tiles in {0:.3f}s".format( time.time() - timeStart ) )
		
		# process image until there are no more pixels left
		print( "start processing: remainingPixelCount={0} tiles={1}(sleeping={2} active={3})".format( \
			remainingPixelCount, len( tiles ), sum( 1 for tile in tiles if tile.sleeping ), \
			sum( 1 for tile in tiles if not tile.sleeping ) ) )
		timeTotalStart = time.time()
		leftCheck = width - 1
		curMark = 2.0
		while remainingPixelCount > 0:
			timeStart = time.time()
			
			for tile in tiles:
				if not tile.sleeping:
					oldPixelCount = tile.remainingPixelCount
					
					for y in range( tile.y, tile.y + tile.sizey ):
						baseOffset = pcompStride * y
						canUp = ( y > 0 )
						canDown = ( y < height - 1 )
						
						for x in range( tile.x, tile.x + tile.sizex ):
							offset = baseOffset + x * 4
							
							# work only with pixels not marked yet
							if pixels[ offset + 3 ] == 0:
								# average the marked pixels from a 9-pixel neighborhood
								averageRed = 0.0
								averageGreen = 0.0
								averageBlue = 0.0
								averageCount = 0
								
								canLeft = ( x > 0 )
								canRight = ( x < leftCheck )
								
								if canUp and canLeft:
									otherIndex = offset - pcompStride - 4
									if pixels[ otherIndex + 3 ] > 0.0 and pixels[ otherIndex + 3 ] < curMark:
										averageRed = averageRed + pixels[ otherIndex ]
										averageGreen = averageGreen + pixels[ otherIndex + 1 ]
										averageBlue = averageBlue + pixels[ otherIndex + 2 ]
										averageCount = averageCount + 1
								
								if canUp:
									otherIndex = offset - pcompStride
									if pixels[ otherIndex + 3 ] > 0.0 and pixels[ otherIndex + 3 ] < curMark:
										averageRed = averageRed + pixels[ otherIndex ]
										averageGreen = averageGreen + pixels[ otherIndex + 1 ]
										averageBlue = averageBlue + pixels[ otherIndex + 2 ]
										averageCount = averageCount + 1
								
								if canUp and canRight:
									otherIndex = offset - pcompStride + 4
									if pixels[ otherIndex + 3 ] > 0.0 and pixels[ otherIndex + 3 ] < curMark:
										averageRed = averageRed + pixels[ otherIndex ]
										averageGreen = averageGreen + pixels[ otherIndex + 1 ]
										averageBlue = averageBlue + pixels[ otherIndex + 2 ]
										averageCount = averageCount + 1
								
								if canLeft:
									otherIndex = offset - 4
									if pixels[ otherIndex + 3 ] > 0.0 and pixels[ otherIndex + 3 ] < curMark:
										averageRed = averageRed + pixels[ otherIndex ]
										averageGreen = averageGreen + pixels[ otherIndex + 1 ]
										averageBlue = averageBlue + pixels[ otherIndex + 2 ]
										averageCount = averageCount + 1
								
								if canRight:
									otherIndex = offset + 4
									if pixels[ otherIndex + 3 ] > 0.0 and pixels[ otherIndex + 3 ] < curMark:
										averageRed = averageRed + pixels[ otherIndex ]
										averageGreen = averageGreen + pixels[ otherIndex + 1 ]
										averageBlue = averageBlue + pixels[ otherIndex + 2 ]
										averageCount = averageCount + 1
								
								if canDown and canLeft:
									otherIndex = offset + pcompStride - 4
									if pixels[ otherIndex + 3 ] > 0.0 and pixels[ otherIndex + 3 ] < curMark:
										averageRed = averageRed + pixels[ otherIndex ]
										averageGreen = averageGreen + pixels[ otherIndex + 1 ]
										averageBlue = averageBlue + pixels[ otherIndex + 2 ]
										averageCount = averageCount + 1
								
								if canDown:
									otherIndex = offset + pcompStride
									if pixels[ otherIndex + 3 ] > 0.0 and pixels[ otherIndex + 3 ] < curMark:
										averageRed = averageRed + pixels[ otherIndex ]
										averageGreen = averageGreen + pixels[ otherIndex + 1 ]
										averageBlue = averageBlue + pixels[ otherIndex + 2 ]
										averageCount = averageCount + 1
								
								if canDown and canRight:
									otherIndex = offset + pcompStride + 4
									if pixels[ otherIndex + 3 ] > 0.0 and pixels[ otherIndex + 3 ] < curMark:
										averageRed = averageRed + pixels[ otherIndex ]
										averageGreen = averageGreen + pixels[ otherIndex + 1 ]
										averageBlue = averageBlue + pixels[ otherIndex + 2 ]
										averageCount = averageCount + 1
								
								if averageCount > 0:
									div = 1.0 / float( averageCount )
									pixels[ offset ] = averageRed * div
									pixels[ offset + 1 ] = averageGreen * div
									pixels[ offset + 2 ] = averageBlue * div
									pixels[ offset + 3 ] = curMark
									tile.remainingPixelCount = tile.remainingPixelCount - 1
									remainingPixelCount = remainingPixelCount - 1
					
					if tile.remainingPixelCount < oldPixelCount:
						tile.wakeUp()
			
			tiles = [ tile for tile in tiles if tile.remainingPixelCount > 0 ]
			
			curMark = curMark + 1.0
			print( "pass finished: time={0:.3f}s remainingPixelCount={1} remainingTiles={2}(sleeping={3} active={4})".format( \
				time.time() - timeStart, remainingPixelCount, len( tiles ), sum( 1 for tile in tiles if tile.sleeping ), \
				sum( 1 for tile in tiles if not tile.sleeping ) ) )
			#if curMark == 100:
			#	break
		print( "finished passes in total {0:.3f}s".format( time.time() - timeTotalStart ) )
		
		# update up the alpha values
		timeStart = time.time()
		for i in range( 3, pcompCount, 4 ):
			pixels[ i ] = 0.0 if pixels[ i ] == 0 else 1.0
		print( "update image alpha in {0:.3f}s".format( time.time() - timeStart ) )
		
		# update the image with the new data
		timeStart = time.time()
		image.pixels = pixels
		image.update()
		print( "update image pixels in {0:.3f}s".format( time.time() - timeStart ) )
		
		bpy.context.area.tag_redraw()
		
		return { 'FINISHED' }
registerClass(OBJECT_OT_ToolTextureFill)


# Blender normal to Drag[en]gine normal
#########################################

class OBJECT_OT_ToolEncBLNor2DENor( bpy.types.Operator ):
	bl_idname = "dragengine.encblnor2denor"
	bl_label = "EncBLNor2DENor"
	__doc__ = """Convert encoded Blender normal to encoded Drag[en]gine normal"""
	
	
	
	@classmethod
	def poll( cls, context ):
		return context.area.type == 'IMAGE_EDITOR' \
			and context.space_data.image \
			and context.space_data.image.depth >= 24 \
			and context.space_data.image.size[0] > 0 \
			and context.space_data.image.size[1] > 0
	
	def execute( self, context ):
		image = context.space_data.image
		width, height = image.size
		pixelCount = width * height
		componentCount = 4
		
		# create working copy (faster than direct access)
		timeStart = time.time()
		pixels = [ x for x in image.pixels ]
		print( "create working copy of {0}x{1} image in {2:.3f}s".format( width, height, time.time() - timeStart ) )
		
		# convert normals
		for i in range( pixelCount ):
			offset = i * componentCount
			pixels[ offset + 1 ] = 1.0 - pixels[ offset + 1 ] # flip green
		print( "convertex normals {0:.3f}s".format( time.time() - timeStart ) )
		
		# update the image with the new data
		timeStart = time.time()
		image.pixels = pixels
		image.update()
		print( "update image pixels in {0:.3f}s".format( time.time() - timeStart ) )
		
		bpy.context.area.tag_redraw()
		
		return { 'FINISHED' }
registerClass(OBJECT_OT_ToolEncBLNor2DENor)
