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
import math
import time
import struct
import mathutils

from mathutils import Vector, Matrix, Quaternion

# constants
PI = 3.14159265
HALF_PI = PI / 2.0
ONE_PI = PI / 180.0

transformPosition = Matrix( ( (-1,0,0,0), (0,0,1,0), (0,-1,0,0), (0,0,0,1) ) )
transformBonePosition = Matrix( ( (1,0,0,0), (0,0,1,0), (0,1,0,0), (0,0,0,1) ) )

transformPositionInv = Matrix( ( (-1,0,0,0), (0,0,-1,0), (0,1,0,0), (0,0,0,1) ) )
transformBonePositionInv = Matrix( ( (1,0,0,0), (0,0,1,0), (0,1,0,0), (0,0,0,1) ) )



# Math classes
################

# helper function formating floating point as text without exponent and trailing
# zeros stripped but at max 4 decimals.
def float2String( floatValue ):
	return '{:.4f}'.format( floatValue ).rstrip( '0' ).rstrip( '.' )

# helper function for writing texel positions
# texel is here an array of two floats in
# blender coordinates.
def writeTexel( f, texel ):
	f.write( struct.pack( "<ff", texel[ 0 ], 1.0 - texel[ 1 ] ) )

# helper function to retrieve time in seconds
def getTime( timeLoc, scale ):
	return float( timeLoc - 1 ) * scale
	#return round( float( timeLoc - 1 ) * scale, 3 )


# helper function to convert matrix from blender to epsylon matrix
def convertMatrix( matrix ):
	# epsylonAxisX = -blenderAxisX
	# epsylonAxisY = blenderAxisZ
	# epsylonAxisZ = -blenderAxisY
	"""
	bax = ( -matrix[0][0], -matrix[1][0], -matrix[2][0] )
	bay = ( matrix[0][2], matrix[1][2], matrix[2][2] )
	baz = ( -matrix[0][1], -matrix[1][1], -matrix[2][1] )
	bpos = ( matrix[0][3], matrix[1][3], matrix[2][3] )
	print( bax, bay, baz, bpos )
	eax = ( -bax[0], bax[2], -bax[1] )
	eay = ( -bay[0], bay[2], -bay[1] )
	eaz = ( -baz[0], baz[2], -baz[1] )
	epos = ( -bpos[0], bpos[2], -bpos[1] )
	print( eax, eay, eaz, epos )
	row1 = ( eax[0], eay[0], eaz[0], epos[0] )
	row2 = ( eax[1], eay[1], eaz[1], epos[1] )
	row3 = ( eax[2], eay[2], eaz[2], epos[2] )
	row4 = (    0.0,    0.0,    0.0,     1.0 )
	"""
	row1 = (  matrix[0][0], -matrix[0][2],  matrix[0][1], -matrix[0][3] )
	row2 = ( -matrix[2][0],  matrix[2][2], -matrix[2][1],  matrix[2][3] )
	row3 = (  matrix[1][0], -matrix[1][2],  matrix[1][1], -matrix[1][3] )
	row4 = (       0      ,       0      ,       0      ,       1       )
	return Matrix( ( row1, row2, row3, row4 ) )

def convertMatrixBone( matrix ):
	# epsylonAxisX = blenderBoneAxisX
	# epsylonAxisY = blenderBoneAxisZ
	# epsylonAxisZ = blenderBoneAxisY
	"""
	bax = ( matrix[0][0], matrix[1][0], matrix[2][0] )
	bay = ( matrix[0][2], matrix[1][2], matrix[2][2] )
	baz = ( matrix[0][1], matrix[1][1], matrix[2][1] )
	bpos = ( matrix[0][3], matrix[1][3], matrix[2][3] )
	print( bax, bay, baz, bpos )
	eax = ( -bax[0], bax[2], -bax[1] )
	eay = ( -bay[0], bay[2], -bay[1] )
	eaz = ( -baz[0], baz[2], -baz[1] )
	epos = ( -bpos[0], bpos[2], -bpos[1] )
	print( eax, eay, eaz, epos )
	row1 = ( eax[0], eay[0], eaz[0], epos[0] )
	row2 = ( eax[1], eay[1], eaz[1], epos[1] )
	row3 = ( eax[2], eay[2], eaz[2], epos[2] )
	row4 = (    0.0,    0.0,    0.0,     1.0 )
	"""
	row1 = ( -matrix[0][0], -matrix[0][2], -matrix[0][1], -matrix[0][3] )
	row2 = (  matrix[2][0],  matrix[2][2],  matrix[2][1],  matrix[2][3] )
	row3 = ( -matrix[1][0], -matrix[1][2], -matrix[1][1], -matrix[1][3] )
	row4 = (       0      ,       0      ,       0      ,       1       )
	return Matrix( ( row1, row2, row3, row4 ) )

def convertEuler( euler ):
	"""
	matrix = euler.to_matrix()
	axisX = ( matrix[0][0], -matrix[0][2], matrix[0][1] )
	axisY = ( matrix[1][0], -matrix[1][2], matrix[1][1] )
	axisZ = ( -matrix[2][0], matrix[2][2], -matrix[2][1] )
	return matrixToEuler( Matrix( ( axisX, axisZ, axisY ) ) )
	"""
	return matrixToEuler( convertMatrix( euler.to_matrix().to_4x4() ) )

# helper function to convert matrix from epsylon to blender matrix
def convertMatrixInv( matrix ):
	# blenderAxisX = -epsylonAxisX
	# blenderAxisY = -epsylonAxisZ
	# blenderAxisZ = epsylonAxisY
	"""
	eax = ( -matrix[0][0], -matrix[1][0], -matrix[2][0] )
	eay = ( -matrix[0][2], -matrix[1][2], -matrix[2][2] )
	eaz = ( matrix[0][1], matrix[1][1], matrix[2][1] )
	epos = ( matrix[0][3], matrix[1][3], matrix[2][3] )
	
	bax = ( -eax[0], -eax[2], eax[1] )
	bay = ( -eay[0], -eay[2], eay[1] )
	baz = ( -eaz[0], -eaz[2], eaz[1] )
	bpos = ( -epos[0], -epos[2], epos[1] )
	
	row1 = ( bax[0], bay[0], baz[0], bpos[0] )
	row2 = ( bax[1], bay[1], baz[1], bpos[1] )
	row3 = ( bax[2], bay[2], baz[2], bpos[2] )
	row4 = (    0.0,    0.0,    0.0,     1.0 )
	"""
	row1 = (  matrix[0][0],  matrix[0][2], -matrix[0][1], -matrix[0][3] )
	row2 = (  matrix[2][0],  matrix[2][2], -matrix[2][1], -matrix[2][3] )
	row3 = ( -matrix[1][0], -matrix[1][2],  matrix[1][1],  matrix[1][3] )
	row4 = (       0      ,       0      ,       0      ,       1       )
	return Matrix( ( row1, row2, row3, row4 ) )

def convertMatrixBoneInv( matrix ):
	# blenderBoneAxisX = epsylonAxisX
	# blenderBoneAxisY = epsylonAxisZ
	# blenderBoneAxisZ = epsylonAxisY
	"""
	eax = ( matrix[0][0], matrix[1][0], matrix[2][0] )
	eay = ( matrix[0][2], matrix[1][2], matrix[2][2] )
	eaz = ( matrix[0][1], matrix[1][1], matrix[2][1] )
	epos = ( matrix[0][3], matrix[1][3], matrix[2][3] )
	
	bax = ( -eax[0], -eax[2], eax[1] )
	bay = ( -eay[0], -eay[2], eay[1] )
	baz = ( -eaz[0], -eaz[2], eaz[1] )
	bpos = ( -epos[0], -epos[2], epos[1] )
	
	row1 = ( bax[0], bay[0], baz[0], bpos[0] )
	row2 = ( bax[1], bay[1], baz[1], bpos[1] )
	row3 = ( bax[2], bay[2], baz[2], bpos[2] )
	row4 = (    0.0,    0.0,    0.0,     1.0 )
	"""
	row1 = ( -matrix[0][0], -matrix[0][2], -matrix[0][1], -matrix[0][3] )
	row2 = ( -matrix[2][0], -matrix[2][2], -matrix[2][1], -matrix[2][3] )
	row3 = (  matrix[1][0],  matrix[1][2],  matrix[1][1],  matrix[1][3] )
	row4 = (       0      ,       0      ,       0      ,       1       )
	return Matrix( ( row1, row2, row3, row4 ) )

# create a dragengine rotation matrix
def deRotationMatrix( r ):
	a = math.sin( r[0] * ONE_PI )
	b = math.cos( r[0] * ONE_PI )
	c = math.sin( r[1] * ONE_PI )
	d = math.cos( r[1] * ONE_PI )
	e = math.sin( r[2] * ONE_PI )
	f = math.cos( r[2] * ONE_PI )
	g = d * f
	h = c * f
	i = c * e
	j = d * e
	#axisX = Vector( [ g-a*i, a*h+j, -c*b, 0 ] )
	#axisY = Vector( [ -b*e, b*f, a, 0 ] )
	#axisZ = Vector( [ h+a*j, -a*g+i, b*d, 0 ] )
	axisX = ( g-a*i, -b*e, h+a*j, 0.0 )
	axisY = ( a*h+j, b*f, -a*g+i, 0.0 )
	axisZ = ( -c*b, a, b*d, 0.0 )
	return Matrix( ( axisX, axisY, axisZ, ( 0.0, 0.0, 0.0, 1.0 ) ) )

# helper function calculating the euler angles
# from a given converted matrix
def matrixToEuler( matrix ):
	euler = Vector( [ 0.0, 0.0, 0.0 ] )
	if matrix[1][2] < 0.99999:
		if matrix[1][2] > -0.99999:
			euler = Vector( [ math.asin( matrix[1][2] ), math.atan2( -matrix[0][2], matrix[2][2] ), math.atan2( -matrix[1][0], matrix[1][1] ) ] )
		else: # undefined
			euler = Vector( [ -PI * 0.5, 0.0, -math.atan2( matrix[2][0], matrix[0][0] ) ] )
	else: # undefined
		euler = Vector( [ PI * 0.5, 0.0, math.atan2( matrix[2][0], matrix[0][0] ) ] )
	return Vector( [ euler.x / ONE_PI, euler.y / ONE_PI, euler.z / ONE_PI ] )

# helper function calculating the quaternion from a converted matrix
def matrixToQuaternion( matrix ):
	trace = matrix[0][0] + matrix[1][1] + matrix[2][2] + 1.0
	if trace > 0.000001:
		s = 0.5 / math.sqrt( trace )
		return Quaternion( ( 0.25 / s, ( matrix[2][1] - matrix[1][2] ) * s, ( matrix[0][2] - matrix[2][0] ) * s, ( matrix[1][0] - matrix[0][1] ) * s ) )
	elif matrix[0][0] > matrix[1][1] and matrix[0][0] > matrix[2][2]:
		s = 2.0 * math.sqrt( 1.0 + matrix[0][0] - matrix[1][1] - matrix[2][2] )
		return Quaternion( ( ( matrix[1][2] - matrix[2][1] ) / s, 0.25 * s, ( matrix[0][1] + matrix[1][0] ) / s, ( matrix[0][2] + matrix[2][0] ) / s ) )
	elif matrix[1][1] > matrix[2][2]:
		s = 2.0 * math.sqrt( 1.0 + matrix[1][1] - matrix[0][0] - matrix[2][2] )
		return Quaternion( ( ( matrix[0][2] - matrix[2][0] ) / s, ( matrix[0][1] + matrix[1][0] ) / s, 0.25 * s, ( matrix[1][2] + matrix[2][1] ) / s ) )
	else:
		s = 2.0 * math.sqrt( 1.0 + matrix[2][2] - matrix[0][0] - matrix[1][1] )
		return Quaternion( ( ( matrix[0][1] - matrix[1][0] ) / s, ( matrix[0][2] + matrix[2][0] ) / s, ( matrix[1][2] + matrix[2][1] ) / s, 0.25 * s ) )

# helper function to transform a vector by a matrix
def vector_by_matrix( m, p ):
	"""return Vector( ( p[0] * m[0][0] + p[1] * m[1][0] + p[2] * m[2][0] + m[3][0],
		p[0] * m[0][1] + p[1] * m[1][1] + p[2] * m[2][1] + m[3][1],
		p[0] * m[0][2] + p[1] * m[1][2] + p[2] * m[2][2] + m[3][2] ) )"""
	return Vector( ( p[0] * m[0][0] + p[1] * m[0][1] + p[2] * m[0][2] + m[0][3],
		p[0] * m[1][0] + p[1] * m[1][1] + p[2] * m[1][2] + m[1][3],
		p[0] * m[2][0] + p[1] * m[2][1] + p[2] * m[2][2] + m[2][3] ) )

# helper function to transform an axis vector by a matrix
def axis_by_matrix( m, p ):
	"""return Vector( ( p[0] * m[0][0] + p[1] * m[1][0] + p[2] * m[2][0],
		p[0] * m[0][1] + p[1] * m[1][1] + p[2] * m[2][1],
		p[0] * m[0][2] + p[1] * m[1][2] + p[2] * m[2][2] ) )"""
	return Vector( ( p[0] * m[0][0] + p[1] * m[0][1] + p[2] * m[0][2],
		p[0] * m[1][0] + p[1] * m[1][1] + p[2] * m[1][2],
		p[0] * m[2][0] + p[1] * m[2][1] + p[2] * m[2][2] ) )

# vector1 + vector2
def vecAdd( v1, v2 ):
	return [ v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2] ]

# vector1 - vector2
def vecSub( v1, v2 ):
	return [ v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2] ]

# vector1 * scalar
def vecMul( v1, s ):
	return [ v1[0] * s, v1[1] * s, v1[2] * s ]

# vector / scalar
def vecDiv( v1, s ):
	return [ v1[0] / s, v1[1] / s, v1[2] / s ]

# vector1 dot-product vector2
def vecDot( v1, v2 ):
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]

# vector1 cross-product vector2
def vecCross( v1, v2 ):
	return [ v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2], v1[0] * v2[1] - v1[1] * v2[0] ]

# vector length
def vecLength( v ):
	return math.sqrt( v[0] * v[0] + v[1] * v[1] + v[2] * v[2] )

# normalize vector
def vecNormalize( v ):
	factor = 1.0 / math.sqrt( v[0] * v[0] + v[1] * v[1] + v[2] * v[2] )
	return [ v[0] * factor, v[1] * factor, v[2] * factor ]

# -vector1
def vecNeg( v ):
	return [ -v[0], -v[1], -v[2] ]

# vector1 - vector2
def vec2Sub( v1, v2 ):
	return [ v1[0] - v2[0], v1[1] - v2[1] ]

# quaternion1 dot-product quaternion2
def quatDot( q1, q2 ):
	return q1[0] * q2[0] + q1[1] * q2[1] + q1[2] * q2[2] + q1[3] * q2[3]

# negate quaternion
def quatNegate( q ):
	return [ -q[0], -q[1], -q[2], -q[3] ]

# determines if a ray hits a triangle
def rayHitsTriangle( rayOrigin, rayDirection, tri1, tri2, tri3 ):
	edge1 = vecSub( tri2, tri1 )
	edge2 = vecSub( tri3, tri2 )
	trinormal = vecCross( edge1, edge2 )
	dot = vecDot( rayDirection, trinormal )
	
	if dot < -0.00001 or dot > 0.00001:
		dist = vecDot( vecSub( tri1, rayOrigin ), trinormal ) / dot
		
		if dist > 0.0:
			hitPoint = vecAdd( rayOrigin, vecMul( rayDirection, dist ) )
			
			if vecDot( vecCross( edge1, vecSub( hitPoint, tri1 ) ), trinormal ) < 0.0:
				return False
			if vecDot( vecCross( edge2, vecSub( hitPoint, tri2 ) ), trinormal ) < 0.0:
				return False
			
			edge3 = vecSub( tri1, tri3 )
			if vecDot( vecCross( edge3, vecSub( hitPoint, tri3 ) ), trinormal ) < 0.0:
				return False
			
			return True
	
	return False

# determines if a ray hits a triangle with normal already calculated
def rayHitsTriangleNormal( rayOrigin, rayDirection, tri1, tri2, tri3, trinormal ):
	dot = vecDot( rayDirection, trinormal )
	
	if dot < -0.00001 or dot > 0.00001:
		dist = vecDot( vecSub( tri1, rayOrigin ), trinormal ) / dot
		
		if dist > 0.0:
			hitPoint = vecAdd( rayOrigin, vecMul( rayDirection, dist ) )
			
			if vecDot( vecCross( vecSub( tri2, tri1 ), vecSub( hitPoint, tri1 ) ), trinormal ) < 0.0:
				return False
			if vecDot( vecCross( vecSub( tri3, tri2 ), vecSub( hitPoint, tri2 ) ), trinormal ) < 0.0:
				return False
			if vecDot( vecCross( vecSub( tri1, tri3 ), vecSub( hitPoint, tri3 ) ), trinormal ) < 0.0:
				return False
			
			return True
	
	return False

# m[0][0] m[0][1] m[0][2] m[0][3]
# m[1][0] m[1][1] m[1][2] m[1][3]
# m[2][0] m[2][1] m[2][2] m[2][3]
# m[3][0] m[3][1] m[3][2] m[3][3]

# (1,0,0) => ( m[0][0] + m[3][0] , m[0][1] + m[3][1] , m[0][2] + m[3][2] )
# (0,1,0) => ( m[1][0] + m[3][0] , m[1][1] + m[3][1] , m[1][2] + m[3][2] )
# (0,0,1) => ( m[2][0] + m[3][0] , m[2][1] + m[3][1] , m[2][2] + m[3][2] )

# [ x.x  x.y  x.z  0 ]
# [ y.x  y.y  y.z  0 ]
# [ z.x  z.x  z.z  0 ]
# [  0    0    0   0 ]

# convert position from world coordinate system:
# [ -1  0  0  0 ]
# [  0  0 -1  0 ]
# [  0  1  0  0 ]
# [  0  0  0  0 ]

# convert position from bone coordinate system:
# [  1  0  0  0 ]
# [  0  0  1  0 ]
# [  0  1  0  0 ]
# [  0  0  0  0 ]



# Collision Detection
#######################

def projectToPlane( point, planeNormal, planePosition ):
	return point - planeNormal * ( planeNormal * ( point - planePosition ) )

def closestPointOnLine( lineStart, lineEnd, point ):
	lineDir = lineEnd - lineStart
	pointDir = point - lineStart
	denom = lineDir * lineDir
	if math.fabs( denom ) > 0.000001:
		lam = ( lineDir * pointDir ) / denom
		if lam <= 0.0: return lineStart
		if lam >= 1.0: return lineEnd
		return lineStart + lineDir * lam
	else:
		return lineStart

def closestPointOnTriangle( tp1, tp2, tp3, point ):
	edge0 = tp2 - tp1
	edge1 = tp3 - tp2
	faceNormal = edge0.cross( edge1 ).normalized()
	
	normal = faceNormal.cross( edge0 ).normalized()
	distance = ( point - tp1 ).dot( normal )
	if distance < 0.0:
		point = point - normal * distance
	
	normal = faceNormal.cross( edge1 ).normalized()
	distance = ( point - tp2 ).dot( normal )
	if distance < 0.0:
		point = point - normal * distance
	
	normal = faceNormal.cross( tp1 - tp3 ).normalized()
	distance = ( point - tp3 ).dot( normal )
	if distance < 0.0:
		point = point - normal * distance
	
	return point - faceNormal * ( point - tp1 ).dot( faceNormal )

def closestPointOnQuad( tp1, tp2, tp3, tp4, point ):
	edge0 = tp2 - tp1
	edge1 = tp3 - tp2
	faceNormal = edge0.cross( edge1 ).normalized()
	
	normal = faceNormal.cross( edge0 ).normalized()
	distance = ( point - tp1 ).dot( normal )
	if distance < 0.0:
		point = point - normal * distance
	
	normal = faceNormal.cross( edge1 ).normalized()
	distance = ( point - tp2 ).dot( normal )
	if distance < 0.0:
		point = point - normal * distance
	
	normal = faceNormal.cross( tp4 - tp3 ).normalized()
	distance = ( point - tp3 ).dot( normal )
	if distance < 0.0:
		point = point - normal * distance
	
	normal = faceNormal.cross( tp1 - tp4 ).normalized()
	distance = ( point - tp4 ).dot( normal )
	if distance < 0.0:
		point = point - normal * distance
	
	return point - faceNormal * ( point - tp1 ).dot( faceNormal )

def bvecL( vector1, vector2 ):
	return vector1.x < vector2.x and vector1.y < vector2.y and vector1.z < vector2.z

def bvecLE( vector1, vector2 ):
	return vector1.x <= vector2.x and vector1.y <= vector2.y and vector1.z <= vector2.z

def bvecG( vector1, vector2 ):
	return vector1.x > vector2.x and vector1.y > vector2.y and vector1.z > vector2.z

def bvecGE( vector1, vector2 ):
	return vector1.x >= vector2.x and vector1.y >= vector2.y and vector1.z >= vector2.z



# Octree
##########

class Octree:
	class Visitor:
		def visitNode( self, node, intersection ): pass
	
	XNEG_YNEG_ZNEG = 0
	XNEG_YNEG_ZPOS = 1
	XNEG_YPOS_ZNEG = 2
	XNEG_YPOS_ZPOS = 3
	XPOS_YNEG_ZNEG = 4
	XPOS_YNEG_ZPOS = 5
	XPOS_YPOS_ZNEG = 6
	XPOS_YPOS_ZPOS = 7
	NOT_FOUND = 8
	
	FRONT_LEFT_TOP = XNEG_YPOS_ZNEG
	FRONT_RIGHT_TOP = XPOS_YPOS_ZNEG
	FRONT_LEFT_BOTTOM = XNEG_YNEG_ZNEG
	FRONT_RIGHT_BOTTOM = XPOS_YNEG_ZNEG
	BACK_LEFT_TOP = XNEG_YPOS_ZPOS
	BACK_RIGHT_TOP = XPOS_YPOS_ZPOS
	BACK_LEFT_BOTTOM = XNEG_YNEG_ZPOS
	BACK_RIGHT_BOTTOM = XPOS_YNEG_ZPOS
	
	INSIDE = 0
	PARTIAL = 1
	OUTSIDE = 2
	
	def __init__( self, center, halfsize ):
		self.nodes = [ None ] * 8
		self.center = center
		self.halfsize = halfsize
		self.minextend = center - halfsize
		self.maxextend = center + halfsize
		self.parent = None
	
	def getNodeAtBox( self, boxCenter, boxHalfSize ):
		octant = self.findOctantAtBox( boxCenter, boxHalfSize )
		if octant == Octree.NOT_FOUND:
			return None
		else:
			if not self.nodes[ octant ]:
				self.nodes[ octant ] = self.createOctree( octant )
				self.nodes[ octant ].parent = self
			return self.nodes[ octant ]
	
	def findNodeAtBox( self, boxCenter, boxHalfSize ):
		octant = self.findOctantAtBox( boxCenter, boxHalfSize )
		return None if octant == Octree.NOT_FOUND else self.nodes[ octant ]
	
	def findOctantAtBox( self, boxCenter, boxHalfSize ):
		octant = 0
		if boxCenter.x - boxHalfSize.x >= self.center.x:
			octant = octant | 4
		elif boxCenter.x + boxHalfSize.x >= self.center.x:
			return Octree.NOT_FOUND
		if boxCenter.y - boxHalfSize.y >= self.center.y:
			octant = octant | 2
		elif boxCenter.y + boxHalfSize.y >= self.center.y:
			return Octree.NOT_FOUND
		if boxCenter.z - boxHalfSize.z >= self.center.z:
			octant = octant | 1
		elif boxCenter.z + boxHalfSize.z >= self.center.z:
			return Octree.NOT_FOUND
		return octant
	
	def containsBox( self, boxCenter, boxHalfSize ):
		return bvecGE( boxCenter - boxHalfSize, self.minextend ) and bvecL( boxCenter + boxHalfSize, self.maxextend )
	
	def findNodeAtPoint( self, point ):
		octant = self.findOctantAtPoint( point )
		return None if octant == Octree.NOT_FOUND else self.nodes[ octant ]
	
	def findOctantAtPoint( self, point ):
		bitmask = 0
		if point.x >= self.center.x: bitmask = bitmask | 4
		if point.y >= self.center.y: bitmask = bitmask | 2
		if point.z >= self.center.z: bitmask = bitmask | 1
		return bitmask
	
	def containsPoint( self, point ):
		return bvecGE( point, self.minextend ) and bvecL( point, self.maxextend )
	
	def searchTreeForBox( self, boxCenter, boxHalfSize ):
		nextNode = self.findNodeAtBox( boxCenter, boxHalfSize )
		curNode = self
		while nextNode:
			curNode = nextNode
			nextNode = curNode.findNodeAtBox( boxCenter, boxHalfSize )
		return curNode
	
	def searchTreeForPoint( self, point ):
		nextNode = self.findNodeAtPoint( point )
		curNode = self
		while nextNode:
			curNode = nextNode
			nextNode = curNode.findNodeAtPoint( point )
		return curNode
	
	def visitNodes( self, visitor ):
		visitor.visitNode( self, Octree.INSIDE )
		for node in self.nodes: node and node.visitNodes( visitor )
	
	def intersectsBox( self, minExtend, maxExtend ):
		if self.maxextend.x < minExtend.x or self.maxextend.y < minExtend.y or self.maxextend.z < minExtend.z: return Octree.OUTSIDE
		if self.minextend.x > maxExtend.x or self.minextend.y > maxExtend.y or self.minextend.z > maxExtend.z: return Octree.OUTSIDE
		if self.minextend.x < minExtend.x or self.minextend.y < minExtend.y or self.minextend.z < minExtend.z: return Octree.PARTIAL
		if self.maxextend.x > maxExtend.x or self.maxextend.y > maxExtend.y or self.maxextend.z > maxExtend.z: return Octree.PARTIAL
		return Octree.INSIDE
	
	def visitNodesColliding( self, visitor, boxMinExtend, boxMaxExtend ):
		result = self.intersectsBox( boxMinExtend, boxMaxExtend )
		if result == Octree.INSIDE:
			self.visitNodes( visitor )
		elif result == Octree.PARTIAL:
			visitor.visitNode( self, Octree.PARTIAL )
			for node in self.nodes: node and node.visitNodesColliding( visitor, boxMinExtend, boxMaxExtend )
	
	def clearTree( self, clearNodes ):
		self.clearNodeContent()
		for i in range( 8 ):
			node = self.nodes[ i ]
			if node:
				node.clearTree( clearNodes )
				if clearNodes:
					self.nodes[ i ] = None
		if clearNodes:
			self.parent = None
	
	def createOctree( self, octant ):
		hs = self.halfsize * 0.5
		c = self.center
		b = [ 4, 2, 1 ]
		return Octree( Vector( [ c[i]+hs[i] if octant&b[i]==b[i] else c[i]-hs[i] for i in range(3) ] ), hs )
	
	def clearNodeContent( self ): pass
	
	def insertIntoTree( self, boxCenter, boxHalfSize, maxDepth=4 ):
		curNode = self
		nextNode = None
		for d in range( maxDepth ):
			nextNode = curNode.getNodeAtBox( boxCenter, boxHalfSize )
			if not nextNode: break
			curNode = nextNode
		return curNode
	
	def printTree( self, indent, nodeIndex ):
		str1 = "({0:.5f},{1:.5f},{2:.5f})".format( self.minextend.x, self.minextend.y, self.minextend.z )
		str2 = "({0:.5f},{1:.5f},{2:.5f})".format( self.maxextend.x, self.maxextend.y, self.maxextend.z )
		print( "{0}node {1}: min={3} max={4}".format( indent, nodeIndex, str1, str2 ) )
		for i in range( 8 ):
			if self.nodes[ i ]:
				self.nodes[ i ].printTree( indent + " ", i )
			else:
				print( "{0}node {1}: <empty>".format( indent + " ", i ) )
	