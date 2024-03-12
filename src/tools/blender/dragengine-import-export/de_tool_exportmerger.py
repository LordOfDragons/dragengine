# -*- coding: utf-8 -*-
#
# Drag[en]gine Blender Scripts
#
# Copyright (C) 2024, Plüss Roland (roland@dragondreams.ch)
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
from .de_helpers import ProgressDisplay


# Tool Export Merger
######################

class OBJECT_OT_ToolExportMerger(bpy.types.Operator):
    bl_idname = "dragengine.exportmerger"
    bl_label = "Export merge"
    bl_options = {'INTERNAL'}
    __doc__ = """Merge collection objects for exporting"""

    @classmethod
    def poll(cls, context):
        obj = context.active_object
        return obj is not None\
            and obj.type == 'MESH'\
            and len(obj.users_collection) == 1

    def execute(self, context):
        obj = context.active_object
        if obj is None or obj.type != 'MESH' or len(obj.users_collection) != 1:
            return {'FINISHED'}

        bpy.ops.object.mode_set(mode='OBJECT')
        vl = context.view_layer

        lodmeshes = []
        curObj = obj
        while curObj:
            lodmeshes.append(curObj)
            curObj = curObj.dragengine_lodmesh
            if curObj is not None and curObj.name not in bpy.data.objects:
                curObj = None

        progress = ProgressDisplay(1000 * len(lodmeshes), self)
        progress.show()

        for index in range(len(lodmeshes)):
            lodmesh = lodmeshes[index]
            progress.update(1000 * index, "Merging '{}'".format(lodmesh.name))
            self.export_merge(context, lodmesh, progress)

        laycol = self.find_collection_in_layer(
            context.view_layer.layer_collection, obj.users_collection[0])
        if laycol:
            laycol.exclude = False

        bpy.ops.object.select_all(action='DESELECT')
        obj.select_set(True)
        vl.objects.active = obj

        return {'FINISHED'}

    def export_merge(self, context, merge_object, progress):
        if len(merge_object.users_collection) != 1:
            return
        collection = merge_object.users_collection[0]

        laycol = self.find_collection_in_layer(
            context.view_layer.layer_collection, collection)
        if laycol:
            laycol.exclude = False
            laycol.hide_viewport = False

        self.make_instances_real(context, collection)

        skeys = merge_object.data.shape_keys
        if skeys is not None and len(skeys.key_blocks) == 0:
            skeys = None

        if skeys:
            self.shapekey_merge(context, collection, merge_object, progress)
        else:
            self.simple_merge(context, collection, merge_object)

        self.cleanup_collection(context, collection, merge_object)
        
        if laycol:
            laycol.exclude = True

    def find_collection_in_layer(self, laycol, collection):
        if laycol.collection == collection:
            return laycol
        for each in laycol.children:
            found = self.find_collection_in_layer(each, collection)
            if found:
                return found
        return None

    def make_instances_real(self, context, collection):
        """Make instances real.
        
        make all instance type collections in the collection real.
        """
        bpy.ops.object.select_all(action='DESELECT')
        for each in collection.all_objects:
            if each.type != 'EMPTY':
                continue
            if each.instance_type != 'COLLECTION':
                continue
            each.select_set(True)
        if len(context.selected_objects) == 0:
            return
        bpy.ops.object.duplicates_make_real()

    def simple_merge(self, context, collection, merge_object):
        """Simple merge for export.
        
        Makes all mesh type objects in the collection (except the
        merge object) unique and apply all modifiers. Then merge
        all these objects with the merge object.
        """
        self.merge_noshapekey(
            context, merge_object,
            [x for x in collection.all_objects
                if x != merge_object and x.type == 'MESH'])

    def shapekey_merge(self, context, collection, merge_object, progress):
        """Shape key aware merge for export.
        
        Similar to simple_merge but ensures shape keys are retained
        even through geometry changing modifiers like subdivion
        or decimation.
        """
        meshes_no_skey = []
        meshes_with_skey = []
        for each in collection.all_objects:
            if each == merge_object or each.type != 'MESH':
                continue
            skeys = each.data.shape_keys
            if skeys is not None and len(skeys.key_blocks) > 0:
                meshes_with_skey.append(each)
            else:
                meshes_no_skey.append(each)

        self.merge_noshapekey(context, merge_object, meshes_no_skey)
        self.merge_withshapekey(context, merge_object, meshes_with_skey, progress)

    def merge_noshapekey(self, context, merge_object, objects):
        """Shape key aware merge of objects without shape keys."""
        if len(objects) == 0:
            return

        vl = context.view_layer
        bpy.ops.object.select_all(action='DESELECT')
        for each in objects:
            each.select_set(True)

        # note: 'convert' requires active_object which is bonkers
        vl.objects.active = context.selected_objects[0]

        bpy.ops.object.make_single_user(
            object=True, obdata=True, material=False, animation=False)
        bpy.ops.object.convert(target='MESH')

        merge_object.select_set(True)
        vl.objects.active = merge_object
        bpy.ops.object.join()

    def merge_withshapekey(self, context, merge_object, objects, progress):
        """Shape key aware merge of objects with shape keys."""
        if len(objects) == 0:
            return

        base_count = progress.progress
        shape_count = sum(len(x.data.shape_keys.key_blocks) for x in objects)
        shape_index = 0

        vl = context.view_layer
        for each in objects:
            bpy.ops.object.select_all(action='DESELECT')
            each.select_set(True)
            vl.objects.active = each

            # create working copy as full duplicate
            bpy.ops.object.duplicate(linked=False)
            workobj = vl.objects.active

            # remove all shape keys. this keeps the base shape.
            # this is required for certain modifiers to work
            bpy.ops.object.shape_key_remove(all=True, apply_mix=False)

            # apply all modifiers
            context.evaluated_depsgraph_get()
            bpy.ops.object.convert(target='MESH')

            # add base shape key
            bpy.ops.object.shape_key_add(from_mix=False)
            woshapes = workobj.data.shape_keys.key_blocks

            # now shape keys have to be rebuild. this requires
            # creating a temporary copy of the original object
            # for each shape key. modifiers in the copies are
            # then applied. these can then be transfered as shapes
            # to the work object. once done the work object can
            # be merged with the merge object and deleted
            skeys = each.data.shape_keys
            for index in range(1, len(skeys.key_blocks)):
                name = skeys.key_blocks[index].name
                shape_index = shape_index + 1
                progress.update(
                    base_count + shape_index * 1000 // shape_count,
                    "Merging '{}' Shape '{}'".format(each.name, name))

                # create temporary object as full copy
                bpy.ops.object.select_all(action='DESELECT')
                each.select_set(True)
                vl.objects.active = each

                bpy.ops.object.duplicate(linked=False)
                tempobj = vl.objects.active

                # set shape weight to 1
                tempobj.data.shape_keys.key_blocks[name].value = 1

                # remove shape keys leaving behind mix shape
                bpy.ops.object.shape_key_remove(all=True, apply_mix=True)

                # apply all modifiers
                context.evaluated_depsgraph_get()
                bpy.ops.object.convert(target='MESH')

                # select temporary and working mesh. make working active
                bpy.ops.object.select_all(action='DESELECT')
                tempobj.select_set(True)
                workobj.select_set(True)
                vl.objects.active = workobj

                # transfer shape from temporary to working mesh
                bpy.ops.object.join_shapes()

                # delete temporary mesh
                bpy.ops.object.select_all(action='DESELECT')
                tempobj.select_set(True)
                vl.objects.active = tempobj
                bpy.ops.object.delete(use_global=False, confirm=False)

                # rename shape
                woshapes[index].name = name

            # merge working mesh with merge object
            bpy.ops.object.select_all(action='DESELECT')
            workobj.select_set(True)
            merge_object.select_set(True)
            vl.objects.active = merge_object
            bpy.ops.object.join()

    def cleanup_collection(self, context, collection, merge_object):
        """Remove all objects in collection except merge object."""
        bpy.ops.object.select_all(action='DESELECT')
        for each in collection.all_objects:
            if each != merge_object:
                each.select_set(True)
        if len(context.selected_objects) != 0:
            bpy.ops.object.delete(use_global=False, confirm=False)

registerClass(OBJECT_OT_ToolExportMerger)
