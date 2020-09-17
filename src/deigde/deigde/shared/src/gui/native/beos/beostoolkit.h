/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _BEOSTOOLKIT_H_
#define _BEOSTOOLKIT_H_

#include "../../../deigde_configuration.h"

#include <dragengine/common/math/decMath.h>

#include <app/Application.h>
#include <app/Message.h>
#include <interface/Alert.h>
#include <interface/Button.h>
#include <interface/CheckBox.h>
#include <interface/ListItem.h>
#include <interface/ListView.h>
#include <interface/MenuField.h>
#include <interface/MenuItem.h>
#include <interface/PopUpMenu.h>
#include <interface/StringItem.h>
#include <interface/TextView.h>

class igdeUIBeOSHelper{
private:
	igdeUIBeOSHelper();
	
public:
	static rgb_color BlendColor( rgb_color color1, rgb_color color2, float factor );
	static rgb_color ConvertColor(const decColor& color);
	static decColor ConvertColor( rgb_color color );
	static rgb_color BrighterColor( rgb_color color, int amount );
};

#endif
