# -*- coding: utf-8 -*-
#
# Drag[en]gine Blender Scripts
#
# Copyright (C) 2013, Plüss Roland ( roland@rptd.ch )
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
import math

from .de_math import vecDot, vecNormalize, ONE_PI
from .de_porting import registerClass



# AO Generation
#################

class OBJECT_OT_ToolAOGeneration( bpy.types.Operator ):
	bl_idname = "dragengine.aogeneration"
	bl_label = "AO Generation from normal map"
	__doc__ = """AO Generation from normal map"""
	
	@classmethod
	def poll( cls, context ):
		return context.area.type == 'IMAGE_EDITOR' and context.space_data.image \
			and context.space_data.image.type == 'IMAGE' and context.space_data.image.depth == 24
	
	def execute( self, context ):
		image = context.space_data.image
		
		if image.type == 'RENDER_RESULT':
			# this is the rendered result image. this image has special properties:
			# - image.type is 'RENDER_RESULT'
			# - image.view_as_renderis True (False for real images)
			# - image.size is (0,0)
			# - image.depth is 0
			# - image.pixels is a 0-length array
			# - image.resolution is (0,0)
			pass
			
		else:
			# this is a conventional image
			width, height = image.size
			pixelCount = width * height
			pcompCount = 4
			pcompStride = width * pcompCount
			
			# create working copy of the normal pixels since this is a lot faster than direct access
			timeStart = time.time()
			norpixels = image.pixels[:]
			print( "create working copy of {0}x{1} image in {2:.3f}s".format( width, height, time.time() - timeStart ) )
			
			# process image
			timeStart = time.time()
			
			tapCount = 18
			turnStep = 6.2831853 / float( tapCount )
			selfOcclusionAngle = 0.0
			selfOcclusionBias = 1.0 - math.cos( selfOcclusionAngle * ONE_PI )
			aoscale = 2.0 / float( tapCount )
			gamma = 1.0 / 2.2
			
			turnAngles = [ turnStep * float( i ) for i in range( tapCount ) ]
			tcnormals = [ vecNormalize( [ math.cos( turnAngles[i] ), math.sin( turnAngles[i] ), 0.0 ] )[:2] for i in range( tapCount ) ]
			
			aopixels = []
			
			for y in range( height ):
				if y % 32 == 0:
					print( "processing line {0}...".format( y ) )
				
				baseOffset = pcompStride * y
				
				for x in range( width ):
					offset = baseOffset + pcompCount * x
					
					normal = vecNormalize( [ v*2.0-1.0 for v in norpixels[ offset : offset+pcompCount ] ][:3] )
					
					occaccum = 0.0
					
					for i in range( tapCount ):
						occaccum = occaccum + min( max( ( normal[0]*tcnormals[i][0] + normal[1]*tcnormals[i][1] ), selfOcclusionBias ), 1.0 )
					
					ao = min( max( 1.0 - occaccum * aoscale, 0.0 ), 1.0 )
					ao = math.pow( ao, gamma ) # temporary
					
					aopixels.append( ao ) # red
					aopixels.append( ao ) # green
					aopixels.append( ao ) # blue
					aopixels.append( 1.0 ) # alpha
			print( "processed {0} pixels in {0:.3f}s".format( pixelCount, time.time() - timeStart ) )
			
			# create image with the new data
			timeStart = time.time()
			imgao = bpy.data.images.new( name='aogen', width=width, height=height, alpha=False, float_buffer=False )
			imgao.pixels = aopixels
			imgao.update()
			print( "create image ao in {0:.3f}s".format( time.time() - timeStart ) )
			
		bpy.context.area.tag_redraw()
		
		return { 'FINISHED' }
registerClass(OBJECT_OT_ToolAOGeneration)
