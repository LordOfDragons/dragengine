/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEBASEIMAGEINFOS_H_
#define _DEBASEIMAGEINFOS_H_

#include "../deBaseModule.h"



/**
 * \brief Image Module Image Informations.
 *
 * Provides informations for an image to loading afterwards. Can be used
 * by the image module to store additional data structures needed for loading
 * in a subclasses object.
 */
class deBaseImageInfo{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new image info object. */
	deBaseImageInfo();
	
	/** \brief Clean up image info object. */
	virtual ~deBaseImageInfo();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Retrieves the width in pixels. */
	virtual int GetWidth() = 0;
	
	/** \brief Retrieves the height in pixels. */
	virtual int GetHeight() = 0;
	
	/** \brief Retrieves the depth in pixels. */
	virtual int GetDepth() = 0;
	
	/** \brief Retrieves the number of components. */
	virtual int GetComponentCount() = 0;
	
	/** \brief Retrieves the bit count of each component. */
	virtual int GetBitCount() = 0;
	/*@}*/
};

#endif
