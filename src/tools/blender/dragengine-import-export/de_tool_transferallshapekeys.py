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

from .de_porting import registerClass


# Tool Transferr all shape keys
#################################

class OBJECT_OT_ToolTransferAllShapeKeys(bpy.types.Operator):
    bl_idname = "dragengine.transferallshapekeys"
    bl_label = "Transfer all shape keys"
    bl_options = {'INTERNAL'}
    __doc__ = """Transfer all shape keys"""

    @classmethod
    def poll(cls, context):
        obj = context.active_object
        if obj is None or obj.type != 'MESH'\
            or obj.data.shape_keys is None:
                return False
        sel = context.selected_objects
        if len(sel) != 2:
            return False
        return len([x for x in sel if x.type == 'MESH'\
            and obj.data.shape_keys]) == 2

    def execute(self, context):
        bpy.ops.object.mode_set(mode='OBJECT')
        sel = context.selected_objects
        objSrc = context.active_object
        objDest = [x for x in sel if x != objSrc][0]
        
        context.view_layer.objects.active = objDest

        while len(objDest.data.shape_keys.key_blocks) > 1:
            l = len(objDest.data.shape_keys.key_blocks)
            objDest.active_shape_key_index = l - 1
            bpy.ops.object.shape_key_remove(all=False)

        for s in range(1, len(objSrc.data.shape_keys.key_blocks)):
            objSrc.active_shape_key_index = s
            bpy.ops.object.shape_key_transfer()

        bpy.ops.object.mode_set(mode='EDIT')
        return {'FINISHED'}


registerClass(OBJECT_OT_ToolTransferAllShapeKeys)
