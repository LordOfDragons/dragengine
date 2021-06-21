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

from .de_tool_aogen import OBJECT_OT_ToolAOGeneration
from .de_tool_channellayout import OBJECT_OT_DEToolSortActionChannels
from .de_tool_collapsevertices import TypeDETVertex
from .de_tool_mirrorvertices import OBJECT_OT_ToolMirrorVertices
from .de_tool_removeemptyvertexgroups import OBJECT_OT_ToolRemoveEmptyVertexGroups
from .de_tool_eclassproperty import OBJECT_OT_DEToolEClassProperty
from .de_tool_gbuffernormgen import OBJECT_OT_ToolGBufferNormGen
from .de_tool_loderrorcalc import OBJECT_OT_DEToolLODInfo
from .de_tool_mirroranim import OBJECT_OT_DEToolMirrorAnimation
from .de_tool_projectuv import TypeDETProjectUVTemplate
from .de_tool_rounding import OBJECT_OT_DEToolRounding
from .de_tool_shapropfromtex import OBJECT_OT_ToolShapePropertyFromTexture
from .de_tool_texturefill import OBJECT_OT_ToolTextureFill
from .de_tool_transferuv import OBJECT_OT_ToolTransferUV
from .de_tool_treebranchunwrap import OBJECT_OT_ToolTreeBranchUnwrap
from .de_tool_fixactiongroups import OBJECT_OT_DEToolFixActionGroups
from .de_porting import registerClass



# Simple tools where using an own file is overkill
####################################################

class OBJECT_OT_DEToolSeamToSharp(bpy.types.Operator):
	bl_idname = "dragengine.seamtosharp"
	bl_label = "Drag[en]gine Seam To Sharp"
	bl_options = { 'REGISTER', 'UNDO' }
	__doc__ = """Set sharp edges from seams"""
	
	selected: bpy.props.BoolProperty(name="Selected Edges", description="Apply only to selected edges", default=True)
	visible: bpy.props.BoolProperty(name="Visible Edges", description="Apply only to visible edges", default=True)
	
	@classmethod
	def poll(cls, context):
		return context.active_object != None \
			and context.active_object.type == 'MESH' \
			and (context.tool_settings.mesh_select_mode[1] or context.tool_settings.mesh_select_mode[2])
	
	def execute(self, context):
		editmode = (bpy.context.mode == 'EDIT_MESH')
		if editmode:
			bpy.ops.object.mode_set(mode='OBJECT')
		
		selectedAlways = not self.selected
		visibleAlways = not self.visible
		
		for e in context.active_object.data.edges:
			if (selectedAlways or e.select) and (visibleAlways or not e.hide):
				e.use_edge_sharp = e.use_seam
		
		if editmode:
			bpy.ops.object.mode_set(mode='EDIT')
		
		return { 'FINISHED' }
registerClass(OBJECT_OT_DEToolSeamToSharp)

class OBJECT_OT_DEToolSelectNgon(bpy.types.Operator):
	bl_idname = "dragengine.selectngon"
	bl_label = "Drag[en]gine Select N-Gon"
	bl_options = { 'REGISTER', 'UNDO' }
	__doc__ = """Set N-Gon faces not supported by some exporters"""
	
	@classmethod
	def poll(cls, context):
		return context.active_object != None and context.active_object.type == 'MESH' \
			and context.tool_settings.mesh_select_mode[2]
	
	def execute(self, context):
		editmode = (bpy.context.mode == 'EDIT_MESH')
		if editmode:
			bpy.ops.object.mode_set(mode='OBJECT')
		
		for p in context.active_object.data.polygons:
			if len(p.vertices) > 4:
				p.select = True
		
		if editmode:
			bpy.ops.object.mode_set(mode='EDIT')
		
		return { 'FINISHED' }
registerClass(OBJECT_OT_DEToolSelectNgon)



# Custom Lists
################
"""
class MATERIAL_UL_matslots_example(bpy.types.UIList):
    # The draw_item function is called for each item of the collection that is visible in the list.
    #   data is the RNA object containing the collection,
    #   item is the current drawn item of the collection,
    #   icon is the "computed" icon for the item (as an integer, because some objects like materials or textures
    #   have custom icons ID, which are not available as enum items).
    #   active_data is the RNA object containing the active property for the collection (i.e. integer pointing to the
    #   active item of the collection).
    #   active_propname is the name of the active property (use 'getattr(active_data, active_propname)').
    #   index is index of the current item in the collection.
    def draw_item(self, context, layout, data, item, icon, active_data, active_propname, index):
        ob = data
        slot = item
        ma = slot.material
        # draw_item must handle the three layout types... Usually 'DEFAULT' and 'COMPACT' can share the same code.
        if self.layout_type in {'DEFAULT', 'COMPACT'}:
            # You should always start your row layout by a label (icon + text), this will also make the row easily
            # selectable in the list!
            # We use icon_value of label, as our given icon is an integer value, not an enum ID.
            layout.label(text=ma.name if ma else "", icon_value=icon)
            # And now we can add other UI stuff...
            # Here, we add nodes info if this material uses (old!) shading nodes.
            if ma and not context.scene.render.use_shading_nodes:
                manode = ma.active_node_material
                if manode:
                    # The static method UILayout.icon returns the integer value of the icon ID "computed" for the given
                    # RNA object.
                    layout.label(text="Node %s" % manode.name, icon_value=layout.icon(manode))
                elif ma.use_nodes:
                    layout.label(text="Node <none>")
                else:
                    layout.label(text="")
        # 'GRID' layout type should be as compact as possible (typically a single icon!).
        elif self.layout_type in {'GRID'}:
            layout.alignment = 'CENTER'
            layout.label(text="", icon_value=icon)

# USE
        # template_list now takes two new args.
        # The first one is the identifier of the registered UIList to use (if you want only the default list,
        # with no custom draw code, use "UI_UL_list").
        layout.template_list("MATERIAL_UL_matslots_example", "", obj, "material_slots", obj, "active_material_index")

        # The second one can usually be left as an empty string. It's an additional ID used to distinguish lists in case you
        # use the same list several times in a given area.
        layout.template_list("MATERIAL_UL_matslots_example", "compact", obj, "material_slots",
                             obj, "active_material_index", type='COMPACT')
"""



# Tools panel
###############

class TypeDETOptions(bpy.types.PropertyGroup):
	collapseVerticesTargets: bpy.props.CollectionProperty(type=TypeDETVertex)
	projectUVCurrent: bpy.props.PointerProperty(type=TypeDETProjectUVTemplate)
	projectUVTemplates: bpy.props.CollectionProperty(type=TypeDETProjectUVTemplate)
registerClass(TypeDETOptions)

class VIEW3D_PT_Dragengine(bpy.types.Panel):
	bl_space_type = 'VIEW_3D'
	bl_region_type = 'TOOLS' if bpy.app.version < (2, 80) else 'UI'
	bl_category = 'Drag[en]gine'
	#bl_category = 'Drag[en]gine'
	bl_label = "Drag[en]gine Tools"
	
	@classmethod
	def poll(cls, context):
		return context.active_object
	
	def draw(self, context):
		layout = self.layout
		#layout.prop(context.scene, "dragengine_scaling", expand = True)
		#layout.menu("VIEW3D_MT_Export")
		if context.active_object.type == 'MESH':
			self.drawMeshTools(context)
		if context.active_object.type == 'ARMATURE':
			self.drawArmatureTools(context.active_object)
		if len(context.selected_objects) > 0:
			self.drawCommonTools(context)
	
	def drawMeshTools(self, context):
		object = context.active_object
		layout = self.layout
		
		# collapse vertices
		layout.row(align=True).label(text="Collapse Vertices:")
		col = layout.column(align=True)
		row = col.row(align=True)
		row.operator(operator="dragengine.collapsevertices_settargets", text="Targets")
		row.operator(operator="dragengine.collapsevertices_apply", text="Apply")
		
		# unwrap
		layout.row(align=True).label(text="Unwrap:")
		col = layout.column(align=True)
		row = col.row(align=True)
		row.operator(operator="dragengine.projectuv", text="Project")
		row.operator(operator="dragengine.treebranchunwrap", text="Branch")
		row = col.row(align=True)
		row.operator(operator="dragengine.transferuv", text="Transfer UV")
		
		# editing
		layout.row(align=True).label(text="Editing:")
		col = layout.column(align=True)
		row = col.row(align=True)
		row.operator(operator="dragengine.rounding", text="Rounding")
		row = col.row(align=True)
		row.operator(operator="dragengine.seamtosharp", text="Seam to Sharp")
		row = col.row(align=True)
		row.operator(operator="uv.seams_from_islands", text="Seams from Island")
		row = col.row(align=True)
		row.operator(operator="dragengine.selectngon", text="Select N-Gon")
		row = col.row(align=True)
		row.operator(operator="dragengine.mirror_vertices", text="Mirror Vertices")
		row = col.row(align=True)
		row.operator(operator="dragengine.remove_empty_vertex_groups", text="Remove Empty VGroups")
		
		# generators
		layout.row(align=True).label(text="Generators:")
		col = layout.column(align=True)
		row = col.row(align=True)
		row.operator(operator="dragengine.gbuffernormgen", text="GBuffer NormGen")
		
		# lod testing
		layout.row(align=True).label(text="LOD Testing:")
		
		col = layout.column(align=True)
		row = col.row(align=True)
		op = row.operator(operator="dragengine.lodinfo", text="Max Err")
		op.calcMaxError = True
		op = row.operator(operator="dragengine.lodinfo", text="+ Avg Err")
		op.calcMaxError = True
		op.calcAvgError = True
		
		col.row(align=True).prop(context.scene, "dragengine_lodmaxerror", expand=True)
		col.row(align=True).prop(context.scene, "dragengine_lodavgerror", expand=True)
		
		col = layout.column(align=True)
		col.row(align=False).prop(context.scene, "dragengine_lodtestresultdistance", expand=True)
		col.row(align=True).prop(context.scene, "dragengine_lodtestfov", expand=True)
		col.row(align=True).prop(context.scene, "dragengine_lodtestscreensize", expand=True)
		col.row(align=True).prop(context.scene, "dragengine_lodtestpixelerror", expand=True)
		col.row(align=True).prop(context.scene, "dragengine_lodtesterror", expand=True)
		
		# physics shapes
		layout.row(align=True).label(text="Physics Shapes:")
		col = layout.column(align=True)
		row = col.row(align=True)
		row.operator(operator="dragengine.shapropfromtex", text="Shape Prop from Tex")
		
		# export
		layout.row(align=True).label(text="Export:")
		col = layout.column(align=True)
		row = col.row(align=True)
		row.operator(operator="dragengine.export_model", text="Model")
		row.operator(operator="dragengine.export_rig", text="Rig")
		row.operator(operator="dragengine.export_occmesh", text="OccMesh")
		row.operator(operator="dragengine.export_navspace", text="NavMesh")
		
	def drawArmatureTools(self, object):
		layout = self.layout
		row = layout.row(align=True)
		row.operator(operator="dragengine.mirroranimation", text="Mirror Animation")
		row = layout.row(align=True)
		row.operator(operator="dragengine.fixactiongroups", text="Fix Action Groups")
		
		# export
		layout.row(align=True).label(text="Export:")
		col = layout.column(align=True)
		row = col.row(align=True)
		row.operator(operator="dragengine.export_animation", text="Animation")
	
	def drawCommonTools(self, context):
		layout = self.layout
		layout.row(align=True).label(text="Element Class Properties:")
		col = layout.column(align=True)
		row = col.row(align=True)
		row.operator(operator="dragengine.eclassproperty", text="Property Text")

registerClass(VIEW3D_PT_Dragengine)



"""                
class DOPESHEETACTION_PT_Dragengine(bpy.types.Header):
	bl_space_type = 'DOPESHEET_EDITOR'
	bl_region_type = 'HEADER'
	bl_label = "Drag[en]gine Tools"
	
	@classmethod
	def poll(cls, context):
		return context.space_data.mode == 'ACTION'
	
	def draw(self, context):
		layout = self.layout
		layout.separator()
		row = layout.row(align=True)
		row.operator(operator="dragengine.sortactionchannels", text="")
registerClass(DOPESHEETACTION_PT_Dragengine)
"""

def DOPESHEETACTION_MT_ChannelToolsMenuFunc(self, context):
	self.layout.operator(operator="dragengine.sortactionchannels", text="Sort Channels Alphabetically")

bpy.types.DOPESHEET_MT_channel.append(DOPESHEETACTION_MT_ChannelToolsMenuFunc)




class IMAGEEDITOR_PT_Dragengine(bpy.types.Panel):
	bl_space_type = 'IMAGE_EDITOR'
	bl_region_type = 'UI'
	bl_label = "Drag[en]gine Tools"
	bl_category = 'Drag[en]gine'
	
	@classmethod
	def poll(cls, context):
		return context.space_data.image
	
	def draw(self, context):
		layout = self.layout
		self.drawImageTools(context)
	
	def drawImageTools(self, context):
		layout = self.layout
		
		row = layout.row(align=True)
		row.operator(operator="dragengine.texturefill", text="Texture Fill")
		row.operator(operator="dragengine.encblnor2denor", text="Convert Normal")
registerClass(IMAGEEDITOR_PT_Dragengine)



# Scene properties
####################

bpy.types.Scene.dragengine_scaling = bpy.props.FloatProperty(
	name="Scaling", description="Scaling to apply", default=1.0, min=0.0, soft_max=1.0)

bpy.types.Scene.dragengine_fps = bpy.props.IntProperty(
	name="FPS", description="FPS to use for animations", default=50, min=1, soft_max=100)

class OBJECT_PT_DragengineScene(bpy.types.Panel):
	bl_space_type = 'PROPERTIES'
	bl_region_type = 'WINDOW'
	bl_context = "scene"
	bl_label = "Drag[en]gine Global"
	
	def draw(self, context):
		layout = self.layout
		rd = context.scene
		row = layout.row(align=True)
		row.prop(rd, "dragengine_scaling", expand=True)
		row.prop(rd, "dragengine_fps", expand=True)
registerClass(OBJECT_PT_DragengineScene)

class TypeDEMoveSet(bpy.types.PropertyGroup):
	filters: bpy.props.CollectionProperty(type=bpy.types.PropertyGroup)
	index: bpy.props.IntProperty(name="Index", description="Index", default=-1, min=-1 )
registerClass(TypeDEMoveSet)

bpy.types.Scene.dragengine_movesets = bpy.props.CollectionProperty(type=TypeDEMoveSet)
bpy.types.Scene.dragengine_movesetidx = bpy.props.IntProperty(
	name="Index", description="Index", default=-1, min=-1 )
bpy.types.Scene.dragengine_movesetname = bpy.props.StringProperty(
	name="Move Set Name", description="Move Set Name", default="move")
bpy.types.Scene.dragengine_movesetfilterexpr = bpy.props.StringProperty(
	name="Expression", description="Filter Expression", default="move\..+")

class OBJECT_OT_MoveSetFilterRemove(bpy.types.Operator):
	bl_idname = "dragengine.movesetfilter_remove"
	bl_label = "Remove Move Set Filter"
	bl_options = { 'INTERNAL' }
	__doc__ = """Removes active move set filter"""
	
	@classmethod
	def poll(cls, context):
		return context.scene.dragengine_movesetidx != -1 and context.scene.dragengine_movesets[context.scene.dragengine_movesetidx].index != -1
	
	def execute(self, context):
		scene = context.scene
		moveset = scene.dragengine_movesets[scene.dragengine_movesetidx]
		moveset.filters.remove(moveset.index)
		if moveset.index > 0:
			moveset.index = moveset.index - 1
		return { 'FINISHED' }
registerClass(OBJECT_OT_MoveSetFilterRemove)

class OBJECT_OT_MoveSetFilterAdd(bpy.types.Operator):
	bl_idname = "dragengine.movesetfilter_add"
	bl_label = "Add Move Set Filter"
	bl_options = { 'INTERNAL' }
	__doc__ = """Adds a move set filter"""
	
	@classmethod
	def poll(cls, context):
		return context.scene.dragengine_movesetidx != -1
	
	def execute(self, context):
		scene = context.scene
		moveset = scene.dragengine_movesets[scene.dragengine_movesetidx]
		if not scene.dragengine_movesetfilterexpr in moveset.filters:
			entry = moveset.filters.add()
			entry.name = scene.dragengine_movesetfilterexpr
			moveset.index = len(moveset.filters) - 1
		return { 'FINISHED' }
registerClass(OBJECT_OT_MoveSetFilterAdd)

class OBJECT_OT_MoveSetRemove(bpy.types.Operator):
	bl_idname = "dragengine.moveset_remove"
	bl_label = "Remove Move Set"
	bl_options = { 'INTERNAL' }
	__doc__ = """Removes active move set"""
	
	@classmethod
	def poll(cls, context):
		return context.scene.dragengine_movesetidx != -1
	
	def execute(self, context):
		scene = context.scene
		scene.dragengine_movesets.remove(scene.dragengine_movesetidx)
		if scene.dragengine_movesetidx > 0:
			scene.dragengine_movesetidx = scene.dragengine_movesetidx - 1
		return { 'FINISHED' }
registerClass(OBJECT_OT_MoveSetRemove)

class OBJECT_OT_MoveSetAdd(bpy.types.Operator):
	bl_idname = "dragengine.moveset_add"
	bl_label = "Add Move Set"
	bl_options = { 'INTERNAL' }
	__doc__ = """Adds a move set"""
	
	def execute(self, context):
		scene = context.scene
		if not scene.dragengine_movesetname in scene.dragengine_movesets:
			entry = scene.dragengine_movesets.add()
			entry.name = scene.dragengine_movesetname
			scene.dragengine_movesetidx = len(scene.dragengine_movesets) - 1
		return { 'FINISHED' }
registerClass(OBJECT_OT_MoveSetAdd)

class OBJECT_OT_MoveSetRename(bpy.types.Operator):
	bl_idname = "dragengine.moveset_rename"
	bl_label = "Rename Move Set"
	bl_options = { 'INTERNAL' }
	__doc__ = """Rename active move set"""
	
	@classmethod
	def poll(cls, context):
		return context.scene.dragengine_movesetidx != -1
	
	def execute(self, context):
		scene = context.scene
		moveset = scene.dragengine_movesets[scene.dragengine_movesetidx]
		if scene.dragengine_movesetname != moveset.name:
			if not scene.dragengine_movesetname in scene.dragengine_movesets:
				moveset.name = scene.dragengine_movesetname
		return { 'FINISHED' }
registerClass(OBJECT_OT_MoveSetRename)

class OBJECT_PT_DragengineMoveSets(bpy.types.Panel):
	bl_space_type = 'PROPERTIES'
	bl_region_type = 'WINDOW'
	bl_context = "scene"
	bl_label = "Drag[en]gine Move Sets"
	
	def draw(self, context):
		layout = self.layout
		rd = context.scene
		row = layout.row(align=True)
		row.template_list("UI_UL_list", "movesets", rd, "dragengine_movesets", rd, "dragengine_movesetidx", rows=2, maxrows=5)
		row = layout.row(align=True)
		row.operator("dragengine.moveset_add", text="Add")
		row.operator("dragengine.moveset_remove", text="Remove")
		row.operator("dragengine.moveset_rename", text="Rename")
		layout.prop(rd, "dragengine_movesetname", expand=True)
registerClass(OBJECT_PT_DragengineMoveSets)

class OBJECT_PT_DragengineMoveSetFilters(bpy.types.Panel):
	bl_space_type = 'PROPERTIES'
	bl_region_type = 'WINDOW'
	bl_context = "scene"
	bl_label = "Drag[en]gine Move Set Filters"
	
	@classmethod
	def poll(cls, context):
		return context.scene.dragengine_movesetidx != -1 and len(context.scene.dragengine_movesets) > 0
	
	def draw(self, context):
		layout = self.layout
		rd = context.scene
		rd2 = rd.dragengine_movesets[rd.dragengine_movesetidx]
		row = layout.row(align=True)
		row.template_list("UI_UL_list", "moveset_filters", rd2, "filters", rd2, "index", rows=2, maxrows=5)
		row = layout.row(align=True)
		row.operator("dragengine.movesetfilter_add", text="Add")
		row.operator("dragengine.movesetfilter_remove", text="Remove")
		#since 2.6x the input value has to match the list of potential values. using reg-expr is not possible anymore
		#without coding an entire control on your own
		#layout.prop_search(rd, "dragengine_movesetfilterexpr", bpy.data, "actions")
		layout.prop(rd, "dragengine_movesetfilterexpr")
registerClass(OBJECT_PT_DragengineMoveSetFilters)



# Object properties
#####################

bpy.types.Object.dragengine_scaling = bpy.props.FloatProperty(
	name="Scaling", description="Scaling to apply", default=1.0, min=0.0, soft_max=1.0)

"""
bpy.types.Object.dragengine_tangentfromseam = bpy.props.BoolProperty(
	name="Tangent From Seam", description="Use tangent from UV Seams instead of normals",
	default=False)

bpy.types.Object.dragengine_splitseam = bpy.props.BoolProperty(name="Split Seam",
	description="Split Normals/Tangents using seams", default=False)
"""

bpy.types.Object.dragengine_lodmesh = bpy.props.StringProperty(name="LOD-Mesh",
	description="Mesh to use as next LOD", default="")

bpy.types.Object.dragengine_hasloderror = bpy.props.BoolProperty(name="LOD-Error",
	description="Use Custom LOD error instead of automatically calculated LOD Error ",
	default=False)

bpy.types.Object.dragengine_loderror = bpy.props.FloatProperty(name="Error",
	description="Custom LOD error to use (relative to base mesh) ",
	precision=3, default=0.01, min=0.001, soft_max=0.1)

bpy.types.Object.dragengine_navspacetype = bpy.props.EnumProperty(
	name = "Nav-Space Type", description = "Navigation space type",
	items = (
		('NONE', "None", "Not a navigation space"),
		('GRID', "Grid", "Grid type navigation space"),
		('MESH', "Mesh", "Mesh type navigation space"),
		('VOLUME', "Volume", "Volume type navigation space")),
	default = 'NONE')

bpy.types.Object.dragengine_hasnoseams = bpy.props.BoolProperty(name="No Seams",
	description="Object has no UV-Seams. If not set exporters stop if UV-Seams are missing",
	default=False)

bpy.types.Object.dragengine_maxweights = bpy.props.IntProperty(
	name="Max Weights", description="Maximum number of weights per weight-set",
	default=4, min=1, soft_max=8)

bpy.types.Object.dragengine_tcequals = bpy.props.FloatProperty(name="TC-Equals",
	description="Texture Coordinate Equals Threshold", precision=5, default=0.0001, min=0.0, soft_max=0.001)

class OBJECT_PT_DragengineObject(bpy.types.Panel):
	bl_space_type = 'PROPERTIES'
	bl_region_type = 'WINDOW'
	bl_context = "object"
	bl_label = "Drag[en]gine Model"
	
	@classmethod
	def poll(cls, context):
		return context.active_object and context.active_object.type == 'MESH'
	
	def draw(self, context):
		layout = self.layout
		rd = context.active_object
		row = layout.row(align=True)
		row.prop(rd, "dragengine_scaling", expand=True)
		row.prop(rd, "dragengine_maxweights", expand=True)
		row = layout.row(align=True)
		row.prop(rd, "dragengine_hasnoseams", expand=True)
		#row.prop(rd, "dragengine_splitseam", expand=True)
		#row.prop(rd, "dragengine_tangentfromseam", expand=True)
		row = layout.row(align=True)
		row.prop(rd, "dragengine_navspacetype", expand=False)
registerClass(OBJECT_PT_DragengineObject)

class OBJECT_PT_DragengineLOD(bpy.types.Panel):
	bl_space_type = 'PROPERTIES'
	bl_region_type = 'WINDOW'
	bl_context = "object"
	bl_label = "Drag[en]gine LOD"
	
	@classmethod
	def poll(cls, context):
		return context.active_object and context.active_object.type == 'MESH'
	
	def draw(self, context):
		layout = self.layout
		rd = context.active_object
		
		layout.column(align=True).prop(rd, "dragengine_lodmesh", expand=True)
		
		row = layout.column(align=True).row(align=True)
		row.prop(rd, "dragengine_hasloderror", expand=True)
		subCol = row.column(align=False)
		subCol.enabled = context.active_object.dragengine_hasloderror
		subCol.prop(rd, "dragengine_loderror", expand=True)
registerClass(OBJECT_PT_DragengineLOD)



bpy.types.Object.dragengine_physics = bpy.props.EnumProperty(items = (
	('NONE', "None", "No physics object"),
	('SPHERE', "Sphere", "Sphere shaped physics object"),
	('BOX', "Box", "Box shaped physics object"),
	('CYLINDER', "Cylinder", "Cylinder shaped physics object"),
	('CAPSULE', "Capsule", "Capsule shaped physics object"),
	('HULL', "Convex Hull", "Convex hull shaped physics object"),
	), name = "Physics", description = "Physics shape if this object is a physics object", default = 'NONE')

bpy.types.Object.dragengine_shapeproperty = bpy.props.StringProperty(
	name="Property", description="Shape property string", default="")


class OBJECT_PT_DragenginePhysics(bpy.types.Panel):
	bl_space_type = 'PROPERTIES'
	bl_region_type = 'WINDOW'
	bl_context = "physics"
	bl_label = "Drag[en]gine Physics"
	
	@classmethod
	def poll(cls, context):
		return context.active_object and context.active_object.type == 'MESH'
	
	def draw(self, context):
		layout = self.layout
		rd = context.active_object
		row = layout.row(align=True)
		row.prop(rd, "dragengine_physics", expand = False)
		row = layout.row(align=True)
		row.prop(rd, "dragengine_shapeproperty", expand = False)
registerClass(OBJECT_PT_DragenginePhysics)



# Material properties
#######################

bpy.types.Material.dragengine_exportname = bpy.props.StringProperty(
	name="Export Name", description="Export name to use for the material. Use empty string to use the blender material name", default="")

bpy.types.Material.dragengine_doublesided = bpy.props.BoolProperty(
	name="Double Sided", description="Faces are double sided", default=False)

bpy.types.Material.dragengine_decal = bpy.props.BoolProperty(
	name="Decal", description="Faces are rendered as decal", default=False)

bpy.types.Material.dragengine_decaloffset = bpy.props.IntProperty(
	name="Decal Offset", description="Offset of decal faces relative to others (higher overlaps lower)",
		default=0, min=0, soft_min=0, soft_max=10, max=255)

bpy.types.Material.dragengine_navtype = bpy.props.IntProperty(
	name="Navigation Type", description="Navigation type to use for this material",
		default=0, min=0, soft_min=0, soft_max=10, max=65535)


class OBJECT_PT_DragengineMaterial(bpy.types.Panel):
	bl_space_type = 'PROPERTIES'
	bl_region_type = 'WINDOW'
	bl_context = "material"
	bl_label = "Drag[en]gine Texture"
	
	@classmethod
	def poll(cls, context):
		return context.active_object and context.active_object.active_material
	
	def draw(self, context):
		layout = self.layout
		rd = context.active_object.active_material
		
		row = layout.row(align=True)
		row.prop(rd, "dragengine_exportname", expand=True)
		
		row = layout.row(align=True)
		row.prop(rd, "dragengine_doublesided", expand=True)
		row.prop(rd, "dragengine_navtype", expand=True)
		
		row = layout.row(align=True)
		row.prop(rd, "dragengine_decal", expand=True)
		row.prop(rd, "dragengine_decaloffset", expand=True, text='Offset')
registerClass(OBJECT_PT_DragengineMaterial)


# Vertex group properties
###########################

class TypeDEVertexGroupNavType(bpy.types.PropertyGroup):
	group: bpy.props.StringProperty(
		name="Group", description="Group name", default='')
	
	navtype: bpy.props.IntProperty(
		name="Navigation Type", description="Navigation type to use for members of the vertex group",
		default=0, min=0, soft_min=0, soft_max=10, max=65535)
registerClass(TypeDEVertexGroupNavType)

bpy.types.Object.dragengine_vgnavtypes = bpy.props.CollectionProperty(type=TypeDEVertexGroupNavType)

class OBJECT_OT_VertexGroupNavTypeInit(bpy.types.Operator):
	bl_idname = "dragengine.vertexgroupnavtype_init"
	bl_label = "Init navigation type data for the active vertex group"
	bl_options = { 'INTERNAL' }
	__doc__ = """Init navigation type data for the active vertex group"""
	
	@classmethod
	def poll(cls, context):
		return context.active_object and context.active_object.type == 'MESH' \
			and context.active_object.vertex_groups.active
	
	def execute(self, context):
		activeVertexGroup = context.active_object.vertex_groups.active
		for vgnt in context.active_object.dragengine_vgnavtypes:
			if vgnt.group == activeVertexGroup.name:
				return { 'FINISHED' }
		vertexGroup = context.active_object.dragengine_vgnavtypes.add()
		vertexGroup.group = activeVertexGroup.name
		vertexGroup.navtype = 0
		return { 'FINISHED' }
registerClass(OBJECT_OT_VertexGroupNavTypeInit)



class OBJECT_PT_DragengineVertexGroup(bpy.types.Panel):
	bl_space_type = 'PROPERTIES'
	bl_region_type = 'WINDOW'
	bl_context = "data"
	bl_label = "Drag[en]gine Vertex Group"
	
	@classmethod
	def poll(cls, context):
		return context.active_object and context.active_object.type == 'MESH' \
			and context.active_object.data and context.active_object.vertex_groups.active
	
	def draw(self, context):
		layout = self.layout
		activeVertexGroup = context.active_object.vertex_groups.active
		
		vertexGroup = None
		for vgnt in context.active_object.dragengine_vgnavtypes:
			if vgnt.group == activeVertexGroup.name:
				vertexGroup = vgnt
				break
		
		row = layout.row(align=True)
		
		if vertexGroup:
			row.prop(vertexGroup, "navtype", expand = True)
		else:
			row.operator("dragengine.vertexgroupnavtype_init", text="Vertex Group Navigation Data")
registerClass(OBJECT_PT_DragengineVertexGroup)



# Armature, Action properties
###############################

bpy.types.Action.dragengine_export = bpy.props.BoolProperty(
	name="Export", description="Determines if this action is exported", default=False)

bpy.types.Action.dragengine_exportname = bpy.props.StringProperty(
	name="Export Name",
	description="Export name to use for the action. Use empty string to use the blender action name",
	default="")

bpy.types.Action.dragengine_timescaling = bpy.props.FloatProperty(
	name="Time Scaling", description="Scaling to apply to the export time values",
	precision=5, default=1.0, min=0.0, soft_min=0.0, soft_max=2.0)

bpy.types.Action.dragengine_autorange = bpy.props.BoolProperty(
	name="Automatic Range",
	description="Determines if the range to export is determined from the first and last keyframe set",
	default=True)

bpy.types.Action.dragengine_rangebegin = bpy.props.IntProperty(
	name="Range Begin", description="First frame to export if not using automatic range",
	default=0, soft_min=0, soft_max=100)

bpy.types.Action.dragengine_rangeend = bpy.props.IntProperty(
	name="Range End", description="Last frame to export if not using automatic range",
	default=100, soft_min=0, soft_max=100)

class OBJECT_PT_DragengineArmatureAction(bpy.types.Panel):
	bl_space_type = 'PROPERTIES'
	bl_region_type = 'WINDOW'
	bl_context = "data"
	bl_label = "Drag[en]gine Move"
	
	@classmethod
	def poll(cls, context):
		return context.active_object and context.active_object.animation_data and context.active_object.animation_data.action
	
	def draw(self, context):
		layout = self.layout
		rd = context.active_object.animation_data.action
		
		col = layout.column(align=True)
		row = col.row(align=True)
		row.prop(rd, "dragengine_export", expand=True, toggle=True)
		
		if rd.dragengine_export:
			row = col.row(align=False)
			row.prop(rd, "dragengine_exportname", expand=True)
			row.prop(rd, "dragengine_timescaling", expand=True, toggle=True)
			
			col = layout.column(align=True)
			row = col.row(align=False)
			row.prop(rd, "dragengine_autorange", expand=True, toggle=True)
			if not rd.dragengine_autorange:
				row = col.row(align=False)
				row.prop(rd, "dragengine_rangebegin", expand=True, toggle=True)
				row.prop(rd, "dragengine_rangeend", expand=True, toggle=True)
registerClass(OBJECT_PT_DragengineArmatureAction)



# Armature properties
#######################

bpy.types.Armature.dragengine_bonefilters = bpy.props.CollectionProperty(
	type=bpy.types.PropertyGroup)

bpy.types.Armature.dragengine_bonefilteridx = bpy.props.IntProperty(
	name="Index",
	description="Index",
	default=-1,
	min=-1 )

bpy.types.Armature.dragengine_bonefilterexpr = bpy.props.StringProperty(
	name="Expression",
	description="Filter Expression",
	default="ik\..+")

bpy.types.Armature.dragengine_shortformat = bpy.props.BoolProperty(
	name="Short Format",
	description="Determines if short format is used instead of float format. Short format produces smaller files but can cause artifacts due to the limited resolution",
	default=False)

bpy.types.Armature.dragengine_thresholdpos = bpy.props.FloatProperty(
	name="Position",
	description="Position difference below this threshold are considered 0",
	precision=4,
	default=0.0001, # 0.001 can lead to troubles with certain animations
	min=0.0,
	soft_min=0.0,
	soft_max=0.01)

bpy.types.Armature.dragengine_thresholdrot = bpy.props.FloatProperty(
	name="Rotation",
	description="Rotation difference below this threshold are considered 0",
	precision=3,
	default=0.01, # 0.1 can lead to troubles with certain animations
	min=0.0,
	soft_min=0.0,
	soft_max=10.0)

bpy.types.Armature.dragengine_thresholdscale = bpy.props.FloatProperty(
	name="Scaling",
	description="Scaling difference below this threshold are considered 0",
	precision=4,
	default=0.001,
	min=0.0,
	soft_min=0.0,
	soft_max=0.01)

class OBJECT_OT_BoneFilterRemove(bpy.types.Operator):
	bl_idname = "dragengine.bonefilter_remove"
	bl_label = "Remove Bone Filter"
	bl_options = { 'INTERNAL' }
	__doc__ = """Removes active bone filter"""
	
	@classmethod
	def poll(cls, context):
		return context.active_object and context.active_object.type == 'ARMATURE' and context.active_object.data.dragengine_bonefilteridx != -1
	
	def execute(self, context):
		armature = context.active_object.data
		armature.dragengine_bonefilters.remove(armature.dragengine_bonefilteridx)
		if armature.dragengine_bonefilteridx > 0:
			armature.dragengine_bonefilteridx = armature.dragengine_bonefilteridx - 1
		return { 'FINISHED' }
registerClass(OBJECT_OT_BoneFilterRemove)

class OBJECT_OT_BoneFilterAdd(bpy.types.Operator):
	bl_idname = "dragengine.bonefilter_add"
	bl_label = "Add Bone Filter"
	bl_options = { 'INTERNAL' }
	__doc__ = """Adds a bone filter"""
	
	@classmethod
	def poll(cls, context):
		return context.active_object and context.active_object.type == 'ARMATURE'
	
	def execute(self, context):
		armature = context.active_object.data
		if not armature.dragengine_bonefilterexpr in armature.dragengine_bonefilters:
			entry = armature.dragengine_bonefilters.add()
			entry.name = armature.dragengine_bonefilterexpr
			armature.dragengine_bonefilteridx = len(armature.dragengine_bonefilters) - 1
		return { 'FINISHED' }
registerClass(OBJECT_OT_BoneFilterAdd)

class OBJECT_PT_DragengineIgnoreBones(bpy.types.Panel):
	bl_space_type = 'PROPERTIES'
	bl_region_type = 'WINDOW'
	bl_context = "data"
	bl_label = "Drag[en]gine Ignore Bones"
	
	@classmethod
	def poll(cls, context):
		return context.active_object and context.active_object.type == 'ARMATURE'
	
	def draw(self, context):
		layout = self.layout
		rd = context.active_object.data
		row = layout.row(align=True)
		row.template_list("UI_UL_list", "bonefilters", rd, "dragengine_bonefilters", rd, "dragengine_bonefilteridx", rows=2, maxrows=5)
		row = layout.row(align=True)
		row.operator("dragengine.bonefilter_add", text="Add")
		row.operator("dragengine.bonefilter_remove", text="Remove")
		layout.prop(rd, "dragengine_bonefilterexpr", expand=True)
registerClass(OBJECT_PT_DragengineIgnoreBones)

class OBJECT_PT_DragengineArmature(bpy.types.Panel):
	bl_space_type = 'PROPERTIES'
	bl_region_type = 'WINDOW'
	bl_context = "data"
	bl_label = "Drag[en]gine Rig"
	
	@classmethod
	def poll(cls, context):
		return context.active_object and context.active_object.type == 'ARMATURE'
	
	def draw(self, context):
		layout = self.layout
		rd = context.active_object.data
		
		split = layout.split()
		
		col = split.column(align=True)
		col.label(text="Export Threshold")
		row = col.row(align=True)
		row.prop(rd, "dragengine_thresholdpos", expand=True, toggle=True)
		row = col.row(align=True)
		row.prop(rd, "dragengine_thresholdrot", expand=True, toggle=True)
		row = col.row(align=True)
		row.prop(rd, "dragengine_thresholdscale", expand=True, toggle=True)
		
		col = split.column(align=True)
		col.label(text="Export Options")
		row = col.row(align=True)
		row.prop(rd, "dragengine_shortformat", expand=True, toggle=True)
registerClass(OBJECT_PT_DragengineArmature)



# Armature Bone properties
############################

"""
# to get bone custom properties working both the Bone and EditBone requires to have
# the same custom property set. this is required since Bone and EditBone are constantly
# rebuild from each other while switching in and out of edit mode. thus to get custom
# properties to transfer properly they have to exist in both classes. per bone options
# though seem a bit a nuisance hence this possibility is dropped and the old filter
# type of selecting bones to export is used.

bpy.types.Bone.dragengine_export = bpy.props.BoolProperty(
	name="Export", description="Determines if this bone is exported", default=True)

bpy.types.EditBone.dragengine_export = bpy.props.BoolProperty(
	name="Export", description="Determines if this bone is exported", default=True)

class OBJECT_PT_DragengineBone(bpy.types.Panel):
	bl_space_type = 'PROPERTIES'
	bl_region_type = 'WINDOW'
	bl_context = "bone"
	bl_label = "Drag[en]gine Bone"
	
	@classmethod
	def poll(cls, context):
		return context.active_bone
	
	def draw(self, context):
		layout = self.layout
		#if context.mode == 'EDIT_ARMATURE':
		#	rd = context.active_object.data.bones[context.active_bone.name]
		#else:
		rd = context.active_bone
		col = layout.column(align=True)
		col.prop(rd, "dragengine_export", expand=True, toggle=True)
registerClass(BJECT_PT_DragengineBone)
"""



# Progress panel
##################

"""
class INFO_HT_DragengineProgress(bpy.types.Panel):
	bl_space_type = 'INFO'
	bl_region_type = 'HEADER'
	bl_label = "TEST"
	
	def draw(self, context):
		layout = self.layout
		layout.label(text="this is a test")
		#rd = context.active_object.active_material
		#layout.prop(rd, "dragengine_name", expand = True)
registerClass(INFO_HT_DragengineProgress)
"""

class TypeDragengineProgress(bpy.types.PropertyGroup):
	percentage: bpy.props.FloatProperty(name="Percentage", description="Percentage to display", default=0.0)
	infoText: bpy.props.StringProperty(name="Info Text", description="Information text to display", default="")
	visible: bpy.props.BoolProperty(name="Visible", description="Determines if the progress panel is visible", default=False)
registerClass(TypeDragengineProgress)

class INFO_HT_DragengineProgress(bpy.types.Header):
	bl_space_type = 'INFO'
	
	@classmethod
	def poll(cls, context):
		return context.window_manager.dragengine_progress.visible
	
	def draw(self, context):
		# poll is unfortunately not working if used in the info header. hence we have to artificially prevent
		# showing anything if we are not visible
		if context.window_manager.dragengine_progress.visible:
			layout = self.layout
			rd = context.window_manager
			layout.label(text="this is a test")
			layout.prop(rd.dragengine_progress, "percentage", expand = True)
registerClass(INFO_HT_DragengineProgress)
