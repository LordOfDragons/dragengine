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

#ifndef _DEBASEGRAPHICIMAGE_H_
#define _DEBASEGRAPHICIMAGE_H_

#include "../../../dragengine_export.h"


/**
 * \brief Graphic module image peer.
 */
class DE_DLL_EXPORT deBaseGraphicImage{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseGraphicImage();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Image data changed. */
	virtual void ImageDataChanged();
	
	/** \brief Image data has been restored from original file. */
	virtual void ImageDataRestored();
	
	/** \brief Image data has been released. */
	virtual void ImageDataReleased();
	
	/**
	 * \brief Graphic module requires image data to remain loaded.
	 * 
	 * Used to optimized memory consumption. Default implementation returns false.
	 */
	virtual bool RetainImageData();
	/*@}*/
};

#endif
