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
import mathutils

from mathutils import Vector, Matrix
from operator import attrgetter

from .de_math import vector_by_matrix, matrixToEuler, convertEuler, convertMatrix, convertMatrixBone, Octree
from .de_math import vecCross, vecSub, vecMul, vecDot, vecNeg, vecDiv, vecLength, vecAdd, vec2Sub
from .de_math import transformPosition, transformBonePosition
from .de_porting import matmul


class Armature:
    class Bone:
        def __init__(self, index, bone):
            self.index = index
            self.bone = bone
            self.name = bone.name
            self.parent = None
            self.pos = Vector((0.0, 0.0, 0.0))
            self.rot = Vector((0.0, 0.0, 0.0))
            self.restMat = Matrix(((1.0, 0.0, 0.0, 0.0), (0.0, 1.0, 0.0, 0.0), (0.0, 0.0, 1.0, 0.0), (0.0, 0.0, 0.0, 1.0)))
        
        def __repr__(self):
            return "[%i %s]" % (self.index, self.name)
        
        def __str__(self):
            return self.__repr__()
    
    class Keyframe:
        def __init__(self, time, pos, rot, scale):
            self.time = time
            self.pos = pos
            self.rot = rot
            self.scale = scale
    
    class KeyframeVps:
        def __init__(self, time, weight):
            self.time = time
            self.weight = weight
        
        def __repr__(self):
            return "[time=%i pos=%s rot=%s scale=%s]" % (self.time, str(self.pos), str(self.rot), str(self.scale))
        
        def __str__(self):
            return self.__repr__()
    
    class MoveBone:
        def __init__(self, bone):
            self.bone = bone
            self.times = []
            self.keyframes = []
            self.used = False
    
    class MoveVps:
        def __init__(self, vps):
            self.vps = vps
            self.times = []
            self.keyframes = []
            self.hasWeight = False
            self.datapath = "key_blocks[\"{}\"].value".format(vps.name)
            self.fcurve = None
        
        def findFCurve(self, fcurves):
            found = [x for x in fcurves if x.data_path == self.datapath]
            if found:
                self.fcurve = found[0]
    
    class Move:
        def __init__(self, action):
            self.action = action
            self.name = action.dragengine_exportname or action.name
            self.firstFrame = action.dragengine_rangebegin
            self.lastFrame = action.dragengine_rangeend
            self.automaticRange = action.dragengine_autorange
    
    class CollisionVolume:
        def __init__(self, object, scalePosition):
            self.object = object
            self.bone = None
            self.topRadiusScale = object.dragengine_shapetopradiusscale
            self.bottomRadiusScale = object.dragengine_shapebottomradiusscale
            self.unitRadii = math.fabs(self.topRadiusScale - 1) < 0.001 and math.fabs(self.bottomRadiusScale - 1) < 0.001
            
            self.matrix = object.matrix_world
            if object.parent:
                if object.parent.type == 'ARMATURE':
                    self.matrix = convertMatrixBone(matmul(object.parent.matrix_world.inverted(), self.matrix))
                    if object.parent_bone:
                        self.matrix = matmul(
                            convertMatrixBone(object.parent.pose.bones[object.parent_bone].matrix).inverted(),
                            self.matrix)
                        self.bone = object.parent_bone
                else:
                    self.matrix = convertMatrix(matmul(object.parent.matrix_world.inverted(), self.matrix))
            
            self.setMatrix(self.matrix, scalePosition)
            
        def setMatrix(self, matrix, scalePosition):
            self.matrix = matrix
            
            (loc,rot,scale) = self.matrix.decompose()
            scale = Vector([self.object.dimensions[0], self.object.dimensions[2], self.object.dimensions[1]])
            self.matrix = matmul(Matrix.Translation(loc), rot.to_matrix().to_4x4())
            
            self.position = vector_by_matrix(matmul(scalePosition, self.matrix), Vector())
            self.rotation = matrixToEuler(self.matrix)
            
            self.scaling = vector_by_matrix(scalePosition, scale)
            self.scaling = Vector([math.fabs(self.scaling.x), math.fabs(self.scaling.y), math.fabs(self.scaling.z)])
    
    class ConstraintAxis:
        def __init__(self):
            self.limitLower = 0
            self.limitUpper = 0
            self.staticFriction = 0
            self.dynamicFriction = 0
            self.springStiffness = 0
    
    # constructor
    def __init__(self, object):
        self.object = object
        self.armature = object.data
        self.bones = []
        self.mapBones = {}
        self.ignoreBones = [re.compile(f.name) for f in object.data.dragengine_bonefilters]
        self.restPose = object.data.pose_position == 'REST'
        
        self.thresholdPosition = object.data.dragengine_thresholdpos
        self.thresholdRotation = object.data.dragengine_thresholdrot
        self.thresholdScale = object.data.dragengine_thresholdscale
        self.thresholdWeight = 0.05
        self.shortFormat = object.data.dragengine_shortformat
    
    # add bones
    def initAddBones(self):
        # add bones in the correct order. for this we simply add
        # all bones without a parent with the recursive helper method
        for bone in self.armature.bones.values():
            if not bone.parent:
                self.initAddBonesHelper(bone, None)
    
    # add bones helper function
    def initAddBonesHelper(self, bone, parent):
        # check if the bone has to be ignore
        for f in self.ignoreBones:
            if f.match(bone.name):
                return
        
        # add bone itself
        boneIndex = len(self.bones)
        newBone = Armature.Bone(boneIndex, bone)
        newBone.parent = parent
        self.bones.append(newBone)
        self.mapBones[bone.name] = newBone
        
        # add all children bones in a depth search fashion
        for child in bone.children:
            self.initAddBonesHelper(child, newBone)
    
    # print infos about the armature
    def printInfos(self):
        print("*** Armature ***")
        print("name:     {}".format(self.object.name))
        print("bones:    {}".format(len(self.bones)))



class Mesh:
    class Texture:
        def __init__(self, index, material, textureSlot):
            self.index = index
            self.material = material
            self.name = material.name
            self.exportName = material.dragengine_exportname
            if self.exportName == "":
                self.exportName = material.name
            self.navtype = material.dragengine_navtype
            self.textureSlot = textureSlot
            self.image = None
            self.width = 1024
            self.height = 1024
            if textureSlot and textureSlot.texture and textureSlot.texture.type == 'IMAGE':
                self.image = textureSlot.texture.image
                if self.image:
                    size = self.image.size
                    if size[0] > 0:
                        self.width = size[0]
                    if size[1] > 0:
                        self.height = size[1]
        
        def __repr__(self):
            return "[%s]" % (self.name)
        
        def __str__(self):
            return self.__repr__()
    
    class TexCoordSet:
        def __init__(self, index, texlayer, uvlayer):
            self.index = index
            self.texlayer = texlayer
            self.uvlayer = uvlayer
            self.name = texlayer.name if texlayer else uvlayer.name
            self.texCoords = []
        
        def __repr__(self):
            return "[%s]" % (self.name)
        
        def __str__(self):
            return self.__repr__()
    
    class TexCoord:
        def __init__(self, uv, invTCEquals):
            self.index = 0
            self.uv = uv
            self.compareTexCoords = tuple([int((x * invTCEquals) + 0.5) for x in uv])
            self.hashed = hash(self.compareTexCoords)
        
        def __eq__(self, other):
            return self.compareTexCoords == other.compareTexCoords
        
        def __hash__(self):
            return self.hashed
        
        def __repr__(self):
            return self.uv.__repr__()
        
        def __str__(self):
            return self.uv.__str__()
    
    class VertexPositionSet:
        def __init__(self, index, shapeKey, name=None):
            self.index = index
            self.shapeKey = shapeKey
            self.name = shapeKey.name if shapeKey else name
            self.baseSet = None
            self.positions = []
        
        def __repr__(self):
            return "[%s]" % (self.name)
        
        def __str__(self):
            return self.__repr__()
    
    class VertexPositionSetPosition:
        def __init__(self, vertex, position):
            self.vertex = vertex
            self.position = position
        
        def __repr__(self):
            return "[%s]" % (self.name)
        
        def __str__(self):
            return self.__repr__()
    
    class Weight:
        def __init__(self, bone, weight):
            self.bone = bone
            self.weight = weight
            self.compareWeight = int((weight * 1000) + 0.5)
        
        def __eq__(self, other):
            return self.bone == other.bone and self.compareWeight == other.compareWeight
        
        def __repr__(self):
            return "[%s=%g]" % (self.bone, self.weight)
        
        def __str__(self):
            return self.__repr__()
    
    class Weights:
        def __init__(self):
            self.weights = []
            self.index = -1
            self.hashed = 0
            
        ## Normalize, sort by descending weight, limit to weight count and calculate hash
        def normalize(self, maxWeights):
            self.weights = sorted(self.weights, key=attrgetter("weight"), reverse=True)[0:maxWeights]
            weightSum = sum([w.weight for w in self.weights])
            if weightSum < 0.001:
                self.weights = []
                self.hashed = 0
            else:
                factor = 1.0 / weightSum
                for weight in self.weights:
                    weight.weight = weight.weight * factor
                self.hashed = hash(tuple([a for b in [(w.bone.index, w.compareWeight) for w in self.weights] for a in b]))
        
        def __hash__(self):
            return self.hashed
        
        def __eq__(self, other):
            if len(self.weights) != len(other.weights):
                return False
            for i in range(len(self.weights)):
                if self.weights[i] != other.weights[i]:
                    return False
            return True
            
        def __repr__(self):
            return self.weights.__repr__()
        
        def __str__(self):
            return self.__repr__()
    
    class Vertex:
        def __init__(self, index, vertex):
            self.vertex = vertex
            self.edges = []
            self.faces = []
            self.index = index
            self.weights = Mesh.Weights()
        
        def __repr__(self):
            return "[%i: edges(%s) faces(%s)]" % (self.index, str(self.edges), str(self.faces))
        
        def __str__(self):
            return self.__repr__()
        
        def findEdge(self, edges, v2):
            for edge in self.edges:
                if edges[edge].vertices[1] == v2:
                    return edge
            return -1
        
        def getHardEdgeCount(self, edges):
            return sum(1 for edge in self.edges if edges[edge].hard)
        
        def getUVSeamCount(self, edges):
            return sum(1 for edge in self.edges if edges[edge].uvseam)
    
    class Edge:
        def __init__(self, index, v1, v2):
            self.vertices = [v1, v2]
            self.faces = [-1, -1]
            self.index = index
            self.hard = False
            self.uvseam = False
            self.navtype = 0
        
        def __repr__(self):
            return "[%i: vertices=(%i,%i) hard=%i uvseam=%i faces=(%s)]" % (self.index,
                self.vertices[0], self.vertices[1], self.hard, self.uvseam, str(self.faces))
        
        def __str__(self):
            return self.__repr__()
    
    class Face:
        def __init__(self, index, face, meshLoops):
            count = len(face.loop_indices)
            self.face = face
            self.vertices = [meshLoops[x].vertex_index for x in face.loop_indices]
            self.normals = [-1] * count
            self.tangents = [-1] * count
            self.edges = [-1] * count
            self.index = index
            self.texture = None
            self.texCoordSets = []
        
        def __repr__(self):
            return "[%i: vertices(%s) normals(%s) tangents(%s) edges(%s)]" % (self.index,
                str(self.vertices), str(self.normals), str(self.tangents), str(self.edges))
        
        def __str__(self):
            return self.__repr__()
        
        def findCorner(self, vertex):
            for index in range(len(self.vertices)):
                if self.vertices[index] == vertex:
                    return index
            return -1
        
        def setNormalFor(self, vertex, normal):
            for index in range(len(self.vertices)):
                if self.vertices[index] == vertex:
                    self.normals[index] = normal
        
        def setTangentFor(self, vertex, tangent):
            for index in range(len(self.vertices)):
                if self.vertices[index] == vertex:
                    self.tangents[index] = tangent
    
    # constructor
    def __init__(self, object):
        self.object = object
        self.mesh = object.data
        self.armature = None
        self.weights = []
        self.mapWeights = {}
        self.vertices = []
        self.edges = []
        self.faces = []
        self.triangles = []
        self.quads = []
        self.ngons = []
        self.degeneratedFaces = []
        self.textures = []
        self.texCoordSets = []
        self.vertPosSets = []
        self.normalCount = 0
        self.tangentCount = 0
        self.maxBoneWeightCount = 0
        self.multiFoldMesh = False
        self.lodMesh = None
        self.lodError = None
        self.navspacetype = object.dragengine_navspacetype
        self.useAutoSmooth = False
        self.autoSmoothAngle = 180.0
        self.edgeSplitModifier = None
        self.autoSmoothModifier = None

    # add textures
    def initAddTextures(self):
        for material in self.mesh.materials:
            textureSlot = None
            if hasattr(material, "texture_slots") and len(material.texture_slots) > 0:
                textureSlot = material.texture_slots[0]
            self.textures.append(Mesh.Texture(len(self.textures), material, textureSlot))
    
    # add texture coordinates sets
    def initAddTexCoordSets(self):
        if hasattr(self.mesh, "uv_textures"):
            if not self.mesh.uv_textures:
                raise 'At least 1 UV-Texture required'
            if len(self.mesh.uv_textures) == 1:
                self.texCoordSets.append(Mesh.TexCoordSet(
                    0, self.mesh.uv_textures[0], self.mesh.uv_layers[0]))
            else:
                for i in range(len(self.mesh.uv_textures)):
                    if self.mesh.uv_textures[i].name == "default":
                        self.texCoordSets.append(Mesh.TexCoordSet(
                            0, self.mesh.uv_textures[i], self.mesh.uv_layers[i]))
                for i in range(len(self.mesh.uv_textures)):
                    if self.mesh.uv_textures[i].name != "default":
                        self.texCoordSets.append(Mesh.TexCoordSet(len(self.texCoordSets),
                            self.mesh.uv_textures[i], self.mesh.uv_layers[i]))
        else:
            if not self.mesh.uv_layers:
                raise 'At least 1 UV-Layer required'
            if len(self.mesh.uv_layers) == 1:
                self.texCoordSets.append(Mesh.TexCoordSet(0, None, self.mesh.uv_layers[0]))
            else:
                for i in range(len(self.mesh.uv_layers)):
                    if self.mesh.uv_layers[i].name == "default":
                        self.texCoordSets.append(Mesh.TexCoordSet(0, None, self.mesh.uv_layers[i]))
                for i in range(len(self.mesh.uv_layers)):
                    if self.mesh.uv_layers[i].name != "default":
                        self.texCoordSets.append(Mesh.TexCoordSet(len(self.texCoordSets),
                            None, self.mesh.uv_layers[i]))
    
    # add vertex position sets
    def initAddVertPosSets(self, baseMesh=None):
        if not self.mesh.shape_keys:
            if baseMesh:
                for vps in baseMesh.vertPosSets:
                    self.vertPosSets.append(Mesh.VertexPositionSet(len(self.vertPosSets), None, vps.name))
            return
        
        if baseMesh:
            for vps in baseMesh.vertPosSets:
                shapeKey = [x for x in self.mesh.shape_keys.key_blocks if x.name == vps.name]
                if shapeKey and len(shapeKey) > 0:
                    self.vertPosSets.append(Mesh.VertexPositionSet(len(self.vertPosSets), shapeKey[0]))
                else:
                    self.vertPosSets.append(Mesh.VertexPositionSet(len(self.vertPosSets), None, vps.name))
        else:
            # skip first shape key as this is the base shape key.
            # fun fact: the first shape key has itself as relative key
            for shapeKey in self.mesh.shape_keys.key_blocks[1:]:
                if shapeKey and shapeKey.relative_key != shapeKey:
                    self.vertPosSets.append(Mesh.VertexPositionSet(len(self.vertPosSets), shapeKey))
            
        for vps in self.vertPosSets:
            if not vps.shapeKey:
                continue
            
            found = [x for x in self.vertPosSets if x.shapeKey == vps.shapeKey.relative_key]
            if found:
                vps.baseSet = found[0]
            
            p1 = vps.shapeKey.data
            p2 = vps.shapeKey.relative_key.data
            for i in range(len(p1)):
                # ignore vertices not having changed
                if (p1[i].co - p2[i].co).length < 0.001:
                    continue
                
                vps.positions.append(Mesh.VertexPositionSetPosition(i, p1[i].co))
    
    # add vertices
    def initAddVertices(self):
        # add vertices
        for vertex in self.mesh.vertices:
            self.vertices.append(Mesh.Vertex(vertex.index, vertex))
        
        # add weights only if there is an armature
        if self.armature:
            vgroups = self.object.vertex_groups
            mapBones = self.armature.mapBones
            
            for vertex in self.vertices:
                maxWeights = self.object.dragengine_maxweights
                # first add all weights as found in the vertex
                # 
                # WARNING blender has a bug that can cause data inconsistency. the group.group
                #         index can be larger or equal to the length of vgroups which is wrong.
                #         we use a try catch to protect against this problem ignoring such
                #         broken weights. so far it is totally unknown how such a data
                #         inconsistency can happen but when it does the exporter breaks
                vw = vertex.weights.weights
                for group in vertex.vertex.groups:
                    try:
                        bone = mapBones.get(vgroups[group.group].name)
                        if bone and group.weight > 0.001:
                            vw.append(Mesh.Weight(bone, group.weight))
                    except Exception as e:
                        print('BLENDER BUG! Data inconsistency caught: group.group={} len(vgroups)={}'.format(
                            group.group, len(vgroups)))
                # normalize weights, sort weights and calculate hash value
                vertex.weights.normalize(maxWeights)
            
            # group weights together eliminating duplicates
            for vertex in self.vertices:
                if not vertex.weights:
                    continue
                weights = self.mapWeights.get(vertex.weights)
                if weights:
                    vertex.weights = weights
                else:
                    vertex.weights.index = len(self.weights)
                    self.weights.append(vertex.weights)
                    self.mapWeights[vertex.weights] = vertex.weights
        
        # determine the maximum number of bones used in all weights
        if self.weights:
            self.maxBoneWeightCount = max([len(w.weights) for w in self.weights])

    # add edges
    def initAddEdges(self):
        beforeB40 = bpy.app.version < (4, 0)
        for edge in self.mesh.edges:
            newEdge = Mesh.Edge(edge.index, edge.vertices[0], edge.vertices[1])
            if beforeB40:
                newEdge.crease = edge.crease
            else:
                ec = self.mesh.edge_creases
                if ec:
                    newEdge.crease = ec.data[edge.index].value
            newEdge.sharp = edge.use_edge_sharp
            newEdge.uvseam = edge.use_seam
            if newEdge.sharp:  # or (False and newEdge.crease > 0.5):
                newEdge.hard = True
            # if self.object.dragengine_splitseam and newEdge.uvseam:
            #    newEdge.hard = True
            self.edges.append(newEdge)
            self.vertices[newEdge.vertices[0]].edges.append(newEdge.index)
            self.vertices[newEdge.vertices[1]].edges.append(newEdge.index)

    # add faces
    def initAddFaces(self):
        textureCount = len(self.textures)
        for face in self.mesh.polygons:
            newFace = Mesh.Face(face.index, face, self.mesh.loops)
            self.faces.append(newFace)
            
            vcount = len(face.vertices)
            if vcount == 3:
                self.triangles.append(newFace)
            elif vcount == 4:
                self.quads.append(newFace)
            elif vcount > 4:
                self.ngons.append(newFace)
            else:
                self.degeneratedFaces.append(newFace)
            
            # assign texture if possible
            if face.material_index >= 0 and face.material_index < textureCount:
                newFace.texture = self.textures[face.material_index]
            # gather vertices and indices
            vi = newFace.vertices
            vo = [self.vertices[x] for x in vi]
            # corners
            count = len(vi)
            for c in range(count):
                self.initProcessFaceCorner(vi, vo, c, (c + 1) % count, newFace.index)
    
    # init face corner
    def initProcessFaceCorner(self, vi, vo, c1, c2, fi):
        vo[c1].faces.append(fi)
        edge = vo[c1].findEdge(self.edges, vi[c2])
        if edge == -1:
            edge = vo[c2].findEdge(self.edges, vi[c1])
        theEdge = self.edges[edge]
        if theEdge.faces[0] == -1:
            theEdge.faces[0] = fi
        elif theEdge.faces[1] == -1:
            theEdge.faces[1] = fi
        else:
            print(fi, theEdge.faces)
            self.multiFoldMesh = True
            self.mesh.edges[edge].select = True
        self.faces[fi].edges[c1] = edge

    # init auto smoothing
    def initAutoSmooth(self):
        if bpy.app.version < (4, 1):
            if self.mesh.use_auto_smooth:
                self.useAutoSmooth = True
                self.autoSmoothAngle = self.mesh.auto_smooth_angle

            for m in self.object.modifiers:
                if m.type != 'EDGE_SPLIT':
                    continue
                self.edgeSplitModifier = m
                break

        else:
            for m in self.object.modifiers:
                if m.type != 'NODES':
                    continue
                if not m.node_group:
                    continue
                if m.node_group.name != 'Smooth by Angle':
                    continue
                self.useAutoSmooth = True
                self.autoSmoothAngle = m["Input_1"]
                self.autoSmoothModifier = m
                break

    # apply auto smoothing
    def applyAutoSmooth(self):
        if not self.useAutoSmooth:
            return
        angle = self.useAutoSmooth
        if angle > 179.9:
            return

        for edge in self.edges:
            if edge.faces[0] != -1 and edge.faces[1] != -1:
                f1 = self.faces[edge.faces[0]]
                f2 = self.faces[edge.faces[1]]
                if f1.face.normal.angle(f2.face.normal, 0) > angle:
                    edge.sharp = True
                    edge.hard = True

    # calculate corner normals
    def initCalcCornerNormals(self):
        self.normalCount = 0
        for vertex in self.vertices:
            hardEdgeCount = vertex.getHardEdgeCount(self.edges)
            if hardEdgeCount == 0:
                for face in vertex.faces:
                    self.faces[face].setNormalFor(vertex.index, self.normalCount)
                self.normalCount = self.normalCount + 1
            else:
                changed = True
                while changed:
                    changed = False
                    for index in vertex.faces:
                        face = self.faces[index]
                        c = face.findCorner(vertex.index)
                        if face.normals[c] == -1:
                            edge = self.edges[face.edges[c]]
                            if edge.faces[0] == face.index:
                                neighbor = edge.faces[1]
                            else:
                                neighbor = edge.faces[0]
                            if edge.hard or neighbor == -1:
                                face.normals[c] = self.normalCount
                                self.normalCount = self.normalCount + 1
                                changed = True
                            else:
                                neighborFace = self.faces[neighbor]
                                neighborCorner = neighborFace.findCorner(vertex.index) 
                                if neighborFace.normals[neighborCorner] != -1:
                                    face.normals[c] = neighborFace.normals[neighborCorner]
                                    changed = True
        # ensure no crap normals exist even if things look shit afterwards
        for face in self.faces:
            for normal in range(len(face.normals)):
                if face.normals[normal] == -1:
                    print("WARNING! -1 normal found at face %i" % (face.index))
                    face.normals[normal] = self.normalCount
                    self.normalCount = self.normalCount + 1
    
    # calculate corner tangents. the seam edge parameter is used to split tangents if requested
    def initCalcCornerTangents(self):
        #if self.object.dragengine_tangentfromseam:
        self.tangentCount = 0
        self.hasUVSeams = False
        for vertex in self.vertices:
            uvseamCount = vertex.getUVSeamCount(self.edges)
            if uvseamCount == 0:
                for face in vertex.faces:
                    self.faces[face].setTangentFor(vertex.index, self.tangentCount)
                self.tangentCount = self.tangentCount + 1
            else:
                self.hasUVSeams = True
                changed = True
                while changed:
                    changed = False
                    for index in vertex.faces:
                        face = self.faces[index]
                        c = face.findCorner(vertex.index)
                        if face.tangents[c] == -1:
                            edge = self.edges[face.edges[c]]
                            if edge.faces[0] == face.index:
                                neighbor = edge.faces[1]
                            else:
                                neighbor = edge.faces[0]
                            if edge.uvseam or neighbor == -1:
                                face.tangents[c] = self.tangentCount
                                self.tangentCount = self.tangentCount + 1
                                changed = True
                            else:
                                neighborFace = self.faces[neighbor]
                                neighborCorner = neighborFace.findCorner(vertex.index) 
                                if neighborFace.tangents[neighborCorner] != -1:
                                    face.tangents[c] = neighborFace.tangents[neighborCorner]
                                    changed = True
        # ensure no crap tangents exist even if things look shit afterwards
        for face in self.faces:
            for tangent in range(len(face.tangents)):
                if face.tangents[tangent] == -1:
                    print("WARNING! -1 tangent found at face %i" % (face.index))
                    face.tangents[tangent] = self.tangentCount
                    self.tangentCount = self.tangentCount + 1
        """else:
            self.tangentCount = self.normalCount
            for face in self.faces:
                face.tangents = [normal for normal in face.normals]"""
    
    # group texture coordinates
    def groupTexCoords(self):
        tcequals = self.object.dragengine_tcequals
        invTCEquals = 1.0 / tcequals
        
        for tcs in self.texCoordSets:
            mapTexCoords = {}
            
            for face in self.faces:
                texCoords = []
                for index in face.face.loop_indices:
                    texCoord = Mesh.TexCoord(tcs.uvlayer.data[index].uv, invTCEquals)
                    found = mapTexCoords.get(texCoord)
                    if found:
                        texCoords.append(found)
                    else:
                        texCoord.index = len(tcs.texCoords)
                        tcs.texCoords.append(texCoord)
                        mapTexCoords[texCoord] = texCoord
                        texCoords.append(texCoord)
                face.texCoordSets.append(texCoords)
    
    # calculate the real positions of vertices
    # NOTE used by de_tool_gbuffernormgen
    def initCalcRealVertexPositions(self, matrix):
        for vertex in self.vertices:
            vertex.realPosition = vector_by_matrix(matrix, vertex.vertex.co)
    
    
    # calculate the real face texture coordinates
    # NOTE used by de_tool_gbuffernormgen
    def initCalcRealFaceTexCoords(self):
        tcs = self.texCoordSets[0]
        for face in self.faces:
            face.realTexCoord = [(tcs.uvlayer.data[x].uv[0], 1.0 - tcs.uvlayer.data[x].uv[1])
                for x in face.face.loop_indices]
        
    
    # calculate the real face normals and tangents
    def initCalcRealFaceNormalsAndTangents(self):
        self.realNormals = [Vector((0.0, 0.0, 0.0))] * self.normalCount
        self.realTangents = [Vector((0.0, 0.0, 0.0))] * self.tangentCount
        
        for face in self.faces:
            vcount = len(face.vertices) - 1
            p1 = self.vertices[face.vertices[vcount]].realPosition
            p2 = self.vertices[face.vertices[vcount - 1]].realPosition
            p3 = self.vertices[face.vertices[vcount - 2]].realPosition
            uv1 = face.realTexCoord[vcount]
            uv2 = face.realTexCoord[vcount - 1]
            uv3 = face.realTexCoord[vcount - 2]
            e1 = vecSub(p2, p1)
            e2 = vecSub(p3, p1)
            
            normal = vecCross(e1, e2)
            vlen = vecLength(normal)
            if vlen > 1e-5:
                normal = vecDiv(normal, vlen)
            else:
                normal = [0.0, 1.0, 0.0]
            
            d1 = vec2Sub(uv2, uv1)
            d2 = vec2Sub(uv3, uv1)
            
            tangent = vecSub(vecMul(e1, d2[1]), vecMul(e2, d1[1]))
            vlen = vecLength(tangent)
            if vlen > 1e-5:
                tangent = vecDiv(tangent, vlen)
            else:
                tangent = [1.0, 0.0, 0.0]
            
            bitangent = vecSub(vecMul(e2, d1[0]), vecMul(e1, d2[0]))
            realbitangent = vecCross(normal, tangent)
            if vecDot(realbitangent, bitangent) < 0.0:
                tangent = vecNeg(tangent)
            
            for i in face.normals:
                self.realNormals[i] = vecAdd(self.realNormals[i], normal)
            for i in face.tangents:
                self.realTangents[i] = vecAdd(self.realTangents[i], tangent)
            
            face.realFaceNormal = Vector(normal)
        
        for i in range(self.normalCount):
            vlen = vecLength(self.realNormals[i])
            if vlen > 1e-5:
                self.realNormals[i] = Vector(vecDiv(self.realNormals[i], vlen))
            else:
                self.realNormals[i] = Vector((0.0, 1.0, 0.0))
        for i in range(self.tangentCount):
            vlen = vecLength(self.realTangents[i])
            if vlen > 1e-5:
                self.realTangents[i] = Vector(vecDiv(self.realTangents[i], vlen))
            else:
                self.realTangents[i] = Vector((1.0, 0.0, 0.0))
    
    # update edges with navspace type from vertex groups
    def initNavType(self):
        self.vertexGroupNavTypes = {}
        for vgnt in self.object.dragengine_vgnavtypes:
            self.vertexGroupNavTypes[vgnt.group] = vgnt.navtype
        
        for edge in self.edges:
            v1 = self.mesh.vertices[edge.vertices[0]]
            v2 = self.mesh.vertices[edge.vertices[1]]
            groups = [x for x in v1.groups if x in v2.groups]
            if len(groups) > 0: # if more than one pick the first one
                groupName = self.object.vertex_groups[groups[0]].name
                if groupName in self.vertexGroupNavTypes:
                    edge.navtype = self.vertexGroupNavTypes[groupName]
                else:
                    edge.navtype = 0
    
    # calculate informational numbers
    def initCalcInfoNumbers(self):
        pass
    
    # print infos
    def printInfos(self):
        print("*** Mesh ***")
        print("name:       {}".format(self.object.name))
        print("textures:   {}".format(len(self.textures)))
        print("texCoordSets: {}".format(len(self.texCoordSets)))
        print("texCoords:  {}".format(len(self.texCoordSets[0].texCoords) if self.texCoordSets else 0))
        print("vertPosSets: {}".format(len(self.vertPosSets)))
        print("vertices:   {}".format(len(self.vertices)))
        print("faces:      {}".format(len(self.faces)))
        print("- tris:     {}".format(len(self.triangles)))
        print("- quads:    {}".format(len(self.quads)))
        print("- ngons:    {}".format(len(self.ngons)))
        print("edges:      {}".format(len(self.edges)))
        print("normals:    {}".format(self.normalCount))
        print("tangents:   {}".format(self.tangentCount))
        print("weights:    {}".format(len(self.weights)))
        print("maxBoneWeightCount: {}".format(self.maxBoneWeightCount))
        if self.lodMesh:
            print("")
            print("LOD Mesh: {}".format(self.lodMesh.object.name))
            self.lodMesh.printInfos()

class Connectivity:
    class Vertex:
        def __init__(self, vertex):
            self.vertex = vertex
            self.index = vertex.index
            self.edges = []
            self.faces = []
            self.marked = False
        
        def getEdgeWithVertex(self, vertex):
            for edge in self.edges:
                if edge.matches(self, vertex):
                    return edge
            return None
    
    class Edge:
        def __init__(self, edge):
            self.edge = edge
            self.index = edge.index
            self.vertex1 = None
            self.vertex2 = None
            self.face1 = None
            self.face2 = None
            self.marked = False
        
        def addFace(self, face):
            if self.face1:
                if self.face2:
                    raise "non-manifold! can't continue!"
                self.face2 = face
            else:
                self.face1 = face
        
        def matches(self, vertex1, vertex2):
            return (self.vertex1 == vertex1 and self.vertex2 == vertex2) or (self.vertex1 == vertex2 and self.vertex2 == vertex1)
    
    class Face:
        def __init__(self, face):
            self.face = face
            self.index = face.index
            self.vertices = []
            self.edges = []
            self.marked = False
        
        def findCorner(self, vertex):
            for index in range(len(self.vertices)):
                if self.vertices[index] == vertex:
                    return index
            return -1
    
    def __init__(self, mesh):
        self.mesh = mesh
        self.vertices = []
        self.edges = []
        self.faces = []
        
        for vertex in mesh.vertices:
            nvertex = Connectivity.Vertex(vertex)
            self.vertices.append(nvertex)
        
        for edge in mesh.edges:
            nedge = Connectivity.Edge(edge)
            nedge.vertex1 = self.vertices[edge.vertices[0]]
            nedge.vertex2 = self.vertices[edge.vertices[1]]
            self.edges.append(nedge)
            nedge.vertex1.edges.append(nedge)
            nedge.vertex2.edges.append(nedge)
        
        for face in mesh.polygons:
            nface = Connectivity.Face(face)
            self.faces.append(nface)
            
            for corner in range(len(face.vertices)):
                vi = face.vertices[corner]
                vertex = self.vertices[vi]
                nface.vertices.append(vertex)
                vertex.faces.append(nface)
                nextVertex = self.vertices[face.vertices[(corner + 1) % len(face.vertices)]]
                edge = self.getEdgeWithVertices(vertex, nextVertex)
                nface.edges.append(edge)
                edge.addFace(nface)
    
    def getEdgeWithVertices(self, vertex1, vertex2):
        for edge in self.edges:
            if (vertex1.index == edge.vertex1.index and vertex2.index == edge.vertex2.index) or (vertex1.index == edge.vertex2.index and vertex2.index == edge.vertex1.index):
                return edge
        return None

class MeshOctree(Octree):
    class Face:
        def __init__(self, mesh, face):
            vertices = [mesh.vertices[vertex].co for vertex in face.vertices]
            threshold = 0.001
            vcount = len(vertices)
            self.face = face
            
            self.minextend = vertices[0].copy()
            self.maxextend = self.minextend.copy()
            for vertex in vertices[1:]:
                self.minextend.x = min(self.minextend.x, vertex.x)
                self.minextend.y = min(self.minextend.y, vertex.y)
                self.minextend.z = min(self.minextend.z, vertex.z)
                self.maxextend.x = max(self.maxextend.x, vertex.x)
                self.maxextend.y = max(self.maxextend.y, vertex.y)
                self.maxextend.z = max(self.maxextend.z, vertex.z)
            self.minextend = self.minextend - Vector((threshold, threshold, threshold))
            self.maxextend = self.maxextend + Vector((threshold, threshold, threshold))
            
            self.faceNormal = face.normal.copy()
            self.faceDistance = vertices[0].dot(self.faceNormal)
            self.edgeNormal = []
            self.edgeDistance = []
            for i in range(vcount):
                self.edgeNormal.append(self.faceNormal.cross(vertices[(i + 1) % vcount] - vertices[i]).normalized())
                self.edgeDistance.append(vertices[i].dot(self.edgeNormal[i]))
            """
            print(face.index, "({: .6f},{: .6f},{: .6f})".format(self.minextend.x, self.minextend.y, self.minextend.z), "({: .6f},{: .6f},{: .6f})".format(self.maxextend.x, self.maxextend.y, self.maxextend.z))
            for vertex in vertices:
                print("-", "({: .6f},{: .6f},{: .6f})".format(vertex.x, vertex.y, vertex.z))
            """
        
        def closestPoint(self, point):
            for i in range(len(self.edgeNormal)):
                normal = self.edgeNormal[i]
                distance = point.dot(normal) - self.edgeDistance[i]
                if distance < 0.0:
                    point = point - normal * distance
            return point - self.faceNormal * (point.dot(self.faceNormal) - self.faceDistance)
    
    def __init__(self, center, halfsize):
        super().__init__(center, halfsize)
        self.faces = []
    
    def createFromMesh(mesh):
        minextend = Vector()
        maxextend = Vector()
        for vertex in mesh.vertices:
            co = vertex.co
            minextend.x = min(minextend.x, co.x)
            minextend.y = min(minextend.y, co.y)
            minextend.z = min(minextend.z, co.z)
            maxextend.x = max(maxextend.x, co.x)
            maxextend.y = max(maxextend.y, co.y)
            maxextend.z = max(maxextend.z, co.z)
        octree = MeshOctree((minextend + maxextend) * 0.5, (maxextend - minextend) * 0.5)
        
        for face in mesh.polygons:
            face = MeshOctree.Face(mesh, face)
            octree.insertIntoTree((face.minextend + face.maxextend) * 0.5, (face.maxextend - face.minextend) * 0.5, 4).faces.append(face)
        
        return octree
    
    def createOctree(self, octant):
        hs = self.halfsize * 0.5
        c = self.center
        b = [4, 2, 1]
        return MeshOctree(Vector([c[i]+hs[i] if octant&b[i]==b[i] else c[i]-hs[i] for i in range(3)]), hs)
    
    def clearNodeContent(self):
        self.faces = []
    
    def printTree(self, indent, nodeIndex):
        str1 = "({0:.5f},{1:.5f},{2:.5f})".format(self.minextend.x, self.minextend.y, self.minextend.z)
        str2 = "({0:.5f},{1:.5f},{2:.5f})".format(self.maxextend.x, self.maxextend.y, self.maxextend.z)
        print("{0}node {1}: faces={2} min={3} max={4}".format(indent, nodeIndex, len(self.faces), str1, str2))
        if False:
            for i in range(len(self.faces)):
                face = self.faces[i]
                str1 = "({0:.5f},{1:.5f},{2:.5f})".format(face.minextend.x, face.minextend.y, face.minextend.z)
                str2 = "({0:.5f},{1:.5f},{2:.5f})".format(face.maxextend.x, face.maxextend.y, face.maxextend.z)
                print("{0}face {1}: index={2} min={3} max={4}".format(indent + "   ", i, face.face.index, str1, str2))
        for i in range(8):
            if self.nodes[i]:
                self.nodes[i].printTree(indent + "   ", i)
            else:
                print("{0}node {1}: <empty>".format(indent + "   ", i))
