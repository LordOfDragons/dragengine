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
import mathutils

from mathutils import Vector, Matrix, Euler

from .de_math import transformPosition, convertMatrix, vector_by_matrix
from .de_resources import Mesh
from .de_porting import registerClass



# GBufferNormGen
##################

class OBJECT_OT_ToolGBufferNormGen( bpy.types.Operator ):
	bl_idname = "dragengine.gbuffernormgen"
	bl_label = "GBuffer Normal Generation"
	__doc__ = """Generate a GBuffer Scene for normal map generation"""
	
	def addInputSocket( self, nodeGroup, name, socketType, defaultValue=None, hide=False ):
		socket = nodeGroup.inputs.new( name, socketType )
		if defaultValue:
			socket.default_value = defaultValue
		if bpy.app.version[ 0 ] >= 2 and bpy.app.version[ 1 ] >= 64:
			socket.hide = hide
	
	def addOutputSocket( self, nodeGroup, name, socketType, defaultValue=None, hide=False ):
		socket = nodeGroup.outputs.new( name, socketType )
		if defaultValue:
			socket.default_value = defaultValue
		if bpy.app.version[ 0 ] >= 2 and bpy.app.version[ 1 ] >= 64:
			socket.hide = hide
	
	def initNodeDefault( self, node, name, x, y, label, hide ):
		node.name = name
		node.location = Vector( ( x, y ) )
		if label:
			node.label = label
		node.select = False
		node.hide = hide
	
	def addMathNode( self, nodeGroup, name, x, y, operator, defaultValue1=0.0, defaultValue2=0.0, label=None, hide=False ):
		node = nodeGroup.nodes.new( 'MATH' )
		self.initNodeDefault( node, name, x, y, label, hide )
		node.inputs[ 0 ].default_value = defaultValue1
		node.inputs[ 1 ].default_value = defaultValue2
		node.operation = operator
	
	def addVectorMathNode( self, nodeGroup, name, x, y, operator, defaultValue1=(0.0,0.0,0.0), defaultValue2=(0.0,0.0,0.0), label=None, hide=False ):
		node = nodeGroup.nodes.new( 'VECT_MATH' )
		self.initNodeDefault( node, name, x, y, label, hide )
		node.inputs[ 0 ].default_value = Vector( defaultValue1 )
		node.inputs[ 1 ].default_value = Vector( defaultValue2 )
		if operator in [ 'NORMALIZE' ]:
			if bpy.app.version[ 0 ] >= 2 and bpy.app.version[ 1 ] >= 64:
				node.inputs[ 1 ].hide = True
		node.operation = operator
	
	def addSeparateColorNode( self, nodeGroup, name, x, y, label=None, hide=False ):
		node = nodeGroup.nodes.new( 'SEPRGBA' )
		self.initNodeDefault( node, name, x, y, label, hide )
	
	def addSeparateColorNoAlphaNode( self, nodeGroup, name, x, y, label=None, hide=False ):
		node = nodeGroup.nodes.new( 'SEPRGB' )
		self.initNodeDefault( node, name, x, y, label, hide )
	
	def addComposeColorNode( self, nodeGroup, name, x, y, label=None, hide=False ):
		node = nodeGroup.nodes.new( 'COMBRGBA' )
		self.initNodeDefault( node, name, x, y, label, hide )
	
	def addComposeColorNoAlphaNode( self, nodeGroup, name, x, y, label=None, hide=False ):
		node = nodeGroup.nodes.new( 'COMBRGB' )
		self.initNodeDefault( node, name, x, y, label, hide )
	
	def addGroupNode( self, nodeGroup, name, x, y, groupName, label=None, hide=False ):
		node = nodeGroup.nodes.new( 'GROUP', bpy.data.node_groups[ groupName ] )
		self.initNodeDefault( node, name, x, y, label, hide )
	
	def addRenderLayersNode( self, nodeGroup, name, x, y, label=None, hide=False ):
		node = nodeGroup.nodes.new( 'R_LAYERS' )
		self.initNodeDefault( node, name, x, y, label, hide )
	
	def addCompositeNode( self, nodeGroup, name, x, y, label=None, hide=False ):
		node = nodeGroup.nodes.new( 'COMPOSITE' )
		self.initNodeDefault( node, name, x, y, label, hide )
	
	def addTextureNode( self, nodeGroup, name, x, y, texture, label=None, hide=False ):
		node = nodeGroup.nodes.new( 'TEXTURE' )
		self.initNodeDefault( node, name, x, y, label, hide )
		node.show_preview = False
		if bpy.app.version[ 0 ] >= 2 and bpy.app.version[ 1 ] >= 64:
			node.outputs[ 0 ].hide = True
			node.outputs[ 2 ].hide = True
		node.texture = texture
	
	def addGeometryNode( self, nodeGroup, name, x, y, uvmap, label=None, hide=False ):
		node = nodeGroup.nodes.new( 'GEOMETRY' )
		self.initNodeDefault( node, name, x, y, label, hide )
		node.uv_layer = uvmap
	
	def addOutputNode( self, nodeGroup, name, x, y, label=None, hide=False ):
		node = nodeGroup.nodes.new( 'OUTPUT' )
		self.initNodeDefault( node, name, x, y, label, hide )
	
	def addLink( self, nodeGroup, fromNodeName=None, fromSocket="", toNodeName=None, toSocket="" ):
		if fromNodeName:
			socket1 = nodeGroup.nodes[ fromNodeName ].outputs[ fromSocket ]
		else:
			socket1 = nodeGroup.inputs[ fromSocket ]
		if toNodeName:
			socket2 = nodeGroup.nodes[ toNodeName ].inputs[ toSocket ]
		else:
			socket2 = nodeGroup.outputs[ toSocket ]
		nodeGroup.links.new( socket1, socket2 )
	
	def hideNotLinkedNodeOutputs( self, nodeGroup ):
		if bpy.app.version[ 0 ] >= 2 and bpy.app.version[ 1 ] >= 64:
			for node in nodeGroup.nodes:
				for socket in node.outputs:
					socket.hide = not socket.is_linked
	
	def clearNodeGroup( self, nodeGroup ):
		clearLinks = [ link for link in nodeGroup.links ]
		for link in clearLinks:
			nodeGroup.links.remove( link )
		clearNodes = [ node for node in nodeGroup.nodes ]
		for node in clearNodes:
			nodeGroup.nodes.remove( node )
	
	# init edit mode check
	def initEditMode( self, context ):
		editmode = ( context.mode == 'EDIT_MESH' )
		if editmode:
			bpy.ops.object.mode_set( mode='OBJECT' )
		return editmode
	
	# create sRGB to Linear conversion node group if not existing
	def createGammaFixNodeGroup( self ):
		if not "sRGB to Linear" in bpy.data.node_groups:
			"""
			little helper to convert links from an existing noodle easily
for socket in ng.inputs:
   print( "self.addInputSocket( ng, \"{0}\", \'{1}\', {2} )".format( socket.name, str(socket.type), \
   [ round( value, 3 ) for value in socket.default_value ] ) )

for socket in ng.outputs:
   print( "self.addOutputSocket( ng, \"{0}\", \'{1}\', {2} )".format( socket.name, str(socket.type), \
   [ round( value, 3 ) for value in socket.default_value ] ) )

for node in ng.nodes:
   print( "self.add( ng, \"{0}\", {1:.0f}, {2:.0f} )".format( node.name, node.location.x, node.location.y ) )

for link in ng.links:
   print( "self.addLink( ng, {0}, {1}, {2}, {3} )".format( "\"{0}\"".format(link.from_node.name) if link.from_node else "None", \
   link.from_node.outputs.values().index(link.from_socket) if link.from_node else "\"{0}\"".format(link.from_socket.name), \
   "\"{0}\"".format(link.to_node.name) if link.to_node else "None", link.to_node.inputs.values().index(link.to_socket) \
   if link.to_node else "\"{0}\"".format(link.to_socket.name) ) )
			"""
			ng = bpy.data.node_groups.new( "sRGB to Linear", 'COMPOSITE' )
			self.addInputSocket( ng, "sRGB", 'VALUE', 1.0 ) # ('VALUE', 'VECTOR', 'RGBA', 'SHADER', 'BOOLEAN', 'MESH', 'INT')
			self.addOutputSocket( ng, "Linear", 'VALUE', 1.0 )
			
			self.addMathNode( ng, "fra1node1", -268, 117, 'GREATER_THAN', 0.0, 0.04045 )
			self.addMathNode( ng, "fra1node2", 95, 116, 'SUBTRACT', 1.0 )
			self.addMathNode( ng, "fra2node1", -277, -117, 'ADD', 0.0, 0.055 )
			self.addMathNode( ng, "fra2node2", -90, -116, 'DIVIDE', 0.0, 1.055 )
			self.addMathNode( ng, "fra2node3", 96, -117, 'POWER', 0.0, 2.4 )
			self.addMathNode( ng, "fra3node1", -89, -346, 'DIVIDE', 0.0, 12.920 )
			self.addMathNode( ng, "fra4node1", 381, -18, 'MULTIPLY' )
			self.addMathNode( ng, "fra4node2", 381, -188, 'MULTIPLY' )
			self.addMathNode( ng, "fra4node3", 567, -70, 'ADD' )
			
			self.addLink( ng, None, "sRGB", "fra1node1", 0 )
			self.addLink( ng, None, "sRGB", "fra2node1", 0 )
			self.addLink( ng, None, "sRGB", "fra3node1", 0 )
			self.addLink( ng, "fra1node1", 0, "fra1node2", 1 )
			self.addLink( ng, "fra1node1", 0, "fra4node1", 1 )
			self.addLink( ng, "fra1node2", 0, "fra4node2", 1 )
			self.addLink( ng, "fra2node1", 0, "fra2node2", 0 )
			self.addLink( ng, "fra2node2", 0, "fra2node3", 0 )
			self.addLink( ng, "fra2node3", 0, "fra4node1", 0 )
			self.addLink( ng, "fra3node1", 0, "fra4node2", 0 )
			self.addLink( ng, "fra4node1", 0, "fra4node3", 0 )
			self.addLink( ng, "fra4node2", 0, "fra4node3", 1 )
			self.addLink( ng, "fra4node3", 0, None, "Linear" )
	
	# create sRGB to Linear Color conversion node group if not existing
	def createGammaFixColorNodeGroup( self ):
		if not "sRGB to Linear Color" in bpy.data.node_groups:
			ng = bpy.data.node_groups.new( "sRGB to Linear Color", 'COMPOSITE' )
			self.addInputSocket( ng, "sRGB", 'RGBA', [ 1.0, 1.0, 1.0, 1.0 ] )
			self.addOutputSocket( ng, "Linear", 'RGBA', [ 1.0, 1.0, 1.0, 1.0 ] )
			
			self.addSeparateColorNode( ng, "node1", 13, -11 )
			self.addGroupNode( ng, "node2", 175, 45, "sRGB to Linear" )
			self.addGroupNode( ng, "node3", 174, -43, "sRGB to Linear" )
			self.addGroupNode( ng, "node4", 172, -128, "sRGB to Linear" )
			self.addComposeColorNode( ng, "node5", 404, -5 )
			
			self.addLink( ng, None, "sRGB", "node1", 0 )
			self.addLink( ng, "node1", 0, "node2", 0 )
			self.addLink( ng, "node1", 1, "node3", 0 )
			self.addLink( ng, "node1", 2, "node4", 0 )
			self.addLink( ng, "node2", 0, "node5", 0 )
			self.addLink( ng, "node3", 0, "node5", 1 )
			self.addLink( ng, "node4", 0, "node5", 2 )
			self.addLink( ng, "node5", 0, None, "Linear" )
	
	# create GBuffer World Normal node group if not existing
	def createGBufferWorldNormalNodeGroup( self ):
		if not "GBuffer World Normal" in bpy.data.node_groups:
			ng = bpy.data.node_groups.new( "GBuffer World Normal", 'SHADER' )
			self.addInputSocket( ng, "Color", 'RGBA', [ 0.5, 0.5, 1.0, 1.0 ] )
			self.addOutputSocket( ng, "Normal", 'VECTOR', [ 0.5, 0.5, 1.0 ] )
			
			self.addSeparateColorNoAlphaNode( ng, "node2", 265, 103 )
			self.addMathNode( ng, "node3", 407, 303, 'MULTIPLY', 0.0, 2.0 )
			self.addMathNode( ng, "node4", 575, 302, 'SUBTRACT', 0.0, 1.0 )
			self.addMathNode( ng, "node5", 407, 129, 'MULTIPLY', 0.0, 2.0 )
			self.addMathNode( ng, "node6", 575, 127, 'SUBTRACT', 0.0, 1.0 )
			self.addMathNode( ng, "node7", 408, -50, 'MULTIPLY', 0.0, 2.0 )
			self.addMathNode( ng, "node8", 576, -52, 'SUBTRACT', 0.0, 1.0 )
			self.addComposeColorNoAlphaNode( ng, "node9", 757, 122 )
			self.addVectorMathNode( ng, "node10", 907, 148, 'NORMALIZE' )
			
			self.addLink( ng, None, "Color", "node2", 0 )
			self.addLink( ng, "node2", 0, "node3", 0 )
			self.addLink( ng, "node2", 1, "node5", 0 )
			self.addLink( ng, "node2", 2, "node7", 0 )
			self.addLink( ng, "node3", 0, "node4", 0 )
			self.addLink( ng, "node4", 0, "node9", 0 )
			self.addLink( ng, "node5", 0, "node6", 0 )
			self.addLink( ng, "node6", 0, "node9", 1 )
			self.addLink( ng, "node7", 0, "node8", 0 )
			self.addLink( ng, "node8", 0, "node9", 2 )
			self.addLink( ng, "node9", 0, "node10", 0 )
			self.addLink( ng, "node10", 0, None, "Normal" )
	
	# create GBuffer VecB2DE node group if not existing
	def createGBufferVecB2DENodeGroup( self ):
		if not "GBuffer VecB2DE" in bpy.data.node_groups:
			ng = bpy.data.node_groups.new( "GBuffer VecB2DE", 'SHADER' )
			self.addInputSocket( ng, "Vector Blender", 'VECTOR', [ 1.0, 0.0, 0.0 ] )
			self.addOutputSocket( ng, "Vector DE", 'VECTOR', [ 1.0, 0.0, 0.0 ] )
			
			self.addSeparateColorNoAlphaNode( ng, "node1", 0, 0 )
			self.addMathNode( ng, "node2", 155, 129, 'SUBTRACT', 0.0 )
			self.addMathNode( ng, "node3", 155, -36, 'SUBTRACT', 0.0 )
			self.addComposeColorNoAlphaNode( ng, "node4", 343, 0 )
			
			self.addLink( ng, None, "Vector Blender", "node1", 0 )
			self.addLink( ng, "node1", 0, "node2", 1 )
			self.addLink( ng, "node1", 2, "node3", 1 )
			self.addLink( ng, "node1", 1, "node4", 2 )
			self.addLink( ng, "node2", 0, "node4", 0 )
			self.addLink( ng, "node3", 0, "node4", 1 )
			self.addLink( ng, "node4", 0, None, "Vector DE" )
	
	# create GBuffer Input node group if not existing
	def createGBufferInputNodeGroup( self ):
		if not "GBuffer Input" in bpy.data.node_groups:
			ng = bpy.data.node_groups.new( "GBuffer Input", 'SHADER' )
			self.addInputSocket( ng, "UV Base", 'VECTOR', [ 0.0, 0.0, 0.0 ] )
			self.addInputSocket( ng, "UV NorXY", 'VECTOR', [ 0.0, 0.0, 0.0 ] )
			self.addInputSocket( ng, "UV NorZTanX", 'VECTOR', [ 0.0, 0.0, 0.0 ] )
			self.addInputSocket( ng, "UV NorTanYZ", 'VECTOR', [ 0.0, 0.0, 0.0 ] )
			self.addOutputSocket( ng, "TexCoord", 'VECTOR', [ 0.0, 0.0, 0.0 ] )
			self.addOutputSocket( ng, "Normal", 'VECTOR', [ 0.0, 0.0, 0.0 ] )
			self.addOutputSocket( ng, "Tangent", 'VECTOR', [ 0.0, 0.0, 0.0 ] )
			
			self.addSeparateColorNoAlphaNode( ng, "node1", 311, 54 )
			self.addSeparateColorNoAlphaNode( ng, "node2", 310, -75 )
			self.addSeparateColorNoAlphaNode( ng, "node3", 310, -188 )
			self.addSeparateColorNoAlphaNode( ng, "node4", 309, -298 )
			self.addMathNode( ng, "node5", 434, 39, 'MULTIPLY', 0.5, 0.5, hide=True )
			self.addMathNode( ng, "node6", 555, 39, 'ADD', 0.5, 0.5, hide=True )
			self.addMathNode( ng, "node7", 432, -1, 'MULTIPLY', 0.5, 0.5, hide=True )
			self.addMathNode( ng, "node8", 552, -1, 'ADD', 0.5, 0.5, hide=True )
			self.addMathNode( ng, "node9", 432, -86, 'MULTIPLY', 0.5, 0.5, hide=True )
			self.addMathNode( ng, "node10", 552, -86, 'ADD', 0.5, 0.5, hide=True )
			self.addMathNode( ng, "node11", 430, -131, 'MULTIPLY', 0.5, 0.5, hide=True )
			self.addMathNode( ng, "node12", 551, -131, 'ADD', 0.5, 0.5, hide=True )
			self.addMathNode( ng, "node13", 426, -198, 'MULTIPLY', 0.5, 0.5, hide=True )
			self.addMathNode( ng, "node14", 547, -198, 'ADD', 0.5, 0.5, hide=True )
			self.addMathNode( ng, "node15", 425, -246, 'MULTIPLY', 0.5, 0.5, hide=True )
			self.addMathNode( ng, "node16", 546, -246, 'ADD', 0.5, 0.5, hide=True )
			self.addMathNode( ng, "node17", 424, -304, 'MULTIPLY', 0.5, 0.5, hide=True )
			self.addMathNode( ng, "node18", 544, -304, 'ADD', 0.5, 0.5, hide=True )
			self.addMathNode( ng, "node19", 421, -348, 'MULTIPLY', 0.5, 0.5, hide=True )
			self.addMathNode( ng, "node20", 542, -348, 'ADD', 0.5, 0.5, hide=True )
			self.addComposeColorNoAlphaNode( ng, "node21", 683, 50 )
			self.addComposeColorNoAlphaNode( ng, "node22", 686, -99 )
			self.addComposeColorNoAlphaNode( ng, "node23", 686, -226 )
			
			self.addLink( ng, None, "UV Base", "node1", 0 )
			self.addLink( ng, None, "UV NorXY", "node2", 0 )
			self.addLink( ng, None, "UV NorZTanX", "node3", 0 )
			self.addLink( ng, None, "UV NorTanYZ", "node4", 0 )
			self.addLink( ng, "node1", 0, "node5", 0 )
			self.addLink( ng, "node1", 1, "node7", 0 )
			self.addLink( ng, "node2", 0, "node9", 0 )
			self.addLink( ng, "node2", 1, "node11", 0 )
			self.addLink( ng, "node3", 0, "node13", 0 )
			self.addLink( ng, "node3", 1, "node15", 0 )
			self.addLink( ng, "node4", 0, "node17", 0 )
			self.addLink( ng, "node4", 1, "node19", 0 )
			self.addLink( ng, "node5", 0, "node6", 0 )
			self.addLink( ng, "node6", 0, "node21", 0 )
			self.addLink( ng, "node7", 0, "node8", 0 )
			self.addLink( ng, "node8", 0, "node21", 1 )
			self.addLink( ng, "node9", 0, "node10", 0 )
			self.addLink( ng, "node10", 0, "node22", 0 )
			self.addLink( ng, "node11", 0, "node12", 0 )
			self.addLink( ng, "node12", 0, "node22", 1 )
			self.addLink( ng, "node13", 0, "node14", 0 )
			self.addLink( ng, "node14", 0, "node22", 2 )
			self.addLink( ng, "node15", 0, "node16", 0 )
			self.addLink( ng, "node16", 0, "node23", 0 )
			self.addLink( ng, "node17", 0, "node18", 0 )
			self.addLink( ng, "node18", 0, "node23", 1 )
			self.addLink( ng, "node19", 0, "node20", 0 )
			self.addLink( ng, "node20", 0, "node23", 2 )
			self.addLink( ng, "node21", 0, None, "TexCoord" )
			self.addLink( ng, "node22", 0, None, "Normal" )
			self.addLink( ng, "node23", 0, None, "Tangent" )
			self.hideNotLinkedNodeOutputs( ng )
	
	# create GBuffer Prepare NTB node group if not existing
	def createGBufferPrepareNTBNodeGroup( self ):
		if not "GBuffer Prepare NTB" in bpy.data.node_groups:
			ng = bpy.data.node_groups.new( "GBuffer Prepare NTB", 'SHADER' )
			self.addInputSocket( ng, "Normal", 'VECTOR', [ 0.0, 1.0, 0.0 ] )
			self.addInputSocket( ng, "Tangent", 'VECTOR', [ 1.0, 0.0, 0.0 ] )
			self.addOutputSocket( ng, "Normal", 'VECTOR', [ 0.0, 0.0, 0.0 ] )
			self.addOutputSocket( ng, "Tangent", 'VECTOR', [ 0.0, 0.0, 0.0 ] )
			self.addOutputSocket( ng, "Bitangent", 'VECTOR', [ 0.0, 0.0, 0.0 ] )
			
			self.addVectorMathNode( ng, "node1", -46, 19, 'NORMALIZE' )
			self.addVectorMathNode( ng, "node2", -46, -139, 'NORMALIZE' )
			self.addVectorMathNode( ng, "node3", 152, -53, 'CROSS_PRODUCT', label="bitangent" )
			self.addVectorMathNode( ng, "node4", 361, -52, 'CROSS_PRODUCT', label="tangent" )
			
			self.addLink( ng, None, "Tangent", "node2", 0 )
			self.addLink( ng, None, "Normal", "node1", 0 )
			self.addLink( ng, "node1", 0, None, "Normal" )
			self.addLink( ng, "node1", 0, "node3", 0 )
			self.addLink( ng, "node2", 0, "node3", 1 )
			self.addLink( ng, "node1", 0, "node4", 1 )
			self.addLink( ng, "node3", 0, None, "Bitangent" )
			self.addLink( ng, "node3", 0, "node4", 0 )
			self.addLink( ng, "node4", 0, None, "Tangent" )
			self.hideNotLinkedNodeOutputs( ng )
	
	# create GBuffer TransformWorld Normal node group if not existing
	def createGBufferTransformWorldNormalNodeGroup( self ):
		if not "GBuffer TransformWorld Normal" in bpy.data.node_groups:
			ng = bpy.data.node_groups.new( "GBuffer TransformWorld Normal", 'SHADER' )
			self.addInputSocket( ng, "Normal", 'VECTOR', [ 0.0, 0.0, 1.0 ] )
			self.addInputSocket( ng, "Tangent", 'VECTOR', [ 1.0, 0.0, 0.0 ] )
			self.addInputSocket( ng, "Bitangent", 'VECTOR', [ 0.0, 1.0, 0.0 ] )
			self.addInputSocket( ng, "World Normal", 'VECTOR', [ 0.0, 0.0, 1.0 ] )
			self.addOutputSocket( ng, "Normal", 'VECTOR', [ 0.0, 0.0, 1.0 ] )
			
			self.addVectorMathNode( ng, "node1", -134, 48, 'DOT_PRODUCT' )
			self.addVectorMathNode( ng, "node2", -135, -110, 'DOT_PRODUCT' )
			self.addVectorMathNode( ng, "node3", -136, -265, 'DOT_PRODUCT' )
			self.addComposeColorNoAlphaNode( ng, "node4", 72, -131 )
			
			self.addLink( ng, None, "Tangent", "node1", 0 )
			self.addLink( ng, None, "World Normal", "node1", 1 )
			self.addLink( ng, None, "Bitangent", "node2", 0 )
			self.addLink( ng, None, "World Normal", "node2", 1 )
			self.addLink( ng, None, "Normal", "node3", 0 )
			self.addLink( ng, None, "World Normal", "node3", 1 )
			self.addLink( ng, "node1", 1, "node4", 0 )
			self.addLink( ng, "node2", 1, "node4", 1 )
			self.addLink( ng, "node3", 1, "node4", 2 )
			self.addLink( ng, "node4", 0, None, "Normal" )
			self.hideNotLinkedNodeOutputs( ng )
	
	# create GBuffer Nor2Clr node group if not existing
	def createGBufferNor2ClrNodeGroup( self ):
		if not "GBuffer Nor2Clr" in bpy.data.node_groups:
			ng = bpy.data.node_groups.new( "GBuffer Nor2Clr", 'SHADER' )
			self.addInputSocket( ng, "Normal", 'VECTOR', [ 0.0, 0.0, 1.0 ] )
			self.addOutputSocket( ng, "Color", 'RGBA', [ 0.5, 0.5, 1.0, 1.0 ] )
			
			self.addSeparateColorNoAlphaNode( ng, "node1", -436, -99 )
			self.addMathNode( ng, "node2", -282, 79, 'MULTIPLY', 0.0, 0.5 )
			self.addMathNode( ng, "node3", -111, 85, 'ADD', 0.0, 0.5 )
			self.addMathNode( ng, "node4", -283, -94, 'MULTIPLY', 0.0, 0.5 )
			self.addMathNode( ng, "node5", -113, -89, 'ADD', 0.0, 0.5 )
			self.addMathNode( ng, "node6", -283, -270, 'MULTIPLY', 0.0, 0.5 )
			self.addMathNode( ng, "node7", -113, -265, 'ADD', 0.0, 0.5 )
			self.addComposeColorNoAlphaNode( ng, "node8", 86, -111 )
			
			self.addLink( ng, None, "Normal", "node1", 0 )
			self.addLink( ng, "node1", 0, "node2", 0 )
			self.addLink( ng, "node1", 1, "node4", 0 )
			self.addLink( ng, "node1", 2, "node6", 0 )
			self.addLink( ng, "node2", 0, "node3", 0 )
			self.addLink( ng, "node3", 0, "node8", 0 )
			self.addLink( ng, "node4", 0, "node5", 0 )
			self.addLink( ng, "node5", 0, "node8", 1 )
			self.addLink( ng, "node6", 0, "node7", 0 )
			self.addLink( ng, "node7", 0, "node8", 2 )
			self.addLink( ng, "node8", 0, None, "Color" )
	
	# create GBuffer Quant 8Bit node group if not existing
	def createGBufferQuant8BitNodeGroup( self ):
		# this shader quantizes a floating point value into an 8-bit integer value. this is required
		# since blender with different versions or platform or graphic card has a non-defined rounding
		# behavior converting rendered images into 8-bit color images while saving. this shader
		# converts the floating point values into values that are correctly converted no matter which
		# rounding policy the actual blender implementation uses. for this the values are rounded down
		# to the nearest 8-bit floating point value and 0.25 in pixel size added
		if not "GBuffer Quant 8Bit" in bpy.data.node_groups:
			ng = bpy.data.node_groups.new( "GBuffer Quant 8Bit", 'SHADER' )
			self.addInputSocket( ng, "Color", 'RGBA', [ 1.0, 1.0, 1.0, 1.0 ] )
			self.addOutputSocket( ng, "Color", 'RGBA', [ 1.0, 1.0, 1.0, 1.0 ] )
			
			self.addSeparateColorNoAlphaNode( ng, "node1", -812, -104 )
			self.addMathNode( ng, "node2", -658, 74, 'MULTIPLY', 0.0, 255.0 )
			self.addMathNode( ng, "node3", -488, 73, 'SUBTRACT', 0.0, 0.5 )
			self.addMathNode( ng, "node4", -323, 73, 'ROUND', 0.0, 1.0 )
			self.addMathNode( ng, "node5", -156, 73, 'DIVIDE', 0.0, 255.0 )
			self.addMathNode( ng, "node6", 0, 73, 'ADD', 0.0, 0.2 / 255.0 )
			self.addMathNode( ng, "node7", -658, -104, 'MULTIPLY', 0.0, 255.0 )
			self.addMathNode( ng, "node8", -488, -105, 'SUBTRACT', 0.0, 0.5 )
			self.addMathNode( ng, "node9", -323, -105, 'ROUND', 0.0, 1.0 )
			self.addMathNode( ng, "node10", -156, -105, 'DIVIDE', 0.0, 255.0 )
			self.addMathNode( ng, "node11", 0, -105, 'ADD', 0.0, 0.2 / 255.0 )
			self.addMathNode( ng, "node12", -658, -287, 'MULTIPLY', 0.0, 255.0 )
			self.addMathNode( ng, "node13", -488, -288, 'SUBTRACT', 0.0, 0.5 )
			self.addMathNode( ng, "node14", -323, -288, 'ROUND', 0.0, 1.0 )
			self.addMathNode( ng, "node15", -156, -288, 'DIVIDE', 0.0, 255.0 )
			self.addMathNode( ng, "node16", 0, -288, 'ADD', 0.0, 0.2 / 255.0 )
			self.addComposeColorNoAlphaNode( ng, "node17", 161, -106 )
			
			self.addLink( ng, None, "Color", "node1", 0 )
			self.addLink( ng, "node1", 0, "node2", 0 )
			self.addLink( ng, "node1", 1, "node7", 0 )
			self.addLink( ng, "node1", 2, "node12", 0 )
			self.addLink( ng, "node2", 0, "node3", 0 )
			self.addLink( ng, "node3", 0, "node4", 0 )
			self.addLink( ng, "node4", 0, "node5", 0 )
			self.addLink( ng, "node5", 0, "node6", 0 )
			self.addLink( ng, "node6", 0, "node17", 0 )
			self.addLink( ng, "node7", 0, "node8", 0 )
			self.addLink( ng, "node8", 0, "node9", 0 )
			self.addLink( ng, "node9", 0, "node10", 0 )
			self.addLink( ng, "node10", 0, "node11", 0 )
			self.addLink( ng, "node11", 0, "node17", 1 )
			self.addLink( ng, "node12", 0, "node13", 0 )
			self.addLink( ng, "node13", 0, "node14", 0 )
			self.addLink( ng, "node14", 0, "node15", 0 )
			self.addLink( ng, "node15", 0, "node16", 0 )
			self.addLink( ng, "node16", 0, "node17", 2 )
			self.addLink( ng, "node17", 0, None, "Color" )
	
	# set up composer nodes. setting use_nodes to True automatically creates and assigns a node
	# tree for us. we just have to fill it in
	def initCompositorNoodle( self, scene ):
		if not scene.use_nodes:
			scene.use_nodes = True
			ng = scene.node_tree
			self.clearNodeGroup( ng )
			
			self.addRenderLayersNode( ng, "node1", -207, 376 )
			self.addGroupNode( ng, "node2", 101, 303, "sRGB to Linear Color" )
			self.addCompositeNode( ng, "node3", 382, 349 )
			
			self.addLink( ng, "node1", 0, "node2", 0 )
			self.addLink( ng, "node2", 0, "node3", 0 )
			self.addLink( ng, "node1", 1, "node3", 1 )
	
	# create the gbuffer screen. set it as the active one to have the right context for operators
	def createScreen( self, context, screenName ):
		if screenName in bpy.data.screens:
			screen = bpy.data.screens[ screenName ]
			context.screen = screen
		else:
			bpy.ops.screen.new()
			screen = bpy.context.screen
			screen.name = screenName
		
		return screen
	
	# create the gbuffer scene. set it as the active one to have the right context for operators
	def createScene( self, context, sceneName ):
		"""
		if sceneName in bpy.data.scenes:
			scene = bpy.data.scenes[ sceneName ]
			objects = scene.objects[:]
			for obj in objects:
				scene.objects.unlink( obj )
		else:
			scene = bpy.data.scenes.new( sceneName )
		context.screen.scene = scene
		"""
		bpy.ops.scene.new( type='NEW' )
		scene = context.scene
		scene.name = sceneName
		
		scene.render.resolution_x = 1024
		scene.render.resolution_y = 1024
		scene.render.resolution_percentage = 100
		scene.render.use_antialiasing = False
		scene.render.use_sequencer = True
		scene.render.use_compositing = True
		
		self.initCompositorNoodle( scene )
		
		return scene
	
	# create camera for rendering
	def createAndAddCameraObject( self, scene, cameraName ):
		if cameraName in bpy.data.cameras:
			camera = bpy.data.cameras[ cameraName ]
		else:
			camera = bpy.data.cameras.new( cameraName )
		
		camera.type = 'ORTHO'
		camera.ortho_scale = 1.0
		
		if cameraName in bpy.data.objects:
			objCamera = bpy.data.objects[ cameraName ]
			objCamera.data = camera
		else:
			objCamera = bpy.data.objects.new( cameraName, camera )
		
		objCamera.location = Vector( ( 0.0, -2.0, 0.0 ) )
		objCamera.rotation_euler = Euler( ( 1.570796251296997, 0.0, 0.0 ), 'XYZ' )
		
		scene.objects.link( objCamera )
		scene.objects.active = objCamera
		
		scene.camera = objCamera
		
		bpy.ops.view3d.viewnumpad( type='CAMERA' )
		
		return objCamera
	
	# create texture with an image
	def createTextureWithImage( self, image, textureName ):
		texture = bpy.data.textures.new( textureName, 'IMAGE' )
		texture.image = image
		texture.use_mipmap = False
		texture.use_alpha = False
		texture.extension = 'EXTEND'
		return texture
	
	# create normal generation material for an image
	def createMaterialWithImage( self, image, materialName ):
		texture = bpy.data.textures.new( materialName, 'IMAGE' )
		texture.image = image
		texture.use_mipmap = False
		texture.use_alpha = False
		texture.extension = 'EXTEND'
		
		material = bpy.data.materials.new( materialName )
		material.use_nodes = True
		ng = material.node_tree
		self.clearNodeGroup( ng )
		
		self.addGeometryNode( ng, "node1", -249, 405, "base", hide=True )
		self.addGeometryNode( ng, "node2", -250, 362, "norXY", hide=True )
		self.addGeometryNode( ng, "node3", -251, 315, "norZtanX", hide=True )
		self.addGeometryNode( ng, "node4", -253, 272, "tanYZ", hide=True )
		self.addGroupNode( ng, "node5", -83, 446, "GBuffer Input" )
		self.addGroupNode( ng, "node6", 149, 443, "GBuffer Prepare NTB" )
		self.addGeometryNode( ng, "node7", -438, 95, "base", hide=True )
		self.addTextureNode( ng, "node8", -278, 171, texture )
		self.addGroupNode( ng, "node9", -86, 171, "GBuffer World Normal" )
		self.addGroupNode( ng, "node10", 146, 174, "GBuffer VecB2DE" )
		self.addGroupNode( ng, "node11", 416, 440, "GBuffer TransformWorld Normal" )
		self.addGroupNode( ng, "node12", 622, 420, "GBuffer Nor2Clr" )
		self.addGroupNode( ng, "node13", 845, 420, "GBuffer Quant 8Bit" )
		self.addOutputNode( ng, "node14", 1051, 442 )
		
		self.addLink( ng, "node1", 4, "node5", 0 )
		self.addLink( ng, "node2", 4, "node5", 1 )
		self.addLink( ng, "node3", 4, "node5", 2 )
		self.addLink( ng, "node4", 4, "node5", 3 )
		self.addLink( ng, "node5", 1, "node6", 0 )
		self.addLink( ng, "node5", 2, "node6", 1 )
		self.addLink( ng, "node6", 0, "node11", 0 )
		self.addLink( ng, "node6", 1, "node11", 1 )
		self.addLink( ng, "node6", 2, "node11", 2 )
		self.addLink( ng, "node7", 4, "node8", 0 )
		self.addLink( ng, "node8", 1, "node9", 0 )
		self.addLink( ng, "node9", 0, "node10", 0 )
		self.addLink( ng, "node10", 0, "node11", 3 )
		self.addLink( ng, "node12", 0, "node13", 0 )
		self.addLink( ng, "node11", 0, "node12", 0 )
		self.addLink( ng, "node13", 0, "node14", 0 )
		self.hideNotLinkedNodeOutputs( ng )
		
		return material
	
	# create a new mesh to work with. make it active to have the right context for operators
	def createAndAddMeshObject( self, scene, meshName, material ):
		mesh = bpy.data.meshes.new( meshName )
		objMesh = bpy.data.objects.new( meshName, mesh )
		
		objMesh.location = Vector( ( -0.5, 0.0, -0.5 ) )
		
		scene.objects.link( objMesh )
		scene.objects.active = objMesh
		objMesh.select = True
		
		bpy.ops.object.material_slot_add()
		objMesh.material_slots[ 0 ].material = material
		
		return objMesh
	
	# make sure the mesh is clean
	def clearMesh( self, mesh ):
		while mesh.uv_layers.active:
			bpy.ops.mesh.uv_texture_remove()
	
	# add vertices and faces as in the original mesh but mapping them to their texture coordinates.
	# for this the position of each face vertex becomes (uv.u, 0, uv.v). hence the position of the
	# vertex as seen from the front side becomes it's texture coordinates. this way during rendering
	# we obtain exatly the same texture map as defined by the vertex texture coordinates but allowing
	# us to alter the output using data stored in additional texture coordinates. if a face has no
	# image assigned move it outside of the screen so it is not rendered
	def addMeshVertices( self, orgMesh, uvlOrg, uvtOrg, gbMesh ):
		"""
		vertices = []
		for i in range( len( orgMesh.vertices ) ):
			vertices.append( orgMesh.vertices[ i ].co )
		faces = []
		for polygon in orgMesh.polygons:
			faces.append( polygon.vertices )
		gbMesh.from_pydata( vertices, [], faces )
		vertices = None
		faces = None
		"""
		"""
		vertices = []
		for i in range( len( orgMesh.loops ) ):
			uv = uvlOrg[ i ].uv
			vertices.append( ( uv[ 0 ], 0.0, uv[ 1 ] ) )
		faces = []
		for polygon in orgMesh.polygons:
			faces.append( [ index for index in polygon.loop_indices ] )
		gbMesh.from_pydata( vertices, [], faces )
		vertices = None
		faces = None
		"""
		
		gbMesh.vertices.add( len( orgMesh.loops ) )
		gbMesh.loops.add( len( orgMesh.loops ) )
		for i in range( len( orgMesh.loops ) ):
			uv = uvlOrg[ i ].uv
			gbMesh.vertices[ i ].co = Vector( ( uv[ 0 ], 0.0, uv[ 1 ] ) )
			gbMesh.loops[ i ].vertex_index = i
		gbMesh.polygons.add( len( orgMesh.polygons ) )
		for i in range( len( orgMesh.polygons ) ):
			orgPolygon = orgMesh.polygons[ i ]
			gbPolygon = gbMesh.polygons[ i ]
			gbPolygon.loop_start = orgPolygon.loop_start
			gbPolygon.loop_total = orgPolygon.loop_total
			if not uvtOrg[ i ].image:
				for index in orgPolygon.loop_indices:
					gbMesh.vertices[ index ].co = Vector( ( -1.0, 0.0, -1.0 ) )
	
	# add the base uv layer
	def addBaseUVLayer( self, orgMesh, uvlOrg, uvtOrg, gbMesh, uvLayerName ):
		bpy.ops.mesh.uv_texture_add()
		gbMesh.uv_textures.active.name = uvLayerName
		uvlBase = gbMesh.uv_layers.active.data
		uvtBase = gbMesh.uv_textures.active.data
		
		# set base uv layer with the same texture as the original mesh and the same texture coordinates.
		# set the normal and tangent uv maps with the calculated normal and tangent. if an 
		for i in range( len( orgMesh.polygons ) ):
			uvtBase[ i ].image = uvtOrg[ i ].image
			for j in orgMesh.polygons[ i ].loop_indices:
				uvlBase[ j ].uv = uvlOrg[ j ].uv
	
	# add the normal and tangent uv layers
	def addNormalTangentUVLayers( self, orgMesh, uvlOrg, uvtOrg, gbMesh, workMesh, uvLayerName1, uvLayerName2, uvLayerName3 ):
		bpy.ops.mesh.uv_texture_add()
		gbMesh.uv_textures.active.name = uvLayerName1
		uvlNorXY = gbMesh.uv_layers.active.data
		
		bpy.ops.mesh.uv_texture_add()
		gbMesh.uv_textures.active.name = uvLayerName2
		uvlNorZTanX = gbMesh.uv_layers.active.data
		
		bpy.ops.mesh.uv_texture_add()
		gbMesh.uv_textures.active.name = uvLayerName3
		uvlTanYZ = gbMesh.uv_layers.active.data
		
		for face in workMesh.faces:
			loopStart = face.face.loop_start
			for j in range( face.face.loop_total ):
				normal = workMesh.realNormals[ face.normals[ j ] ]
				tangent = workMesh.realTangents[ face.tangents[ j ] ]
				vindex = loopStart + j
				uvlNorXY[ vindex ].uv = ( normal[ 0 ], normal[ 1 ] )
				uvlNorZTanX[ vindex ].uv = ( normal[ 2 ], tangent[ 0 ] )
				uvlTanYZ[ vindex ].uv = ( tangent[ 1 ], tangent[ 2 ] )
	
	# create the working mesh class for the selected object
	def createWorkMesh( self, objMesh ):
		mesh = Mesh( objMesh )
		
		mesh.initAddTextures()
		mesh.initAddTexCoordSets()
		mesh.initAddVertices()
		mesh.initAddEdges()
		mesh.initAddFaces()
		mesh.initCalcCornerNormals()
		mesh.initCalcCornerTangents()
		mesh.initCalcInfoNumbers()
		
		if mesh.multiFoldMesh:
			self.report( { 'INFO', 'ERROR' }, "Can not Multi-Fold meshes (Edges used by more than 2 faces)." )
			return None
		if mesh.degeneratedFaces:
			self.report( { 'INFO', 'ERROR' }, "Degenerated Faces found." )
			return None
		if not mesh.texCoordSets:
			self.report( { 'INFO', 'ERROR' }, "No UV-Layers found. Add at least one UV-Layer." )
			return None
		
		mesh.initCalcRealVertexPositions( transformPosition )
		mesh.initCalcRealFaceTexCoords()
		mesh.initCalcRealFaceNormalsAndTangents()
		
		return mesh
	
	@classmethod
	def poll( cls, context ):
		return context.active_object != None and context.active_object.type == 'MESH' and context.active_object.data.uv_layers.active
	
	def execute( self, context ):
		# some defaults
		SCREEN_GBUFFER = "GBuffer"
		SCENE_GBUFFER = "GBuffer"
		CAMERA_GBUFFER = "GBuffer Camera"
		MESH_GBUFFER = "GBuffer Mesh"
		UVMAP_BASE = "base"
		UVMAP_NORMALXY = "norXY"
		UVMAP_NORMALZ_TANGENTX = "norZtanX"
		UVMAP_TANGENTYZ = "tanYZ"
		
		# create working mesh. only continue if nothing is wrong
		workMesh = self.createWorkMesh( context.active_object )
		
		if workMesh:
			# preparations
			editmode = self.initEditMode( context )
			
			# put aside the pointers to the original mesh to work with
			orgMesh = context.active_object.data
			uvlOrg = orgMesh.uv_layers.active.data
			uvtOrg = orgMesh.uv_textures.active.data
			
			# create node groups
			self.createGammaFixNodeGroup()
			self.createGammaFixColorNodeGroup()
			self.createGBufferWorldNormalNodeGroup()
			self.createGBufferVecB2DENodeGroup()
			self.createGBufferInputNodeGroup()
			self.createGBufferPrepareNTBNodeGroup()
			self.createGBufferTransformWorldNormalNodeGroup()
			self.createGBufferNor2ClrNodeGroup()
			self.createGBufferQuant8BitNodeGroup()
			
			# create material with the first image found in the original mesh assigned to textures
			material = None
			for face in uvtOrg:
				if face.image:
					material = self.createMaterialWithImage( face.image, 'GBuffer NormGen' )
					break
			
			# create screen, scene and with all the required objects
			#gbScreen = self.createScreen( context, SCREEN_GBUFFER )
			gbScene = self.createScene( context, SCENE_GBUFFER )
			gbObjCamera = self.createAndAddCameraObject( gbScene, CAMERA_GBUFFER )
			gbObjMesh = self.createAndAddMeshObject( gbScene, MESH_GBUFFER, material )
			gbMesh = gbObjMesh.data
			
			self.clearMesh( gbMesh )
			self.addMeshVertices( orgMesh, uvlOrg, uvtOrg, gbMesh )
			self.addBaseUVLayer( orgMesh, uvlOrg, uvtOrg, gbMesh, UVMAP_BASE )
			self.addNormalTangentUVLayers( orgMesh, uvlOrg, uvtOrg, gbMesh, workMesh, UVMAP_NORMALXY, UVMAP_NORMALZ_TANGENTX, UVMAP_TANGENTYZ )
			
			# update the mesh
			gbMesh.uv_textures.active = gbMesh.uv_textures[ UVMAP_BASE ]
			
			gbMesh.update( True, True )
			gbMesh.calc_normals()
			gbMesh.validate( True )
			
			# to avoid potential problems remove all doubles. this way we get texture coordinate space
			# connectivity for free
			"""
			bpy.ops.object.mode_set( mode='EDIT' )
			bpy.ops.mesh.remove_doubles( mergedist=0.0001, use_unselected=True )
			bpy.ops.object.mode_set( mode='OBJECT' )
			"""
			
			# clean up
			if editmode:
				bpy.ops.object.mode_set( mode='EDIT' )
		
		return { 'FINISHED' }
registerClass(OBJECT_OT_ToolGBufferNormGen)
