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

bl_info = {
	"name": "Drag[en]gine Import/Export",
	"description": "Import/Export Drag[en]gine Resources including helper tools",
	"author": "Plüss Roland",
	"version": (1, 0),
	"blender": (2, 80, 0),
	"api": 36079,
	"location": "File > Import-Export > Drag[en]gine",
	"warning": "",
	"url": "http://dragengine.rptd.ch",
	"support": "COMMUNITY",
	"category": "Import-Export"
	}

# porting problems ahead. blender 2.8 hard-requires version 2,80,0 as mininum.
# to support both the value has to be 2,80,0 to work. for 2.7 this shows warnings
# to users but that is okay. in the long run we are forced to switch anyways

if "bpy" in locals():
	import imp
	if "de_math" in locals():
		imp.reload(de_math)
	if "de_resources" in locals():
		imp.reload(de_resources)
	if "de_configuration" in locals():
		imp.reload(de_configuration)
	
	if "de_tool_aogen" in locals():
		imp.reload(de_tool_aogen)
	if "de_tool_texturefill" in locals():
		imp.reload(de_tool_texturefill)
	if "de_tool_gbuffernormgen" in locals():
		imp.reload(de_tool_gbuffernormgen)
	if "de_tool_treebranchunwrap" in locals():
		imp.reload(de_tool_treebranchunwrap)
	if "de_tool_channellayout" in locals():
		imp.reload(de_tool_channellayout)
	if "de_tool_mirroranim" in locals():
		imp.reload(de_tool_mirroranim)
	if "de_tool_loderrorcalc" in locals():
		imp.reload(de_tool_loderrorcalc)
	if "de_tool_rounding" in locals():
		imp.reload(de_tool_rounding)
	if "de_tool_collapsevertices" in locals():
		imp.reload(de_tool_collapsevertices)
	if "de_tool_mirrorvertices" in locals():
		imp.reload(de_tool_mirrorvertices)
	if "de_tool_removeemptyvertexgroups" in locals():
		imp.reload(de_tool_removeemptyvertexgroups)
	if "de_tool_projectuv" in locals():
		imp.reload(de_tool_projectuv)
	if "de_tool_transferuv" in locals():
		imp.reload(de_tool_transferuv)
	if "de_tool_shapropfromtex" in locals():
		imp.reload(de_tool_shapropfromtex)
	if "de_tools" in locals():
		imp.reload(de_tools)
	
	if "de_export_model" in locals():
		imp.reload(de_export_model)
	if "de_export_rig" in locals():
		imp.reload(de_export_rig)
	if "de_export_animation" in locals():
		imp.reload(de_export_animation)
	if "de_export_navspace" in locals():
		imp.reload(de_export_navspace)
	if "de_export_occmesh" in locals():
		imp.reload(de_export_occmesh)
	if "de_export_config" in locals():
		imp.reload(de_export_config)
	
	if "de_import_animation" in locals():
		imp.reload(de_import_animation)
	if "de_import_rig" in locals():
		imp.reload(de_import_rig)

from .de_tools import TypeDragengineProgress, TypeDETOptions
from .de_export_model import OBJECT_OT_ExportModel
from .de_export_rig import OBJECT_OT_ExportRig
from .de_export_animation import OBJECT_OT_ExportAnimation
from .de_export_navspace import OBJECT_OT_ExportNavigationSpace
from .de_export_occmesh import OBJECT_OT_ExportOcclusionMesh
from .de_export_config import OBJECT_OT_ExportConfig
from .de_import_animation import OBJECT_OT_ImportAnimation
from .de_import_rig import OBJECT_OT_ImportRig
from .de_porting import registerClass, unregisterRegisteredClasses

import bpy
import os
import re
import math
import time
import struct
import mathutils

from bpy_extras.io_utils import ExportHelper
from bpy_extras.io_utils import ImportHelper
from mathutils import Vector, Matrix



class VIEW3D_MT_DragengineExport(bpy.types.Menu):
	bl_label = "Drag[en]gine Resources"
	
	def draw(self, context):
		layout = self.layout
		settings = context.tool_settings
		layout.operator_context = 'INVOKE_REGION_WIN'
		layout.operator("dragengine.export_model", text=OBJECT_OT_ExportModel.bl_label)
		layout.operator("dragengine.export_animation", text=OBJECT_OT_ExportAnimation.bl_label)
		layout.operator("dragengine.export_rig", text=OBJECT_OT_ExportRig.bl_label)
		layout.operator("dragengine.export_occmesh", text=OBJECT_OT_ExportOcclusionMesh.bl_label)
		layout.operator("dragengine.export_navspace", text=OBJECT_OT_ExportNavigationSpace.bl_label)
		layout.operator("dragengine.export_config", text=OBJECT_OT_ExportConfig.bl_label)
registerClass(VIEW3D_MT_DragengineExport)

class VIEW3D_MT_DragengineImport(bpy.types.Menu):
	bl_label = "Drag[en]gine Resources"
	
	def draw(self, context):
		layout = self.layout
		settings = context.tool_settings
		layout.operator_context = 'INVOKE_REGION_WIN'
		layout.operator("dragengine.import_animation", text=OBJECT_OT_ImportAnimation.bl_label)
		layout.operator("dragengine.import_rig", text=OBJECT_OT_ImportRig.bl_label)
registerClass(VIEW3D_MT_DragengineImport)

def VIEW3D_MT_DragengineExportMenuFunc(self, context):
	self.layout.menu("VIEW3D_MT_DragengineExport")

def VIEW3D_MT_DragengineImportMenuFunc(self, context):
	self.layout.menu("VIEW3D_MT_DragengineImport")

def register():
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
	if hasattr(bpy.types, "INFO_MT_file_import"):
		bpy.types.INFO_MT_file_import.remove(VIEW3D_MT_DragengineImportMenuFunc)
	elif hasattr(bpy.types, "TOPBAR_MT_file_import"):
		bpy.types.TOPBAR_MT_file_import.remove(VIEW3D_MT_DragengineImportMenuFunc)
	
	if hasattr(bpy.types, "INFO_MT_file_export"):
		bpy.types.INFO_MT_file_export.remove(VIEW3D_MT_DragengineExportMenuFunc)
	elif hasattr(bpy.types, "TOPBAR_MT_file_export"):
		bpy.types.TOPBAR_MT_file_export.remove(VIEW3D_MT_DragengineExportMenuFunc)
	
	unregisterRegisteredClasses()

if __name__ == "__main__":
	register()
