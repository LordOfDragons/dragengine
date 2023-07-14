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

#ifndef _DESKINPROPERTYNODEIMAGE_H_
#define _DESKINPROPERTYNODEIMAGE_H_

#include "deSkinPropertyNode.h"
#include "../../../image/deImage.h"
#include "../../../../common/string/decString.h"


/**
 * \brief Skin property image node for constructed texture property.
 */
class DE_DLL_EXPORT deSkinPropertyNodeImage : public deSkinPropertyNode{
private:
	decString pPath;
	deImage::Ref pImage;
	decPoint pRepeat;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create constructed property node. */
	deSkinPropertyNodeImage();
	
	/** \brief Clean up constructed property node. */
	virtual ~deSkinPropertyNodeImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Image path or empty string if not set. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set image path or empty path if not set. */
	void SetPath( const char *path );
	
	/** \brief Image or nullptr if not set. */
	inline const deImage::Ref &GetImage() const{ return pImage; }
	
	/** \brief Set image or nullptr if not set. */
	void SetImage( deImage *image );
	
	/** \brief Repeat count. */
	inline const decPoint &GetRepeat() const{ return pRepeat; }
	
	/**
	 * \brief Set repeat count.
	 * \throws deeInvalidParam One or more components of \em count are less than 1.
	 */
	void SetRepeat( const decPoint &count );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits node. */
	virtual void Visit( deSkinPropertyNodeVisitor &visitor );
	/*@}*/
};

#endif
