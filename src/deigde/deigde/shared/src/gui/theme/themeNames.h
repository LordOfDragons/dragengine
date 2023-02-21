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

#ifndef _THEMENAMES_H_
#define _THEMENAMES_H_

#include <dragengine\dragengine_export.h>

/**
 * \brief Names for gui themes provided by the basic IGDE implementation.
 */
namespace igdeGuiThemeNames{
	/**
	 * \brief Default gui theme.
	 * 
	 * Name: "default"
	 */
	extern DE_DLL_EXPORT const char * defaultTheme;
	
	/**
	 * \brief Properties side panel gui theme.
	 * 
	 * Name: "properties"
	 */
	extern DE_DLL_EXPORT const char * properties;
};

#endif
