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

from .de_version import addonVersion
from .de_porting import registerClass, registerKnownClasses
from .de_porting import unregisterRegisteredClasses, delog

delog("Version {}".format(addonVersion))


bl_info = {
    "name": "Drag[en]gine Import/Export",
    "description": "Import/Export Drag[en]gine Resources including helper tools",
    "author": "Plüss Roland",
    "version": ({VERSION_MAJOR}, {VERSION_MINOR}),
    "blender": (3, 0, 0),
    "api": 36079,
    "location": "File > Import-Export > Drag[en]gine",
    "warning": "",
    "url": "https://dragondreams.ch/index.php/dragengine",
    "support": "COMMUNITY",
    "category": "Import-Export"
    }


class VIEW3D_MT_DragengineExport(bpy.types.Menu):
    bl_label = "Drag[en]gine Resources"
    bl_addon_version = addonVersion

    def draw(self, context):
        layout = self.layout
        # settings = context.tool_settings
        layout.operator_context = 'INVOKE_REGION_WIN'
        layout.operator("dragengine.export_model", text=OBJECT_OT_ExportModel.bl_label)
        layout.operator("dragengine.export_animation", text=OBJECT_OT_ExportAnimation.bl_label)
        layout.operator("dragengine.export_rig", text=OBJECT_OT_ExportRig.bl_label)
        layout.operator("dragengine.export_occmesh", text=OBJECT_OT_ExportOcclusionMesh.bl_label)
        layout.operator("dragengine.export_navspace", text=OBJECT_OT_ExportNavigationSpace.bl_label)
        layout.operator("dragengine.export_config", text=OBJECT_OT_ExportConfig.bl_label)


class VIEW3D_MT_DragengineImport(bpy.types.Menu):
    bl_label = "Drag[en]gine Resources"

    def draw(self, context):
        layout = self.layout
        # settings = context.tool_settings
        layout.operator_context = 'INVOKE_REGION_WIN'
        layout.operator("dragengine.import_animation", text=OBJECT_OT_ImportAnimation.bl_label)
        layout.operator("dragengine.import_rig", text=OBJECT_OT_ImportRig.bl_label)


def checkAlreadyRegistered():
    """
    Checks if this add-on has been already registered. This happens if
    the add-on is bundled. If the add-on is registered but is newer
    unregister the old add-on and register this one.

    Returns True to continue registering or False to not register
    """
    regver = None
    unregfunc = None

    if hasattr(bpy.types, "VIEW3D_MT_DragengineExport"):
        view = bpy.types.VIEW3D_MT_DragengineExport
        if "bl_addon_version" in view.__annotations__:
            regver = view.__annotations__["bl_addon_version"]
            unregfunc = view.__annotations__["bl_addon_unregister"]

    delog("=> Already registered add-on version: {}".format(regver))

    if regver:
        if regver < addonVersion:
            delog("=> Already registered add-on is older")
            delog("=> Unregister already registered add-on")
            unregfunc()
        else:
            delog("=> Already registered add-on is not older")
            delog("=> Abort registering")
            return False

    return True


def VIEW3D_MT_DragengineExportMenuFunc(self, context):
    self.layout.menu("VIEW3D_MT_DragengineExport")


def VIEW3D_MT_DragengineImportMenuFunc(self, context):
    self.layout.menu("VIEW3D_MT_DragengineImport")


def register():
    delog("Register classes")
    if not checkAlreadyRegistered():
        return

    VIEW3D_MT_DragengineExport.__annotations__["bl_addon_unregister"] = unregister

    from .de_tools import TypeDragengineProgress, TypeDETOptions
    from .de_export_model import OBJECT_OT_ExportModel
    from .de_export_rig import OBJECT_OT_ExportRig
    from .de_export_animation import OBJECT_OT_ExportAnimation
    from .de_export_navspace import OBJECT_OT_ExportNavigationSpace
    from .de_export_occmesh import OBJECT_OT_ExportOcclusionMesh
    from .de_export_config import OBJECT_OT_ExportConfig
    from .de_import_animation import OBJECT_OT_ImportAnimation
    from .de_import_rig import OBJECT_OT_ImportRig

    registerClass(VIEW3D_MT_DragengineExport)
    registerClass(VIEW3D_MT_DragengineImport)

    registerKnownClasses()

    bpy.types.WindowManager.dragengine_progress = bpy.props.PointerProperty(type=TypeDragengineProgress)
    bpy.types.WindowManager.dragengine_tooloptions = bpy.props.PointerProperty(type=TypeDETOptions)

    if hasattr(bpy.types, "INFO_MT_file_export"):
        bpy.types.INFO_MT_file_export.append(VIEW3D_MT_DragengineExportMenuFunc)
    elif hasattr(bpy.types, "TOPBAR_MT_file_export"):
        bpy.types.TOPBAR_MT_file_export.append(VIEW3D_MT_DragengineExportMenuFunc)

    if hasattr(bpy.types, "INFO_MT_file_import"):
        bpy.types.INFO_MT_file_import.append(VIEW3D_MT_DragengineImportMenuFunc)
    elif hasattr(bpy.types, "TOPBAR_MT_file_import"):
        bpy.types.TOPBAR_MT_file_import.append(VIEW3D_MT_DragengineImportMenuFunc)


def unregister():
    delog("Unregister classes")

    if hasattr(bpy.types, "INFO_MT_file_import"):
        bpy.types.INFO_MT_file_import.remove(VIEW3D_MT_DragengineImportMenuFunc)
    elif hasattr(bpy.types, "TOPBAR_MT_file_import"):
        bpy.types.TOPBAR_MT_file_import.remove(VIEW3D_MT_DragengineImportMenuFunc)

    if hasattr(bpy.types, "INFO_MT_file_export"):
        bpy.types.INFO_MT_file_export.remove(VIEW3D_MT_DragengineExportMenuFunc)
    elif hasattr(bpy.types, "TOPBAR_MT_file_export"):
        bpy.types.TOPBAR_MT_file_export.remove(VIEW3D_MT_DragengineExportMenuFunc)

    if hasattr(bpy.types.WindowManager, "dragengine_progress"):
        delattr(bpy.types.WindowManager, "dragengine_progress")

    if hasattr(bpy.types.WindowManager, "dragengine_tooloptions"):
        delattr(bpy.types.WindowManager, "dragengine_tooloptions")

    unregisterRegisteredClasses()
