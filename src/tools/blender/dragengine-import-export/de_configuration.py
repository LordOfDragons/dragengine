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



class Configuration:
	def __init__( self, textObjectName ):
		self.textObjectName = textObjectName
		self.config = {}
	
	def parseConfig( self ):
		# reset the configuration
		self.config = {}
		
		# try to locate the configuration text object
		if self.textObjectName in bpy.data.texts:
			text = bpy.data.texts[ self.textObjectName ]
		else:
			return True
		
		# setup some regexp for later use
		reKey = re.compile( "^[A-Za-z][A-Za-z0-9_]*(\.[A-Za-z][A-Za-z0-9_]*)*" )
		reValueString = re.compile( "^\"([^\"]|\\\\\")*\"" )
		reValueNumber = re.compile( "^-?[0-9]+(\.[0-9]+)?" )
		
		# parse the text
		for line in text.lines:
			line = line.body
			
			# remove whitespaces from start and end
			line = line.strip()
			
			# ignore if an empty line
			if len( line ) == 0:
				continue
			
			# ignore if a comment
			if line[ 0 ] == '#':
				continue
			
			# this has to be a command so read it
			match = reKey.match( line )
			if not match:
				print( "Invalid command '%s'." % ( line ) )
				return False
			key = match.group()
			line = line[ match.end() : len( line ) ]
			
			if len( line ) == 0 or not line[ 0 ].isspace():
				print( "Values missing for key '%s'." % ( key ) )
				return False
			line = line.lstrip()
			
			values = []
			while len( line ) > 0:
				match = reValueString.match( line )
				if match:
					values.append( match.group()[ 1 : -1 ] )
				else:
					match = reValueNumber.match( line )
					if match:
						values.append( float( match.group() ) )
					else:
						return False
				
				nextLine = line[ match.end() : len( line ) ]
				if len( nextLine ) > 0 and not nextLine[ 0 ].isspace():
					print( "Invalid value '%s'." % ( line ) )
					return False
				line = nextLine.lstrip()
			
			if key in self.config:
				self.config[ key ].append( values )
				
			else:
				self.config[ key ] = [ values ]
		
		return True
	
	def getValueFor( self, key, value ):
		if key in self.config:
			return self.config[ key ][ 0 ][ 0 ]
		else:
			return value
	
	def getValuesFor( self, key, values ):
		if key in self.config:
			return self.config[ key ][ 0 ]
		else:
			return values
	
	def getArrayFor( self, key, value ):
		if key in self.config:
			return self.config[ key ]
		else:
			return value
	
	def printConfig( self ):
		print( "configuration:" )
		for key in self.config.keys():
			print( "%s = %s" % ( key, self.config[ key ] ) )
