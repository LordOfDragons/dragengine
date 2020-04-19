# -*- coding: utf-8 -*-
#
# Drag[en]gine Blender Scripts
#
# Copyright (C) 2011, Plüss Roland ( roland@rptd.ch )
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



# Tool Sort Action Channels
#############################

class OBJECT_OT_DEToolSortActionChannels( bpy.types.Operator ):
	bl_idname = "dragengine.sortactionchannels"
	bl_label = "Drag[en]gine Sort Action Channels"
	bl_options = { 'REGISTER', 'UNDO' }
	__doc__ = """Sort Action Channels"""
	
	#digits = bpy.props.IntProperty( name="Digits", description="Digits to round to", soft_min=0, soft_max=5, default=5 )
	
	@classmethod
	def poll( cls, context ):
		return context.active_object != None \
			and context.active_object.type == 'ARMATURE' \
			and bpy.context.mode == 'POSE' \
			and context.active_object.animation_data \
			and context.active_object.animation_data.action \
			and len( context.active_object.animation_data.action.groups ) > 0
	
	def execute( self, context ):
		pose = context.active_object.pose
		action = context.active_object.animation_data.action
		
		channelNames = [ channel.name for channel in action.groups ]
		
		# sort whatever way
		sortedChannelNames = channelNames
		sortedChannelNames.sort()
		
		channelNames = []
		
		channelNames.extend( [ name for name in sortedChannelNames if name.startswith( "dr." ) ] )
		sortedChannelNames = [ name for name in sortedChannelNames if not name.startswith( "dr." ) ]
		
		channelNames.extend( [ name for name in sortedChannelNames if name.startswith( "ik." ) ] )
		sortedChannelNames = [ name for name in sortedChannelNames if not name.startswith( "ik." ) ]
		
		channelNames.extend( [ name for name in sortedChannelNames if name.startswith( "ikb." ) ] )
		sortedChannelNames = [ name for name in sortedChannelNames if not name.startswith( "ikb." ) ]
		
		channelNames.extend( [ name for name in sortedChannelNames if name.startswith( "ikt." ) ] )
		sortedChannelNames = [ name for name in sortedChannelNames if not name.startswith( "ikt." ) ]
		
		channelNames.extend( sortedChannelNames )
		
		# make sure all channels are deselected. this is tricky since there is a nasty bug problem with
		# blender trying to handle channels. if a channel is active (selected and showing up in the
		# curve editor) sorting is not 100% correct. in particular the active channel is not sorted
		# correctly but trails at the end of the list. the reason for this bug is totally unknown. the
		# only remedy is deselect all channels before using the script. unfortunately blender does not
		# have an explicit deselect operator only a toggle operator. to get this all working the first
		# channel is selected. then the toggle seelction can be used to deselect all channels
		action.groups[ 0 ].select = True
		bpy.ops.anim.channels_select_all_toggle()
		
		# since the ordering can not be changed directly from blender we use a trick. to sort the channels
		# go through the list of the sorted names in the right order. select only the channel to move and
		# call the move to bottom operator. deselect the channel and repeat. this way all channels are
		# properly sorted in the desired order
		for channelName in channelNames:
			action.groups[ channelName ].select = True
			bpy.ops.anim.channels_move( direction='BOTTOM' )
			action.groups[ channelName ].select = False
		
		return { 'FINISHED' }
registerClass(OBJECT_OT_DEToolSortActionChannels)
