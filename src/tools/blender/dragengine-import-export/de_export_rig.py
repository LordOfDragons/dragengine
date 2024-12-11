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

from bpy_extras.io_utils import ExportHelper
from mathutils import Vector, Matrix

from .de_math import transformPosition, transformBonePosition, convertMatrix, convertMatrixBone
from .de_math import vector_by_matrix, axis_by_matrix, vecAdd, vecMul, vecDiv, matrixToEuler, ONE_PI
from .de_configuration import Configuration
from .de_resources import Mesh, Armature
from .de_porting import registerClass, matmul



class OBJECT_OT_ExportRig(bpy.types.Operator, ExportHelper):
    bl_idname = "dragengine.export_rig"
    bl_label = "Rig (.derig)"
    bl_label_button = "Rig"
    bl_icon = 'ARMATURE_DATA'
    __doc__ = """Export as Drag[en]gine Rig Resource"""
    filename_ext = ".derig"
    filter_glob: bpy.props.StringProperty(default="*.derig", options={'HIDDEN'})
    debug_level: bpy.props.EnumProperty(items = (
        ('0', "None", "Output no debug messages."),
        ('1', "Basic", "Output basic amount of debug messages."),
        ('2', "Verbose", "Output lots of debug messages."),
        ('3', "Debug", "Output very large amount of debug messages."),
        ), name = "Logging", description = "Choose amount of logging", default = '1')
    
    def __init__(self):
        self.mesh = None
        self.armature = None
        self.volumes = []
        self.debugLevel = 1
    
    @classmethod
    def poll(cls, context):
        return context.active_object != None
    
    def execute(self, context):
        self.debugLevel = int(self.debug_level)
        self.report({'INFO'}, "export to '%s'..." % (self.filepath))
        
        self.scaling = context.scene.dragengine_scaling
        self.scalePosition = Matrix(((self.scaling,0,0,0), (0,self.scaling,0,0), (0,0,self.scaling,0), (0,0,0,1)))
        self.transformScalePosition = matmul(self.scalePosition, transformPosition)
        self.transformBoneScalePosition = matmul(self.scalePosition, transformBonePosition)
        
        self.export(context)
        
        return {'FINISHED'}
    
    def export(self, context):
        self.initExporterObjects(context)
        if not self.checkInitState(context):
            return False
        f = open(self.filepath, "w")
        try:
            result = self.safeExport(context, f)
        finally:
            f.close()
        self.printInfos(context)
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
        self.initCollisionVolumes(context)
    
    def identifyAndAddCollisionVolume(self, obj, obj_parent=None, transform=None):
        if obj.rigid_body or obj.dragengine_physics != 'NONE':
            self.volumes.append(Armature.CollisionVolume(
                obj, self.scalePosition, obj_parent, transform))

        elif obj.type == 'EMPTY' and obj.instance_collection:
            c_transform = obj.matrix_world
            if transform:
                c_transform = transform @ c_transform

            for c_obj in obj.instance_collection.objects:
                self.identifyAndAddCollisionVolume(c_obj, obj, c_transform)
    
    def initCollisionVolumes(self, context):
        explicitColVols = [x for x in context.selected_objects
            if x.rigid_body or x.dragengine_physics != 'NONE']

        if explicitColVols:
            for obj in explicitColVols:
                self.identifyAndAddCollisionVolume(obj)

        else:
            if self.armature:
                for obj in self.armature.object.children:
                    self.identifyAndAddCollisionVolume(obj)
            if self.mesh:
                for obj in self.mesh.object.children:
                    self.identifyAndAddCollisionVolume(obj)
        
        print("volumes we found:")
        for v in self.volumes:
            print("-", v.position, v.rotation, v.scaling, v.bone)
    
    def checkInitState(self, context):
        if not self.armature and not self.mesh:
            self.report({'INFO', 'ERROR'}, "There is no Armature or Mesh selected. Select at last one Armature or Mesh")
            return False
        return True
    
    def printInfos(self, context):
        if self.armature:
            self.armature.printInfos()
    
    def safeExport(self, context, f):
        factor = 1.0 / 2.0
        #Blender.Window.DrawProgressBar(0.0, "Writing Rig Informations")
        if not self.writeHeader(f) or not self.writeRig(f):
            return False
        #Blender.Window.DrawProgressBar(factor, "Writing Bone Informations")
        if not self.writeBones(f) or not self.writeTrailer(f):
            return False
        #Blender.Window.DrawProgressBar(1.0, 'Finished')
        return True	
    
    # write header to file
    def writeHeader(self, f):
        f.write("<?xml version='1.0' encoding='UTF-8'?>\n")
        return True
    
    # write collision volume
    def writeCollisionVolume(self, f, volume, indention, bone):
        pos = volume.position
        rot = volume.rotation
        hext = Vector((volume.scaling.x * 0.5, volume.scaling.y * 0.5, volume.scaling.z * 0.5))
        topRadius = hext.x * volume.topRadiusScale
        bottomRadius = hext.x * volume.bottomRadiusScale
        
        rbshape = ''
        if volume.object.rigid_body:
            rbshape = volume.object.rigid_body.collision_shape
        
        if rbshape == 'SPHERE' or volume.object.dragengine_physics == 'SPHERE':
            f.write("{}<sphere>\n".format(indention))
            f.write("{}\t<position x='{:.4g}' y='{:.4g}' z='{:.4g}'/>\n".format(indention, pos.x, pos.y, pos.z))
            f.write("{}\t<radius>{:.4g}</radius>\n".format(indention, hext.x))
            if volume.object.dragengine_shapeproperty:
                f.write("{}\t<property>{}</property>\n".format(indention, volume.object.dragengine_shapeproperty))
            f.write("{}</sphere>\n".format(indention))
            
        elif rbshape == 'BOX' or volume.object.dragengine_physics == 'BOX':
            f.write("{}<box>\n".format(indention))
            f.write("{}\t<position x='{:.4g}' y='{:.4g}' z='{:.4g}'/>\n".format(indention, pos.x, pos.y, pos.z))
            f.write("{}\t<halfExtends x='{:.4g}' y='{:.4g}' z='{:.4g}'/>\n".format(indention, hext.x, hext.y, hext.z))
            f.write("{}\t<rotation x='{:.3g}' y='{:.3g}' z='{:.3g}'/>\n".format(indention, rot.x, rot.y, rot.z))
            if volume.object.dragengine_shapeproperty:
                f.write("{}\t<property>{}</property>\n".format(indention, volume.object.dragengine_shapeproperty))
            f.write("{}</box>\n".format(indention))
            
        elif rbshape == 'CYLINDER' or volume.object.dragengine_physics == 'CYLINDER':
            shapePos = vecAdd(pos, axis_by_matrix(volume.matrix, (0, (bottomRadius - topRadius) * 0.5, 0)))
            f.write("{}<cylinder>\n".format(indention))
            f.write("{}\t<position x='{:.4g}' y='{:.4g}' z='{:.4g}'/>\n".format(indention, *shapePos))
            f.write("{}\t<rotation x='{:.3g}' y='{:.3g}' z='{:.3g}'/>\n".format(indention, rot.x, rot.y, rot.z))
            f.write("{}\t<halfHeight>{:.4g}</halfHeight>\n".format(indention, hext.y))
            if volume.unitRadii:
                f.write("{}\t<radius>{:.4g}</radius>\n".format(indention, hext.x))
            else:
                f.write("{}\t<topRadius>{:.4g}</topRadius>\n".format(indention, topRadius))
                f.write("{}\t<bottomRadius>{:.4g}</bottomRadius>\n".format(indention, bottomRadius))
            if volume.object.dragengine_shapeproperty:
                f.write("{}\t<property>{}</property>\n".format(indention, volume.object.dragengine_shapeproperty))
            f.write("{}</cylinder>\n".format(indention))
            
        elif rbshape == 'CAPSULE' or volume.object.dragengine_physics == 'CAPSULE':
            shapePos = vecAdd(pos, axis_by_matrix(volume.matrix, (0, (bottomRadius - topRadius) * 0.5, 0)))
            radius = hext.x
            halfHeight = max((hext.y * 2 - topRadius - bottomRadius) / 2, 0)
            f.write("{}<capsule>\n".format(indention))
            f.write("{}\t<position x='{:.4g}' y='{:.4g}' z='{:.4g}'/>\n".format(indention, *shapePos))
            f.write("{}\t<rotation x='{:.3g}' y='{:.3g}' z='{:.3g}'/>\n".format(indention, rot.x, rot.y, rot.z))
            f.write("{}\t<halfHeight>{:.4g}</halfHeight>\n".format(indention, halfHeight))
            if volume.unitRadii:
                f.write("{}\t<radius>{:.4g}</radius>\n".format(indention, radius))
            else:
                f.write("{}\t<topRadius>{:.4g}</topRadius>\n".format(indention, topRadius))
                f.write("{}\t<bottomRadius>{:.4g}</bottomRadius>\n".format(indention, bottomRadius))
            if volume.object.dragengine_shapeproperty:
                f.write("{}\t<property>{}</property>\n".format(indention, volume.object.dragengine_shapeproperty))
            f.write("{}</capsule>\n".format(indention))
            
        elif rbshape == 'CONVEX_HULL' or volume.object.dragengine_physics == 'HULL':
            f.write("{}<hull>\n".format(indention))
            f.write("{}\t<position x='{:.4g}' y='{:.4g}' z='{:.4g}'/>\n".format(indention, pos.x, pos.y, pos.z))
            f.write("{}\t<rotation x='{:.3g}' y='{:.3g}' z='{:.3g}'/>\n".format(indention, rot.x, rot.y, rot.z))
            for vertex in volume.object.data.vertices:
                wpos = vector_by_matrix(self.transformScalePosition, vertex.co)
                f.write("{}\t<point x='{:.4g}' y='{:.4g}' z='{:.4g}'/>\n".format(indention, wpos.x, wpos.y, wpos.z))
            if volume.object.dragengine_shapeproperty:
                f.write("{}\t<property>{}</property>\n".format(indention, volume.object.dragengine_shapeproperty))
            f.write("{}</hull>\n".format(indention))
            
        elif rbshape == 'CONE':
            f.write("{}<cylinder>\n".format(indention))
            f.write("{}\t<position x='{:.4g}' y='{:.4g}' z='{:.4g}'/>\n".format(indention, pos.x, pos.y, pos.z))
            f.write("{}\t<rotation x='{:.3g}' y='{:.3g}' z='{:.3g}'/>\n".format(indention, rot.x, rot.y, rot.z))
            f.write("{}\t<halfHeight>{:.4g}</halfHeight>\n".format(indention, hext.y))
            f.write("{}\t<topRadius>0</topRadius>\n".format(indention))
            f.write("{}\t<bottomRadius>{:.4g}</bottomRadius>\n".format(indention, hext.x))
            if volume.object.dragengine_shapeproperty:
                f.write("{}\t<property>{}</property>\n".format(indention, volume.object.dragengine_shapeproperty))
            f.write("{}</cylinder>\n".format(indention))
            
        elif rbshape == 'MESH':
            pass # not supported
            
        elif rbshape == 'COMPOUND':
            pass # not supported
    
    # write rig
    def writeRig(self, f):
        f.write("<rig>\n")
        f.write("\t<mass>1</mass>\n")
        f.write("\t<centralMassPoint x='0' y='0' z='0'/>\n")
        # write volumes belonging to the mesh in general
        for volume in self.volumes:
            if not volume.bone:
                self.writeCollisionVolume(f, volume, "\t", None)
        return True
    
    # write bones
    def writeBones(self, f):
        if self.debugLevel > 0:
            print("saving bones...")
        pose = None
        if self.armature:
            pose = self.armature.object.pose
            count = 0
            for bone in self.armature.bones:
                realBone = bone.bone
                bone.rigMat = convertMatrixBone(realBone.matrix_local)
                bone.restMat = bone.rigMat
                if realBone.parent:
                    bone.restMat = matmul(convertMatrixBone(realBone.parent.matrix_local).inverted(), bone.restMat)
                bone.pos = vector_by_matrix(matmul(self.scalePosition, bone.restMat), Vector((0,0,0)))
                bone.rot = matrixToEuler(bone.restMat)
                boneVolumes = [v for v in self.volumes if v.bone == bone.name]
                boneRigidBodies = [v for v in boneVolumes if v.object.rigid_body]
                boneConstraints = [v for v in boneRigidBodies if (v.object.rigid_body_constraint
                    and v.object.rigid_body_constraint.object2 == v.object)]
                
                # write name
                f.write("\t\n")
                f.write("\t<bone name='%s'>\n" % bone.name)
                # parent
                if bone.parent:
                    f.write("\t\t<parent>%s</parent>\n" % bone.parent.name)
                # position
                f.write("\t\t<position x='%s' y='%s' z='%s'/>\n" % (str(round(bone.pos.x, 3)), str(round(bone.pos.y, 3)), str(round(bone.pos.z, 3))))
                # rotation
                f.write("\t\t<rotation x='%s' y='%s' z='%s'/>\n" % (str(round(bone.rot.x, 2)), str(round(bone.rot.y, 2)), str(round(bone.rot.z, 2))))
                
                # physics properties
                if boneRigidBodies:
                    mass = 0
                    cmp = Vector((0, 0, 0))
                    active = False
                    
                    for v in boneRigidBodies:
                        rbmass = v.object.rigid_body.mass
                        rbshape = v.object.rigid_body.collision_shape
                        if v.object.rigid_body.type == 'ACTIVE':
                            active = True
                        
                        mass = mass + rbmass
                        
                        if rbshape == 'SPHERE' or rbshape == 'BOX' or rbshape == 'CYLINDER' or rbshape == 'CAPSULE':
                            cmp = vecAdd(cmp, vecMul(v.position, rbmass))
                            
                        elif rbshape == 'CONE':
                            # cmp of cone is 1/4 above base along center line
                            cmp = vecAdd(cmp, vecMul(vector_by_matrix(v.matrix,
                                Vector((0, volume.scaling.y * 0.25, 0))), rbmass))
                            
                        elif rbshape == 'CONVEX_HULL':
                            if v.object.data.vertices:
                                vertexCenter = Vector((0, 0, 0))
                                for vertex in v.object.data.vertices:
                                    vertexCenter = vecAdd(vertexCenter, vertex.co)
                                cmp = vecAdd(cmp, vecMul(vector_by_matrix(
                                    matmul(self.scalePosition, v.matrix), vertexCenter), rbmass))
                    if mass:
                        cmp = vecDiv(cmp, mass)
                    
                    if abs(mass - 1) > 1e-5:
                        f.write("\t\t<mass>{:.3g}</mass>\n".format(mass))
                    if any([abs(x) > 1e-5 for x in cmp]):
                        f.write("\t\t<centralMassPoint x='{:.3g}' y='{:.3g}' z='{:.3}'/>\n".format(cmp[0], cmp[1], cmp[2]))
                    if active:
                        f.write("\t\t<dynamic>true</dynamic>\n")
                
                # ik limits
                if pose:
                    poseBone = pose.bones[bone.name]
                    
                    if poseBone.lock_ik_x or poseBone.use_ik_limit_x or poseBone.ik_stiffness_x != 0:
                        f.write("\t\t<ikX>\n")
                        if poseBone.lock_ik_x:
                            f.write("\t\t\t<locked>1</locked>\n")
                        elif poseBone.use_ik_limit_x:
                            f.write("\t\t\t<lower>%g</lower>\n" % (poseBone.ik_min_x / ONE_PI))
                            f.write('\t\t\t<upper>%g</upper>\n' % (poseBone.ik_max_x / ONE_PI))
                            if poseBone.ik_stiffness_x != 0:
                                f.write("\t\t\t<resistance>%g</resistance>\n" % (poseBone.ik_stiffness_x))
                        else:
                            if poseBone.ik_stiffness_x != 0:
                                f.write("\t\t\t<resistance>%g</resistance>\n" % (poseBone.ik_stiffness_x))
                        f.write("\t\t</ikX>\n")
                    
                    if poseBone.lock_ik_z or poseBone.use_ik_limit_z or poseBone.ik_stiffness_z != 0:
                        f.write("\t\t<ikY>\n")
                        if poseBone.lock_ik_z:
                            f.write("\t\t\t<locked>1</locked>\n")
                        elif poseBone.use_ik_limit_z:
                            f.write("\t\t\t<lower>%g</lower>\n" % (poseBone.ik_min_z / ONE_PI))
                            f.write('\t\t\t<upper>%g</upper>\n' % (poseBone.ik_max_z / ONE_PI))
                            if poseBone.ik_stiffness_z != 0:
                                f.write("\t\t\t<resistance>%g</resistance>\n" % (poseBone.ik_stiffness_z))
                        else:
                            if poseBone.ik_stiffness_z != 0:
                                f.write("\t\t\t<resistance>%g</resistance>\n" % (poseBone.ik_stiffness_z))
                        f.write("\t\t</ikY>\n")
                    
                    if poseBone.lock_ik_y or poseBone.use_ik_limit_y or poseBone.ik_stiffness_y != 0:
                        f.write("\t\t<ikZ>\n")
                        if poseBone.lock_ik_y:
                            f.write("\t\t\t<locked>1</locked>\n")
                        elif poseBone.use_ik_limit_y:
                            f.write("\t\t\t<lower>%g</lower>\n" % (poseBone.ik_min_y / ONE_PI))
                            f.write('\t\t\t<upper>%g</upper>\n' % (poseBone.ik_max_y / ONE_PI))
                            if poseBone.ik_stiffness_y != 0:
                                f.write("\t\t\t<resistance>%g</resistance>\n" % (poseBone.ik_stiffness_y))
                        else:
                            if poseBone.ik_stiffness_y != 0:
                                f.write("\t\t\t<resistance>%g</resistance>\n" % (poseBone.ik_stiffness_y))
                        f.write("\t\t</ikZ>\n")
                    
                # write volumes belonging to this bone
                for volume in self.volumes:
                    if volume.bone == bone.name:
                        self.writeCollisionVolume(f, volume, "\t\t", bone)
                
                # constraints
                for volume in boneConstraints:
                    rb = volume.object.rigid_body
                    rbc = volume.object.rigid_body_constraint
                    parentObject = rbc.object1
                    
                    parentVolume = None
                    if parentObject:
                        parentVolume = [v for v in self.volumes if v.object == parentObject]
                        if len(parentVolume) != 1 or not parentVolume[0].bone:
                            continue
                    
                    f.write("\t\t<constraint>\n")
                    #f.write("\t\t\t<position x='0' y='0' z='0'/>\n")
                    #f.write("\t\t\t<rotation x='0' y='0' z='0'/>\n")
                    #f.write("\t\t\t<offset x='0' y='0' z='0'/>\n")
                    if parentVolume:
                        f.write("\t\t\t<bone>{}</bone>\n".format(parentVolume[0].bone))
                    
                    f.write("\t\t\t<damping>\n")
                    if rb.linear_damping > 0:
                        f.write("\t\t\t\t<linear>{:.3g}</linear>\n".format(1 - rb.linear_damping))
                    if rb.angular_damping > 0:
                        f.write("\t\t\t\t<angular>{:.3g}</angular>\n".format(1 - rb.angular_damping))
                    
                    damping = 0
                    count = 0
                    if rbc.use_spring_ang_x:
                        damping = damping + rbc.spring_damping_ang_x
                        count = count + 1
                    if rbc.use_spring_ang_y:
                        damping = damping + rbc.spring_damping_ang_y
                        count = count + 1
                    if rbc.use_spring_ang_z:
                        damping = damping + rbc.spring_damping_ang_z
                        count = count + 1
                    if rbc.use_spring_x:
                        damping = damping + rbc.spring_damping_x
                        count = count + 1
                    if rbc.use_spring_y:
                        damping = damping + rbc.spring_damping_y
                        count = count + 1
                    if rbc.use_spring_z:
                        damping = damping + rbc.spring_damping_z
                        count = count + 1
                    if count > 0:
                        f.write("\t\t\t\t<spring>{:.3g}</spring>\n".format(1 - damping / count))
                    
                    f.write("\t\t\t</damping>\n")
                    
                    # angular x
                    constraint = None
                    
                    if rbc.use_limit_ang_x:
                        if rbc.limit_ang_x_lower != 0 or rbc.limit_ang_x_upper != 0:
                            constraint = Armature.ConstraintAxis()
                            constraint.limitLower = rbc.limit_ang_x_lower / ONE_PI
                            constraint.limitUpper = rbc.limit_ang_x_upper / ONE_PI
                            if rbc.use_spring_ang_x:
                                constraint.springStiffness = rbc.spring_stiffness_ang_x
                    else:
                        constraint = Armature.ConstraintAxis()
                        constraint.limitLower = 1
                        constraint.limitUpper = 0
                        if rbc.use_spring_ang_x:
                            constraint.springStiffness = rbc.spring_stiffness_ang_x
                    
                    if constraint:
                        self.writeConstraintAxis(f, constraint, "angularX")
                    
                    # angular y
                    constraint = None
                    
                    if rbc.use_limit_ang_y:
                        if rbc.limit_ang_y_lower != 0 or rbc.limit_ang_y_upper != 0:
                            constraint = Armature.ConstraintAxis()
                            constraint.limitLower = rbc.limit_ang_y_lower / ONE_PI
                            constraint.limitUpper = rbc.limit_ang_y_upper / ONE_PI
                            if rbc.use_spring_ang_y:
                                constraint.springStiffness = rbc.spring_stiffness_ang_y
                    else:
                        constraint = Armature.ConstraintAxis()
                        constraint.limitLower = 1
                        constraint.limitUpper = 0
                        if rbc.use_spring_ang_y:
                            constraint.springStiffness = rbc.spring_stiffness_ang_y
                    
                    if constraint:
                        self.writeConstraintAxis(f, constraint, "angularY")
                    
                    # angular z
                    constraint = None
                    
                    if rbc.use_limit_ang_z:
                        if rbc.limit_ang_z_lower != 0 or rbc.limit_ang_z_upper != 0:
                            constraint = Armature.ConstraintAxis()
                            constraint.limitLower = rbc.limit_ang_z_lower / ONE_PI
                            constraint.limitUpper = rbc.limit_ang_z_upper / ONE_PI
                            if rbc.use_spring_ang_z:
                                constraint.springStiffness = rbc.spring_stiffness_ang_z
                    else:
                        constraint = Armature.ConstraintAxis()
                        constraint.limitLower = 1
                        constraint.limitUpper = 0
                        if rbc.use_spring_ang_z:
                            constraint.springStiffness = rbc.spring_stiffness_ang_z
                    
                    if constraint:
                        self.writeConstraintAxis(f, constraint, "angularZ")
                    
                    # linear x
                    constraint = None
                    
                    if rbc.use_limit_lin_x:
                        if rbc.limit_lin_x_lower != 0 or rbc.limit_lin_x_upper != 0:
                            constraint = Armature.ConstraintAxis()
                            constraint.limitLower = rbc.limit_lin_x_lower
                            constraint.limitUpper = rbc.limit_lin_x_upper
                            if rbc.use_spring_ang_x:
                                constraint.springStiffness = rbc.spring_stiffness_ang_x
                    else:
                        constraint = Armature.ConstraintAxis()
                        constraint.limitLower = 1
                        constraint.limitUpper = 0
                        if rbc.use_spring_x:
                            constraint.springStiffness = rbc.spring_stiffness_x
                    
                    if constraint:
                        self.writeConstraintAxis(f, constraint, "linearX")
                    
                    # linear y
                    constraint = None
                    
                    if rbc.use_limit_lin_y:
                        if rbc.limit_lin_y_lower != 0 or rbc.limit_lin_y_upper != 0:
                            constraint = Armature.ConstraintAxis()
                            constraint.limitLower = rbc.limit_lin_y_lower
                            constraint.limitUpper = rbc.limit_lin_y_upper
                            if rbc.use_spring_ang_y:
                                constraint.springStiffness = rbc.spring_stiffness_ang_y
                    else:
                        constraint = Armature.ConstraintAxis()
                        constraint.limitLower = 1
                        constraint.limitUpper = 0
                        if rbc.use_spring_y:
                            constraint.springStiffness = rbc.spring_stiffness_y
                    
                    if constraint:
                        self.writeConstraintAxis(f, constraint, "linearY")
                    
                    # linear z
                    constraint = None
                    
                    if rbc.use_limit_lin_z:
                        if rbc.limit_lin_z_lower != 0 or rbc.limit_lin_z_upper != 0:
                            constraint = Armature.ConstraintAxis()
                            constraint.limitLower = rbc.limit_lin_z_lower
                            constraint.limitUpper = rbc.limit_lin_z_upper
                            if rbc.use_spring_ang_z:
                                constraint.springStiffness = rbc.spring_stiffness_ang_z
                    else:
                        constraint = Armature.ConstraintAxis()
                        constraint.limitLower = 1
                        constraint.limitUpper = 0
                        if rbc.use_spring_z:
                            constraint.springStiffness = rbc.spring_stiffness_z
                    
                    if constraint:
                        self.writeConstraintAxis(f, constraint, "linearZ")
                    
                    f.write("\t\t</constraint>\n")
                
                f.write("\t</bone>\n")
                count = count + 1
                
                # debug
                if self.debugLevel > 1:
                    print("- bone", bone.name, "parent", bone.parent, "pos", bone.pos, "rot", bone.rot)
        return True
    
    def writeConstraintAxis(self, f, constraint, tagName):
        f.write("\t\t\t<{}>\n".format(tagName))
        f.write("\t\t\t\t<limitLower>{:.3g}</limitLower>\n".format(constraint.limitLower))
        f.write("\t\t\t\t<limitUpper>{:.3g}</limitUpper>\n".format(constraint.limitUpper))
        #f.write("\t\t\t\t<staticFriction>{}</staticFriction>\n")
        #f.write("\t\t\t\t<kinematicFriction>{}</kinematicFriction>\n")
        if constraint.springStiffness > 0:
            f.write("\t\t\t\t<springStiffness>{:.3g}</springStiffness>\n".format(constraint.springStiffness))
        f.write("\t\t\t</{}>\n".format(tagName))
    
    # write trailer
    def writeTrailer(self, f):
        f.write("</rig>\n")
        return True
registerClass(OBJECT_OT_ExportRig)
