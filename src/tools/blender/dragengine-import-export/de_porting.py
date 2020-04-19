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
import operator # to get function names for operators like @, +, -

# helper file for blender 2.8 porting only


registeredClasses = []

def make_annotations(cls):
	"""Converts class fields to annotations if running with Blender 2.8"""
	if bpy.app.version < (2, 80):
		return cls
	bl_props = {k: v for k, v in cls.__dict__.items() if isinstance(v, tuple)}
	if bl_props:
		if '__annotations__' not in cls.__dict__:
			setattr(cls, '__annotations__', {})
		annotations = cls.__dict__['__annotations__']
		for k, v in bl_props.items():
			annotations[k] = v
			delattr(cls, k)
	return cls

def registerClass(cls):
	global registeredClasses
	make_annotations(cls)
	bpy.utils.register_class(cls)
	registeredClasses.append(cls)

def unregisterRegisteredClasses():
	global registeredClasses
	for cls in reversed(registeredClasses):
		bpy.utils.unregister_class(cls)



def matmul27(a, b):
	return a * b

def matmul28(a, b):
	return operator.matmul(a, b) # the same as writing a @ b

matmul = matmul28 if hasattr(bpy.app, "version") and bpy.app.version >= (2, 80) else matmul27
