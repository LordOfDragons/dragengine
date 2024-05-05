# -*- coding: utf-8 -*-
#
# Drag[en]gine Blender Scripts
#
# Copyright (C) 2024, Plüss Roland (roland@rptd.ch)
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

from .de_porting import registerClass, appendToMenu, \
    appendSubMenuToMenu, layOpRow
from .de_helpers import ProgressDisplay


class OBJECT_OT_ToolHomogenizeVertexGroups(bpy.types.Operator):
    bl_idname = "dragengine.homogenize_verteygroups"
    bl_label = "Homogenize Vertex Groups"
    bl_label_button = "Vertex Groups"
    bl_options = {'REGISTER', 'UNDO'}
    bl_icon = 'GROUP_VERTEX'
    __doc__ = """Adjust selected mesh vertex groups to have the
exact same layout as the active mesh vertex groups"""

    @classmethod
    def poll(cls, context):
        obj = context.active_object
        return obj is not None and obj.type == 'MESH' \
            and len([x for x in context.selected_objects
                    if x.type == 'MESH']) > 1

    def execute(self, context):
        actobj = context.active_object
        if actobj is None or actobj.type != 'MESH':
            return {'FINISHED'}
        objs = [x for x in context.selected_objects if x.type == 'MESH']
        if len(objs) < 2:
            return {'FINISHED'}

        del objs[objs.index(actobj)]
        objs.insert(0, actobj)

        bpy.ops.object.mode_set(mode='OBJECT')
        vl = context.view_layer

        names = []
        for o in objs:
            for vg in o.vertex_groups:
                if vg.name not in names:
                    names.append(vg.name)
        count = len(names)
        if not count:
            return {'FINISHED'}

        progress = ProgressDisplay(len(names) * len(objs), self)
        progress.show()

        for o in objs:
            bpy.ops.object.select_all(action='DESELECT')
            o.select_set(True)
            vl.objects.active = o

            for i in range(count):
                name = names[i]
                progress.advance("Process '{}': '{}'".format(o.name, name))
                if name in o.vertex_groups:
                    vg = o.vertex_groups[name]
                else:
                    vg = o.vertex_groups.new(name=name)
                o.vertex_groups.active_index = vg.index
                index = vg.index
                while index > i:
                    bpy.ops.object.vertex_group_move(direction='UP')
                    index = index - 1
                while index < i:
                    bpy.ops.object.vertex_group_move(direction='DOWN')
                    index = index + 1

            o.vertex_groups.active_index = 0

        bpy.ops.object.select_all(action='DESELECT')
        actobj.select_set(True)
        vl.objects.active = actobj

        return {'FINISHED'}


registerClass(OBJECT_OT_ToolHomogenizeVertexGroups)


class OBJECT_OT_ToolHomogenizeMaterials(bpy.types.Operator):
    bl_idname = "dragengine.homogenize_materials"
    bl_label = "Homogenize Materials"
    bl_label_button = "Materials"
    bl_options = {'REGISTER', 'UNDO'}
    bl_icon = 'MATERIAL'
    __doc__ = """Adjust selected mesh material slots to have the
exact same layout as the active mesh material slots"""

    @classmethod
    def poll(cls, context):
        obj = context.active_object
        return obj is not None and obj.type == 'MESH' \
            and len([x for x in context.selected_objects
                    if x.type == 'MESH']) > 1

    def execute(self, context):
        actobj = context.active_object
        if actobj is None or actobj.type != 'MESH':
            return {'FINISHED'}
        objs = [x for x in context.selected_objects if x.type == 'MESH']
        if len(objs) < 2:
            return {'FINISHED'}

        del objs[objs.index(actobj)]
        objs.insert(0, actobj)

        bpy.ops.object.mode_set(mode='OBJECT')
        vl = context.view_layer

        names = []
        for o in objs:
            for ms in o.material_slots:
                if ms.name not in names:
                    names.append(ms.name)
        count = len(names)
        if not count:
            return {'FINISHED'}

        progress = ProgressDisplay(len(names) * len(objs), self)
        progress.show()

        for o in objs:
            bpy.ops.object.select_all(action='DESELECT')
            o.select_set(True)
            vl.objects.active = o

            for i in range(count):
                name = names[i]
                progress.advance("Process '{}': '{}'".format(o.name, name))
                if name in o.material_slots:
                    ms = o.material_slots[name]
                else:
                    bpy.ops.object.material_slot_add()
                    ms = o.material_slots[o.active_material_index]
                    ms.material = bpy.data.materials[name]
                o.active_material_index = ms.slot_index
                index = ms.slot_index
                while index > i:
                    bpy.ops.object.material_slot_move(direction='UP')
                    index = index - 1
                while index < i:
                    bpy.ops.object.material_slot_move(direction='DOWN')
                    index = index + 1

            o.active_material_index = 0

        bpy.ops.object.select_all(action='DESELECT')
        actobj.select_set(True)
        vl.objects.active = actobj

        return {'FINISHED'}


registerClass(OBJECT_OT_ToolHomogenizeMaterials)


class OBJECT_OT_ToolHomogenizeShapeKeys(bpy.types.Operator):
    bl_idname = "dragengine.homogenize_shapekeys"
    bl_label = "Homogenize Shape Keys"
    bl_label_button = "Shape Keys"
    bl_options = {'REGISTER', 'UNDO'}
    bl_icon = 'SHAPEKEY_DATA'
    __doc__ = """Adjust selected mesh shape keys to have the
exact same layout as the active mesh shape keys"""

    @classmethod
    def poll(cls, context):
        obj = context.active_object
        return obj is not None and obj.type == 'MESH' \
            and len([x for x in context.selected_objects
                    if x.type == 'MESH']) > 1

    def execute(self, context):
        actobj = context.active_object
        if actobj is None or actobj.type != 'MESH':
            return {'FINISHED'}
        objs = [x for x in context.selected_objects if x.type == 'MESH']
        if len(objs) < 2:
            return {'FINISHED'}

        del objs[objs.index(actobj)]
        objs.insert(0, actobj)

        bpy.ops.object.mode_set(mode='OBJECT')
        vl = context.view_layer

        names = []
        for o in objs:
            skeys = o.data.shape_keys
            if skeys:
                for sk in skeys.key_blocks[1:]:
                    if sk.name not in names:
                        names.append(sk.name)
        count = len(names)
        if not count:
            return {'FINISHED'}

        progress = ProgressDisplay(len(names) * len(objs), self)
        progress.show()

        for o in objs:
            bpy.ops.object.select_all(action='DESELECT')
            o.select_set(True)
            vl.objects.active = o

            for i in range(count):
                name = names[i]
                progress.advance("Process '{}': '{}'".format(o.name, name))
                skeys = o.data.shape_keys
                if not skeys:
                    o.shape_key_add(name='Basis', from_mix=False)
                    skeys = o.data.shape_keys
                skb = skeys.key_blocks
                if name in skb:
                    sk = skb[name]
                else:
                    sk = o.shape_key_add(name=name, from_mix=False)
                o.active_shape_key_index = skb.keys().index(name)
                index = o.active_shape_key_index
                while index > i:
                    bpy.ops.object.shape_key_move(type='UP')
                    index = index - 1
                while index < i:
                    bpy.ops.object.shape_key_move(type='DOWN')
                    index = index + 1

            o.active_shape_key_index = 0

        bpy.ops.object.select_all(action='DESELECT')
        actobj.select_set(True)
        vl.objects.active = actobj

        return {'FINISHED'}


registerClass(OBJECT_OT_ToolHomogenizeShapeKeys)


class VIEW3D_MT_homogenize(bpy.types.Menu):
    bl_idname = "VIEW3D_MT_homogenize"
    bl_label = "Homogenize"

    def draw(self, context):
        layout = self.layout
        layOpRow(layout, OBJECT_OT_ToolHomogenizeVertexGroups)
        layOpRow(layout, OBJECT_OT_ToolHomogenizeMaterials)
        layOpRow(layout, OBJECT_OT_ToolHomogenizeShapeKeys)


registerClass(VIEW3D_MT_homogenize)
appendSubMenuToMenu(bpy.types.VIEW3D_MT_object, VIEW3D_MT_homogenize)
