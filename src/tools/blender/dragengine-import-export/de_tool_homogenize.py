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

        bpy.ops.object.mode_set(mode='OBJECT')
        vl = context.view_layer

        names = []
        for o in objs:
            for vg in o.vertex_groups:
                if vg.name not in names:
                    names.append(vg.name)
        count = len(names)

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
                while vg.index > i:
                    bpy.ops.object.vertex_group_move(direction='UP')
                while vg.index < i:
                    bpy.ops.object.vertex_group_move(direction='DOWN')

        bpy.ops.object.select_all(action='DESELECT')
        actobj.select_set(True)
        vl.objects.active = actobj

        return {'FINISHED'}


registerClass(OBJECT_OT_ToolHomogenizeVertexGroups)


class VIEW3D_MT_homogenize(bpy.types.Menu):
    bl_idname = "VIEW3D_MT_homogenize"
    bl_label = "Homogenize"

    def draw(self, context):
        layout = self.layout
        layOpRow(layout, OBJECT_OT_ToolHomogenizeVertexGroups)


registerClass(VIEW3D_MT_homogenize)
appendSubMenuToMenu(bpy.types.VIEW3D_MT_object, VIEW3D_MT_homogenize)
