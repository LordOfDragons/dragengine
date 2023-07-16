/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLSKINCONSTRUCTEDPROPERTY_H_
#define _DEOGLSKINCONSTRUCTEDPROPERTY_H_

#include "state/node/deoglSkinStateCNGroup.h"

#include <dragengine/resources/skin/property/deSkinPropertyConstructed.h>


class deoglSkinStateConstructed;


/**
 * Skin constructed property.
 */
class deoglSkinConstructedProperty : public deObject{
public:
	typedef deTObjectReference<deoglSkinConstructedProperty> Ref;
	
private:
	const deoglSkinStateCNGroup::Ref pContent;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new skin constructed property. */
	deoglSkinConstructedProperty( deoglSkinStateCNGroup *content );
	
protected:
	/** Cleans up the skin constructed property. */
	virtual ~deoglSkinConstructedProperty();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Node. */
	inline const deoglSkinStateCNGroup &GetContent() const{ return pContent; }
	/*@}*/
};

#endif
