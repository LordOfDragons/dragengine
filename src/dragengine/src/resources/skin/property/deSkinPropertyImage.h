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

#ifndef _DESKINPROPERTYIMAGE_H_
#define _DESKINPROPERTYIMAGE_H_

#include "deSkinProperty.h"
#include "../../image/deImageReference.h"
#include "../../../common/string/decString.h"


/**
 * \brief Skin texture image property.
 */
class deSkinPropertyImage : public deSkinProperty{
private:
	decString pPath;
	deImageReference pImage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create skin image property. */
	deSkinPropertyImage( const char *type );
	
	/** \brief Clean up skin property. */
	virtual ~deSkinPropertyImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Image path. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set image path. */
	void SetPath( const char *path );
	
	/** \brief Image. */
	inline deImage *GetImage() const{ return pImage; }
	
	/** \brief Set image. */
	void SetImage( deImage *image );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit property. */
	virtual void Visit( deSkinPropertyVisitor &visitor );
	/*@}*/
};

#endif
