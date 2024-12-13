# -*- coding: utf-8 -*-
#
# Drag[en]gine Blender Scripts
#
# Copyright (C) 2023, Plüss Roland (roland@rptd.ch)
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

from .de_porting import registerClass, appendToMenu


# Tool Copy Vertices
######################

class TypeDETCVVertex(bpy.types.PropertyGroup):
    index: bpy.props.IntProperty(
        name="Index",
        description="Vertex index",
        default=0)
    co: bpy.props.FloatVectorProperty(
        name="Coordinates",
        description="Vertex coordinates",
        default=(0, 0, 0),
        size=3)


registerClass(TypeDETCVVertex)


class OBJECT_OT_ToolCopyVerticesCopy(bpy.types.Operator):
    bl_idname = "dragengine.copyvertices_copy"
    bl_label = "Shape key: Copy vertex position"
    bl_label_button = "Copy vertices"
    bl_options = {'REGISTER'}
    bl_icon = 'COPYDOWN'
    __doc__ = """Copy position of selected vertices in active shape key"""

    @classmethod
    def poll(cls, context):
        obj = context.active_object
        return obj is not None and obj.type == 'MESH'\
            and obj.active_shape_key
        # and any([v for v in obj.data.vertices if v.select])

    def execute(self, context):
        bpy.ops.object.mode_set(mode='OBJECT')

        opts = context.window_manager.dragengine_tooloptions
        selection = opts.copyVerticesSelection
        while len(selection) > 0:
            selection.remove(len(selection) - 1)
        shapeKey = context.active_object.active_shape_key
        mesh = context.active_object.data
        for v in mesh.vertices:
            if v.select:
                s = selection.add()
                s.index = v.index
                s.co = shapeKey.data[v.index].co

        bpy.ops.object.mode_set(mode='EDIT')
        return {'FINISHED'}


registerClass(OBJECT_OT_ToolCopyVerticesCopy)
appendToMenu(bpy.types.VIEW3D_MT_edit_mesh_vertices,
             OBJECT_OT_ToolCopyVerticesCopy)

class OBJECT_OT_ToolCopyVerticesPaste(bpy.types.Operator):
    bl_idname = "dragengine.copyvertices_paste"
    bl_label = "Shape key: Paste vertex position"
    bl_label_button = "Paste Vertices"
    bl_icon = 'PASTEDOWN'
    bl_options = {'REGISTER', 'UNDO'}
    __doc__ = """Paste copied vertex positions into active shape key"""

    mirror: bpy.props.BoolProperty(
        name="Mirror",
        description="Mirror vertices",
        default=False)

    topology: bpy.props.BoolProperty(
        name="Topology",
        description="Mirror vertices with topology",
        default=False)

    @classmethod
    def poll(cls, context):
        obj = context.active_object
        return obj is not None and obj.type == 'MESH'
        # and any([x for x in obj.data.vertices if x.select])

    def execute(self, context):
        bpy.ops.object.mode_set(mode='OBJECT')

        opts = context.window_manager.dragengine_tooloptions
        shapeKey = context.active_object.active_shape_key
        mesh = context.active_object.data
        for s in opts.copyVerticesSelection:
            shapeKey.data[s.index].co = s.co

        if self.mirror:
            bpy.ops.object.shape_key_mirror(use_topology=self.topology)

        bpy.ops.object.mode_set(mode='EDIT')
        return {'FINISHED'}


registerClass(OBJECT_OT_ToolCopyVerticesPaste)
appendToMenu(bpy.types.VIEW3D_MT_edit_mesh_vertices,
             OBJECT_OT_ToolCopyVerticesPaste)
