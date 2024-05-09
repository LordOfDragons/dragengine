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
import operator  # to get function names for operators like @, +, -

# helper file for blender 2.8 porting only


knownClasses = []
registeredClasses = []
logPrefix = "Drag[en]gine Add-On: "


def delog(message):
    print(logPrefix + message)


def make_annotations(cls):
    """Converts class fields to annotations if running with Blender 2.8"""
    if bpy.app.version < (2, 80):
        return cls
    bl_props = {k: v for k, v in cls.__dict__.items() if isinstance(v, tuple)}
    if bl_props:
        if "__annotations__" not in cls.__dict__:
            setattr(cls, "__annotations__", {})
        annotations = cls.__dict__["__annotations__"]
        for k, v in bl_props.items():
            annotations[k] = v
            delattr(cls, k)
    return cls


def registerClass(cls):
    global knownClasses
    global registeredClasses

    if cls not in knownClasses:
        delog("Remember class: {}".format(cls))
        make_annotations(cls)
        knownClasses.append(cls)

    if cls not in registeredClasses:
        delog("Register class: {}".format(cls))
        bpy.utils.register_class(cls)
        registeredClasses.append(cls)


def registerKnownClasses():
    global knownClasses
    for cls in knownClasses:
        registerClass(cls)


def unregisterRegisteredClasses():
    global registeredClasses
    for cls in reversed(registeredClasses):
        delog("unregisterRegisteredClasses: {}".format(cls))
        bpy.utils.unregister_class(cls)
    del registeredClasses[:]


def appendToMenu(menu, cls):
    def menu_func(self, context):
        self.layout.operator(
            cls.bl_idname, text=cls.bl_label,
            icon=cls.bl_icon if hasattr(cls, "bl_icon") else None)
    menu.append(menu_func)


def appendSubMenuToMenu(menu, cls):
    def menu_func(self, context):
        self.layout.menu(cls.bl_idname)
    menu.append(menu_func)


def layOpRow(layout, cls, text=None, icon=None):
    if not text:
        if hasattr(cls, "bl_label_button"):
            text = cls.bl_label_button
        else:
            text = cls.bl_label
    if not icon and hasattr(cls, "bl_icon"):
        icon = cls.bl_icon
    if not icon:
        icon = 'NONE'
    return layout.row(align=True).operator(
        operator=cls.bl_idname, text=text, icon=icon)


def layPropRow(layout, data, property, expand=True, readonly=False):
    # prop = getattr(data, property)
    # readonly not supported yet by blender
    return layout.row(align=True).prop(
        data=data, property=property, expand=expand)


def layLabRow(layout, text):
    return layout.row(align=True).label(text=text)


def matmul27(a, b):
    return a * b


def matmul28(a, b):
    return operator.matmul(a, b)  # the same as writing a @ b


matmul = matmul28 if hasattr(bpy.app, "version") \
    and bpy.app.version >= (2, 80) else matmul27
