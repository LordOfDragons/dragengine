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

#ifndef _DEDSRENDERABLEVALUE_H_
#define _DEDSRENDERABLEVALUE_H_

#include "deDSRenderable.h"


/**
 * \brief Dynamic Skin Renderable Value.
 *
 * The entire texture is defined by one single value for all pixels
 * and all pixel components. Use this type if the renderable is used
 * as the source of a Value Type Skin Property. If used for an Image
 * Type Skin Property the dimension of the texture is undefined but
 * usually very small ( possibly down to 1x1 ).
 */
class deDSRenderableValue : public deDSRenderable{
private:
	float pValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new renderable. */
	deDSRenderableValue( const char *name );
	
	/** \brief Clean up renderable. */
	virtual ~deDSRenderableValue();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Value. */
	inline float GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue( float value );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the renderable. */
	virtual void Visit( deDSRenderableVisitor &visitor );
};

#endif
