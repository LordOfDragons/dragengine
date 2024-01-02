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
import blf
import bgl
import os
import re
import math
import struct
import mathutils

from bpy_extras.io_utils import ExportHelper
from mathutils import Vector, Matrix

from .de_math import transformPosition, transformBonePosition, convertMatrixBone, ONE_PI
from .de_math import vector_by_matrix, matrixToEuler, vecLength, vecSub, quatDot, quatNegate
from .de_configuration import Configuration
from .de_resources import Mesh, Armature
from .de_porting import registerClass, matmul
from .de_helpers import ProgressDisplay, Timer


class OBJECT_OT_ExportAnimation(bpy.types.Operator, ExportHelper):
    def exportActionList(self, context):
        actionFilters = []
        if context.scene.dragengine_movesetidx == -1 or not context.scene.dragengine_movesets:
            actionFilters.append(re.compile("^.+$"))
        else:
            for f in context.scene.dragengine_movesets[context.scene.dragengine_movesetidx].filters:
                actionFilters.append(re.compile("^%s$" % f.name))
        
        actions = []
        for action in bpy.data.actions.values():
            if len(action.groups) == 0:
                continue
            if not action.dragengine_export:
                continue
            if action.library:  # linked actions can cause lots of troubles
                continue
            
            exportAction = False
            for f in actionFilters:
                if f.match(action.name):
                    exportAction = True
                    break
            if not exportAction:
                continue
            
            actions.append(action.name)
        
        return [(x, x, x) for x in actions]
    
    bl_idname = "dragengine.export_animation"
    bl_label = "Animations (.deanim)"
    __doc__ = """Export as Drag[en]gine Animation Resource"""
    filename_ext = ".deanim"
    filter_glob: bpy.props.StringProperty(default="*.deanim", options={'HIDDEN'})
    debug_level: bpy.props.EnumProperty(items = (
        ('0', "None", "Output no debug messages."),
        ('1', "Basic", "Output basic amount of debug messages."),
        ('2', "Verbose", "Output lots of debug messages."),
        ('3', "Debug", "Output very large amount of debug messages."),
        ), name = "Logging", description = "Choose amount of logging", default = '1')
    export_mode: bpy.props.EnumProperty(items = (
        ('0', "Export All", "Export all moves. File is completely overwritten."),
        ('1', "Update Single", "Export single move. File is updated with exported move."),
        ), name = "Export Mode", description = "Export mode", default = '0')
    #export_move: bpy.props.EnumProperty(items=exportActionList)
    export_move: bpy.props.StringProperty(default="")
    
    def __init__(self):
        self.mesh = None
        self.armature = None
        self.moves = []
        self.debugLevel = 1
        self.exportMode = 0
    
    
    
    def draw(self, context):
        layout = self.layout
        
        box = layout.box()
        column = box.column(align=True)
        column.prop(self, "debug_level", expand=False)
        
        layout.row(align=True).label(text="Move exporting")
        box = layout.box()
        column = box.column(align=True)
        row = column.row(align=True)
        row.prop(self, "export_mode", expand=True)
        
        row = column.row(align=True)
        #row.prop(self, "export_move", expand=False, text="")
        row.prop_search(self, "export_move", bpy.data, "actions", text="")
        row.enabled = (self.export_mode == '1')
    
    
    
    @classmethod
    def poll(cls, context):
        return context.active_object != None
    
    def execute(self, context):
        self.debugLevel = int(self.debug_level)
        self.exportMode = int(self.export_mode)
        if self.exportMode == 1:
            self.exportMove = Armature.Move(bpy.data.actions[self.export_move])
        
        self.report({'INFO'}, "export to '%s'..." % (self.filepath))
        
        configuration = Configuration("deexport.config")
        if configuration.parseConfig():
            configuration.printConfig()
        
        self.scaling = context.scene.dragengine_scaling
        self.scalePosition = Matrix(((self.scaling,0,0,0), (0,self.scaling,0,0), (0,0,self.scaling,0), (0,0,0,1)))
        self.transformScalePosition = matmul(self.scalePosition, transformPosition)
        self.transformBoneScalePosition = matmul(self.scalePosition, transformBonePosition)
        
        self.animFPS = context.scene.dragengine_fps
        self.transformTime = 1.0 / float(self.animFPS)
        self.animLimitVarScale = configuration.getValueFor("animation.limits.variable.scale", 0.01)
        
        self.actionFilters = []
        if context.scene.dragengine_movesetidx == -1 or not context.scene.dragengine_movesets:
            if self.debugLevel > 0:
                print("animation actions: add filter '.+'")
            self.actionFilters.append(re.compile("^.+$"))
        else:
            for f in context.scene.dragengine_movesets[context.scene.dragengine_movesetidx].filters:
                if self.debugLevel > 0:
                    print("animation actions: add filter '%s'" % f.name)
                self.actionFilters.append(re.compile("^%s$" % f.name))
        
        self.export(context)
        
        print("*** WARNING ***")
        print("animation exporting uses short for positions and rotations. for position this")
        print("is multiplied by 1000 causing a maximal precision of 1mm. this can lead to")
        print("problems. the code has to be changed to float to fix this. while being at it")
        print("adding support for bezier curves would help to store animations at higher")
        print("quality as well as reducing file size")
        
        return {'FINISHED'}
    
    def export(self, context):
        self.timer = Timer()
        self.initExporterObjects(context)
        if not self.checkInitState(context):
            return False
        self.timer.log("prepare export", peek=True)
        
        retainContent = None
        if self.exportMode == 1:
            if os.path.exists(self.filepath):
                f = open(self.filepath, "rb")
                try:
                    retainContent = self.loadRetainedContent(context, f)
                finally:
                    f.close()
            else:
                retainContent = []
            self.timer.log("load retained actions", peek=True)
        
        f = open(self.filepath, "wb")
        try:
            result = self.safeExport(context, f, retainContent)
        finally:
            f.close()
        self.timer.log("finished", peek=True)
        self.printInfos(context)
        bpy.ops.wm.redraw_timer(type='DRAW_WIN_SWAP', iterations=1)
        return result
    
    def initFindMeshArmRef(self, context):
        for object in context.selected_objects:
            if object.type == 'MESH':
                if not self.mesh:
                    self.mesh = Mesh(object)
                    if object.parent and object.parent.type == 'ARMATURE':
                        if not self.armature:
                            self.armature = Armature(object.parent)
            elif object.type == 'ARMATURE':
                if not self.armature:
                    self.armature = Armature(object)
        if self.mesh and self.armature:
            self.mesh.armature = self.armature
    
    def initExporterObjects(self, context):
        self.initFindMeshArmRef(context)
        if self.armature:
            self.armature.ignoreBones = []
            for f in self.armature.armature.dragengine_bonefilters:
                self.armature.ignoreBones.append(re.compile("^%s$" % f.name))
            self.armature.initAddBones()
        if self.mesh:
            self.mesh.initAddVertPosSets()
    
    def checkInitState(self, context):
        if not self.armature and not self.mesh:
            self.report({'INFO', 'ERROR'}, "There is no Armature or Mesh selected. Select at last one Armature or Mesh parented to an armature")
            return False
        return True
    
    def printInfos(self, context):
        if self.armature:
            self.armature.printInfos()
    
    def loadRetainedContent(self, context, f):
        content = {}
        
        # header
        content["dataHeader"] = f.read(26)
        if content["dataHeader"][0:24] != bytes("Drag[en]gine Animation  ", 'UTF-8'):
            raise Exception("Invalid file format")
        content["version"] = struct.unpack("<B", content["dataHeader"][24:25])[0]
        content["flags"] = struct.unpack("<B", content["dataHeader"][25:26])[0]
        
        if content["version"] not in [1, 2]:
            raise Exception("Invalid file format")
        if content["version"] != 2:
            raise Exception("Can not export single animation into old format. Do a full export.")
        
        # bones
        filepos = f.tell()
        boneCount = struct.unpack("<H", f.read(2))[0]
        if boneCount != len(self.armature.bones):
            raise Exception("Bone count mismatch (is {} should {})".format(len(self.armature.bones), boneCount))
        for i in range(boneCount):
            nameLen = struct.unpack("<B", f.read(1))[0]
            f.read(nameLen)
        datasize = f.tell() - filepos
        f.seek(filepos)
        content["dataBones"] = f.read(datasize)
        
        # vertex position sets
        filepos = f.tell()
        vpsCount = struct.unpack("<H", f.read(2))[0]
        #if vpsCount != len(self.armature.vpsets):
        #    raise Exception( "Vertex position set count mismatch (is {} should {})".format(len(self.armature.bones), boneCount))
        for i in range(vpsCount):
            nameLen = struct.unpack("<B", f.read(1))[0]
            f.read(nameLen)
        datasize = f.tell() - filepos
        f.seek(filepos)
        content["dataVps"] = f.read(datasize)
        
        # moves
        content["dataMoves"] = []
        countMoves = struct.unpack("<H", f.read(2))[0]
        for i in range(countMoves):
            filepos = f.tell()
            
            nameLen = struct.unpack("<B", f.read(1))[0]
            name = f.read(nameLen).decode("utf-8")
            
            f.read(4) # play time in seconds (float)
            playframes = struct.unpack("<H", f.read(2))[0] # play time in frames
            
            for j in range(boneCount):
                boneFlags = struct.unpack("<B", f.read(1))[0]
                
                hasNoBones = ((boneFlags & 0x10) == 0x10)
                if hasNoBones:
                    continue
                
                hasVarPos = (boneFlags & 0x01) == 0x01
                hasVarRot = (boneFlags & 0x02) == 0x02
                hasVarScale = (boneFlags & 0x04) == 0x04
                hasFewKeyframes = (boneFlags & 0x08) == 0x08
                ignoreBone = (boneFlags & 0x10) == 0x10
                shortFormat = (boneFlags & 0x20) != 0x20
                
                if ignoreBone:
                    continue
                if not hasVarPos and not hasVarRot and not hasVarScale:
                    continue
                
                keyframeCount = playframes
                if hasFewKeyframes:
                    keyframeCount = struct.unpack("<H", f.read(2))[0]
                
                dataSize = 2 # keyframe time
                if hasVarPos:
                    dataSize = dataSize + (6 if shortFormat else 12) # struct hhh / fff
                if hasVarRot:
                    dataSize = dataSize + (6 if shortFormat else 12) # struct hhh / fff
                if hasVarScale:
                    dataSize = dataSize + (6 if shortFormat else 12) # struct hhh / fff
                
                f.read(dataSize * keyframeCount)
            
            for j in range(vpsCount):
                vpsFlags = struct.unpack("<B", f.read(1))[0]
                
                hasVarWeight = ((vpsFlags & 0x1) == 0x1)
                hasFewKeyframes = ((vpsFlags & 0x2) == 0x2)
                ignoreSet = ((vpsFlags & 0x4) == 0x4)
                shortFormat = ((vpsFlags & 0x8) != 0x8)
                
                if ignoreSet:
                    continue
                if not hasVarWeight:
                    continue
                
                keyframeCount = playframes
                if hasFewKeyframes:
                    keyframeCount = struct.unpack("<H", f.read(2))[0]
                
                dataSize = 2 # keyframe time
                if hasVarWeight:
                    dataSize = dataSize + (2 if shortFormat else 4) # struct h / f
                
                f.read(dataSize * keyframeCount)
            
            if name == self.exportMove.name:
                continue
            
            datasize = f.tell() - filepos
            f.seek(filepos)
            content["dataMoves"].append(f.read(datasize))
        
        return content
    
    def safeExport(self, context, f, retainContent):
        if not self.prepareBones():
            return False
        
        if not self.scanMoves():
            return False
        
        self.prepareProgress(context)
        
        if not self.writeHeader(f):
            return False
        
        if retainContent:
            f.write(retainContent["dataBones"])
            f.write(retainContent["dataVps"])
            
        else:
            if not self.writeAnimation(f):
                return False
            
            if not self.writeBones(f):
                return False
            if not self.writeVertexPositionSets(f):
                return False
        self.timer.log("prepare and write up to moves", peek=True)
        
        if not self.writeMoves(f, retainContent):
            return False
        
        if not self.writeTrailer(f):
            return False
        
        return True
    
    def roundToInt(self, value):
        return int(value + 0.5)
    
    # helper function that fills a list with the time
    # positions where knots occure in an ipo curve.
    def buildTimeList(self, fcurve, times):
        for keyframe in fcurve.keyframe_points:
            timeToCheck = self.roundToInt(keyframe.co.x)
            if not timeToCheck in times:
                times.append(timeToCheck)
        times.sort()
    
    # prepare bones
    def prepareBones(self):
        if self.debugLevel > 0:
            print("prepare bones...")
        
        for bone in self.armature.bones:
            realBone = bone.bone
            bone.rigMat = convertMatrixBone(realBone.matrix_local)
            bone.restMat = bone.rigMat
            if realBone.parent:
                bone.restMat = matmul(convertMatrixBone(realBone.parent.matrix_local).inverted(), bone.restMat)
            bone.pos = vector_by_matrix(matmul(self.scalePosition, bone.restMat), Vector((0,0,0)))
            bone.rot = matrixToEuler(bone.restMat)
        
        return True
    
    # build list of moves to export
    def scanMoves(self):
        # determine actions to use
        for action in bpy.data.actions.values():
            if len(action.groups) == 0:
                continue
            if not action.dragengine_export:
                continue
            if action.library: # linked actions can cause lots of troubles
                continue
            
            if self.exportMode == 1 and action != self.exportMove.action:
                continue
            
            exportAction = False
            for f in self.actionFilters:
                if f.match(action.name):
                    exportAction = True
                    break
            if not exportAction:
                continue
            
            move = Armature.Move(action)
            if [x for x in self.moves if x.name == move.name]:
                self.report({'INFO', 'ERROR'}, "Tried to export two moves with duplicate export names ({}).".format(
                    ", ".join(["'{}'".format(x.action.name) for x in self.moves if x.name == move.name])))
                return False
            self.moves.append(Armature.Move(action))
        return True
    
    def prepareProgress(self, context):
        self.progress = ProgressDisplay(len(self.moves) + 1, self)
        self.progress.show()
        self.progress.update(0, "Preparations...")
    
    def writeHeader(self, f):
        f.write(bytes("Drag[en]gine Animation  ", 'UTF-8'))
        f.write(struct.pack("<BB", 2, 0))  # version, flags
        return True
    
    def writeAnimation(self, f):
        return True
    
    def writeBones(self, f):
        if self.debugLevel > 0:
            print("saving bones...")
        
        f.write(struct.pack("<H", len(self.armature.bones)))
        
        for bone in self.armature.bones:
            f.write(struct.pack("<B", len(bone.name)))
            f.write(bytes(bone.name, 'UTF-8'))
            
            if self.debugLevel > 1:
                print("- bone", bone.name)
        return True
   
    def writeVertexPositionSets(self, f):
        if self.debugLevel > 0:
            print("saving vertex position sets...")
        if self.mesh:
            f.write(struct.pack("<H", len(self.mesh.vertPosSets)))
            for vps in self.mesh.vertPosSets:
                f.write(struct.pack("<B", len(vps.name)))
                f.write(bytes(vps.name, 'UTF-8'))
                if self.debugLevel > 1:
                    print("- vertexPositionSet", vps.name)
        else:
            f.write(struct.pack("<H", 0))
        return True

    # fix flipping for example when constraint bones cause negated quaterions
    """ these are no quaterions but euler angles!
    def fixQuaternionFlipping(self, moveBones):
        for moveBone in moveBones:
            if len(moveBone.keyframes) < 2:
                continue
            
            for index in range(1, len(moveBone.keyframes)):
                rot1 = moveBone.keyframes[index - 1].rot
                rot2 = moveBone.keyframes[index].rot
                if quatDot(rot1, rot2) < 0.0:
                    moveBone.keyframes[index].rot = quatNegate(moveBone.keyframes[index].rot)
    """
    
    # tests if a sequence of keyframes is linear
    def areKeyframesLinear(self, keyframes, firstIndex, lastIndex):
        kf1 = keyframes[firstIndex]
        kf2 = keyframes[lastIndex]
        
        # calculate differences
        mulTime = 1.0 / (kf2.time - kf1.time)
        diffPos = kf2.pos - kf1.pos
        diffRot = kf2.rot - kf1.rot
        diffScale = kf2.scale - kf1.scale
        
        # test all keyframes in between against this differences
        for index in range(firstIndex + 1, lastIndex):
            kf = keyframes[index]
            factor = (kf.time - kf1.time) * mulTime
            expectedPos = kf1.pos + diffPos * factor
            expectedRot = kf1.rot + diffRot * factor
            expectedScale = kf1.scale + diffScale * factor
            
            if (kf.pos - expectedPos).length > self.armature.thresholdPosition \
            or (kf.rot - expectedRot).length > self.armature.thresholdRotation \
            or (kf.scale - expectedScale).length > self.armature.thresholdScale:
                return False
        
        return True
    
    # optimize the keyframes by dropping keyframes inside linear changes
    def optimizeKeyframes(self, moveBones):
        for moveBone in moveBones:
            if len(moveBone.keyframes) > 0:
                keyframes = [moveBone.keyframes[0]]
                firstIndex = 0
                for index in range(2, len(moveBone.keyframes)):
                    if not self.areKeyframesLinear(moveBone.keyframes, firstIndex, index):
                        firstIndex = index -1
                        keyframes.append(moveBone.keyframes[firstIndex])
                if firstIndex < len(moveBone.keyframes) - 1:
                    keyframes.append(moveBone.keyframes[-1])
                moveBone.keyframes = keyframes

    # tests if a sequence of keyframes is linear
    def areKeyframesLinearVps(self, keyframes, firstIndex, lastIndex):
        kf1 = keyframes[firstIndex]
        kf2 = keyframes[lastIndex]

        # calculate differences
        mulTime = 1.0 / (kf2.time - kf1.time)
        diffWeight = kf2.weight - kf1.weight

        # test all keyframes in between against this differences
        for index in range(firstIndex + 1, lastIndex):
            kf = keyframes[index]
            factor = (kf.time - kf1.time) * mulTime
            expectedWeight = kf1.weight + diffWeight * factor

            if abs(kf.weight - expectedWeight) > self.armature.thresholdWeight:
                return False

        return True

    # optimize the keyframes by dropping keyframes inside linear changes
    def optimizeKeyframesVps(self, moveVpSets):
        for moveVps in moveVpSets:
            if len(moveVps.keyframes) > 0:
                keyframes = [moveVps.keyframes[0]]
                firstIndex = 0
                for index in range(2, len(moveVps.keyframes)):
                    if not self.areKeyframesLinearVps(moveVps.keyframes, firstIndex, index):
                        firstIndex = index -1
                        keyframes.append(moveVps.keyframes[firstIndex])
                if firstIndex < len(moveVps.keyframes) - 1:
                    keyframes.append(moveVps.keyframes[-1])
                moveVps.keyframes = keyframes

    # write moves
    def writeMoves(self, f, retainContent):
        if self.debugLevel > 0:
            print("saving moves...")
        
        pose = self.armature.object.pose
        
        # write moves to file
        countMoves = len(self.moves)
        if retainContent:
            countMoves = countMoves + len(retainContent["dataMoves"])
        f.write(struct.pack("<H", countMoves)) # number of moves
        
        # process moves
        progressCounter = 0
        
        if retainContent:
            for data in retainContent["dataMoves"]:
                f.write(data)
            progressCounter = len(retainContent["dataMoves"])
        
        for move in self.moves:
            self.report({'INFO'}, "%i/%i (%.2f%%): Writing Move %s..." % (progressCounter, \
                len(self.moves), float(progressCounter) / float(countMoves), move.name))
            self.timer.log("{}/{} ({:.2f}): Writing Move {}...".format(progressCounter, len(self.moves),
                float(progressCounter) / float(countMoves), move.name), peek=True)
            self.progress.advance("Export {}...".format(move.name))
            
            """bgl.glColor3f(0.34, 0.50, 0.76)
            blf.position(0, 0, 0, 0)
            blf.draw(0, move.name)"""
            #Blender.Window.DrawProgressBar(0.2 + progressCounter * 0.8 / countMoves, "Writing Move %s..." % move.name)
            playtime = 0
            firstFrame = 1
            lastFrame = 1
            moveBones = []
            moveVpSets = []
            agroups = move.action.groups
            skafcurves = None
            
            # determine the time scaling
            self.timeScale = self.transformTime * move.action.dragengine_timescaling
            
            # determine keyframe time locations for all bones
            validDPParams = []
            validDPParams.append("location")
            validDPParams.append("rotation_quaternion")
            validDPParams.append("rotation_euler")
            validDPParams.append("scale")
            
            for bone in self.armature.bones:
                moveBone = Armature.MoveBone(bone)
                moveBone.times.append(1)
                if bone.name in agroups:
                    boneAGroup = agroups[bone.name]
                    moveBone.used = True
                    for fcurve in boneAGroup.channels:
                        dpParams = fcurve.data_path.rsplit(".", 1)
                        if len(dpParams) == 2 and dpParams[1] in validDPParams:
                            self.buildTimeList(fcurve, moveBone.times)
                
                timeCount = len(moveBone.times)
                if timeCount > 0:
                    firstFrame = min(firstFrame, moveBone.times[0])
                    lastFrame = max(lastFrame, moveBone.times[-1])
                moveBones.append(moveBone)

            # vertex position sets
            shapeKeyAction = None
            shapeKeys = None
            if self.mesh and len(self.mesh.vertPosSets) > 0:
                shapeKeys = self.mesh.mesh.shape_keys
                skaname = move.name + ".shapeKey"
                if skaname in bpy.data.actions:
                    shapeKeyAction = bpy.data.actions[skaname]
                    skafcurves = shapeKeyAction.fcurves

            if shapeKeyAction:
                for vps in self.mesh.vertPosSets:
                    if not vps.shapeKey:
                        continue
                    moveVps = Armature.MoveVps(vps)
                    moveVps.times.append(1)
                    moveVps.findFCurve(skafcurves)
                    moveVps.used = True
                    if moveVps.fcurve:
                        self.buildTimeList(moveVps.fcurve, moveBone.times)
                    moveVpSets.append(moveVps)
            elif shapeKeys:
                for vps in self.mesh.vertPosSets:
                    if not vps.shapeKey:
                        continue
                    moveVps = Armature.MoveVps(vps)
                    moveVps.times.append(1)
                    moveVps.used = True
                    moveVpSets.append(moveVps)

            # determine the frames to export. this can be altered by properties
            if move.automaticRange:
                move.firstFrame = firstFrame
                move.lastFrame = lastFrame
            
            playtime = move.lastFrame - move.firstFrame
            
            # write move informations
            f.write(struct.pack("<B", len(move.name))) # length name
            f.write(bytes(move.name, 'UTF-8')) # name
            
            #f.write(struct.pack("<B", 1.0 / self.timeScale)) # animFPS)) # frames per second (bugged due to rounding errors)
            f.write(struct.pack("<f", float(playtime) * self.timeScale)) # play time in seconds
            f.write(struct.pack("<H", playtime)) # play time in frames
            
            # switch action
            self.armature.object.animation_data.action = move.action
            if shapeKeyAction:
                if not shapeKeys.animation_data:
                    shapeKeys.animation_data_create()
                shapeKeys.animation_data.action = shapeKeyAction
            
            # blender fails to properly update bone states after changing the action. to fix this bug
            # the scene frame is first set to 1 after the initial frame to save. this way the frame
            # has to change for the first written frame forcing an update in all cases
            bpy.context.scene.frame_set(move.firstFrame + 1)

            # depending on the constraints set we need to update more than one the first time
            # or inverse kinematics rules could cause troubles in tricky setups
            for i in range(3):
                bpy.context.scene.frame_set(move.firstFrame)

            # fetch keyframe values
            for time in range(move.firstFrame, move.lastFrame + 1):
                bpy.context.scene.frame_set(time)
                
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
                            poseBone = pose.bones[bone.bone.name]
                            refMat = bone.restMat
                            keyframes = moveBone.keyframes
                            
                            restMat = convertMatrixBone(poseBone.matrix)
                            if poseBone.parent:
                                restMat = matmul(matmul(convertMatrixBone(poseBone.parent.matrix), refMat).inverted(), restMat)
                            else:
                                restMat = matmul(refMat.inverted(), restMat)
                            pos = vector_by_matrix(matmul(self.scalePosition, restMat), Vector((0,0,0)))
                            rot = matrixToEuler(restMat)
                            scale = restMat.decompose()[2]
                            
                            moveBone.keyframes.append(Armature.Keyframe(time - move.firstFrame, pos, rot, scale))

                for moveVps in moveVpSets:
                    if moveVps.used:
                        requiresKeyframe = True

                        if requiresKeyframe:  #time in moveVps.times:
                            keyframes = moveVps.keyframes
                            if moveVps.vps:
                                weight = moveVps.vps.shapeKey.value
                            else:
                                weight = 0
                            moveVps.keyframes.append(Armature.KeyframeVps(time - move.firstFrame, weight))

            # fix flipping for example when constraint bones cause negated quaterions
            #self.fixQuaternionFlipping(moveBones)
            self.timer.log("- build keyframe list", peek=True)
            
            # optimize the keyframes by dropping keyframes inside linear changes
            #for DUMMY in moveBone.keyframes: print(DUMMY)
            self.optimizeKeyframes(moveBones)
            self.optimizeKeyframesVps(moveVpSets)
            self.timer.log("- optimize keyframe list", peek=True)
            
            if self.debugLevel > 0:
                print("- move", move.name, "playtime", float(playtime) * self.timeScale)
            
            # write bones
            for moveBone in moveBones:
                # the bone is stored if there are keyframes
                if len(moveBone.keyframes) > 0:
                    hasVarPos = False
                    hasVarRot = False
                    hasVarScale = False
                    
                    for keyframe in moveBone.keyframes:
                        if keyframe.pos.length > self.armature.thresholdPosition:
                            hasVarPos = True
                        if keyframe.rot.length > self.armature.thresholdRotation:
                            hasVarRot = True
                        if (keyframe.scale - Vector((1.0, 1.0, 1.0))).length > self.armature.thresholdScale:
                            hasVarScale = True
                    
                    hasFewKeyframes = True #(len(moveBone.keyframes) / playtime < 0.25)
                    
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
                    
                    f.write(struct.pack("<B", flags))
                    
                    # only store the keyframes if we have a variable position, rotation or scaling
                    if hasVarPos or hasVarRot or hasVarScale:
                        # number of keyframes if we have only a few
                        if hasFewKeyframes:
                            f.write(struct.pack("<H", len(moveBone.keyframes)))
                        
                        # write keyframes
                        for keyframe in moveBone.keyframes:
                            # if we have few keyframes write the time in frames
                            f.write(struct.pack("<H", keyframe.time))
                            
                            # if we have a variable position write it. if short format is used encode as signed 16-bit.
                            if hasVarPos:
                                if self.armature.shortFormat:
                                    f.write(struct.pack("<hhh", int(keyframe.pos.x * 1000.0), \
                                        int(keyframe.pos.y * 1000.0), int(keyframe.pos.z * 1000.0)))
                                else:
                                    f.write(struct.pack("<fff", keyframe.pos.x, keyframe.pos.y, keyframe.pos.z))
                            
                            # if we have a variable rotation write it. if short format is used encode as signed 16-bit.
                            if hasVarRot:
                                if self.armature.shortFormat:
                                    f.write(struct.pack("<hhh", int(keyframe.rot.x * 100.0), \
                                        int(keyframe.rot.y * 100.0), int(keyframe.rot.z * 100.0)))
                                else:
                                    f.write(struct.pack("<fff", keyframe.rot.x * ONE_PI, keyframe.rot.y * ONE_PI, keyframe.rot.z * ONE_PI))
                            
                            # if we have a variable scale write it. if short format is used encode as signed 16-bit.
                            if hasVarScale:
                                if self.armature.shortFormat:
                                    f.write(struct.pack("<hhh", int(keyframe.scale.x * 1000.0), \
                                        int(keyframe.scale.y * 1000.0), int(keyframe.scale.z * 1000.0)))
                                else:
                                    f.write(struct.pack("<fff", keyframe.scale.x, keyframe.scale.y, keyframe.scale.z))
                            
                            if self.debugLevel > 1:
                                print("- bone", moveBone.bone.name, "keyframe", float(keyframe.time) * self.timeScale, "pos", keyframe.pos, "rot", keyframe.rot, "scale", keyframe.scale)
                    
                    #if self.debugLevel > 0: print("- bone", moveBone.bone.name, "hasVarPos", hasVarPos, "hasVarRot", hasVarRot, "hasVarScale", hasVarScale, "keyframes", len(moveBone.keyframes))
                
                # otherwise the bone is ignored
                else:
                    f.write(struct.pack("<B", 0x10))
                    #if self.debugLevel > 0: print("- bone", moveBone.bone.name, "ignored", "keyframes", len(moveBone.keyframes))

            # write vertex position sets
            for moveVps in moveVpSets:
                if len(moveVps.keyframes) > 0:
                    hasVarWeight = False

                    for keyframe in moveVps.keyframes:
                        if keyframe.weight > self.armature.thresholdWeight:
                            hasVarWeight = True

                    hasFewKeyframes = True  #(len(moveBone.keyframes) / playtime < 0.25)

                    # flags
                    flags = 0
                    if hasVarWeight:
                        flags = flags | 0x1
                    if hasFewKeyframes:
                        flags = flags | 0x2
                    if not self.armature.shortFormat:
                        flags = flags | 0x8

                    f.write(struct.pack("<B", flags))

                    if hasVarWeight:
                        if hasFewKeyframes:
                            f.write(struct.pack("<H", len(moveVps.keyframes)))

                        for keyframe in moveVps.keyframes:
                            f.write(struct.pack("<H", keyframe.time))

                            if hasVarWeight:
                                if self.armature.shortFormat:
                                    f.write(struct.pack("<h", int(keyframe.weight * 1000.0)))
                                else:
                                    f.write(struct.pack("<f", keyframe.weight))

                            if self.debugLevel > 1:
                                print("- vertexPositionSet", moveVps.vps.name if moveVps.vps else "-",
                                    "keyframe", float(keyframe.time) * self.timeScale, "weight", keyframe.weight)

                else:
                    f.write(struct.pack("<B", 0x4))

            progressCounter = progressCounter + 1
            self.timer.log("- write keyframes", peek=True)
        return True

    # write trailer
    def writeTrailer(self, f):
        return True

registerClass(OBJECT_OT_ExportAnimation)
