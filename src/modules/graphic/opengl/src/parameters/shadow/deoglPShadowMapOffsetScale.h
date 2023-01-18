/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLPSHADOWMAPOFFSETSCALE_H_
#define _DEOGLPSHADOWMAPOFFSETSCALE_H_

#include "../deoglParameterFloat.h"



/**
 * Shadow Map Offset Scale Parameter.
 */
class deoglPShadowMapOffsetScale : public deoglParameterFloat{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new parameter. */
	deoglPShadowMapOffsetScale( deGraphicOpenGl &ogl );
	/** Cleans up the parameter. */
	virtual ~deoglPShadowMapOffsetScale();
	/*@}*/
	
	/** \name Parameter Value */
	/*@{*/
	/** Retrieves the current value. */
	virtual float GetParameterFloat();
	/** Sets the current value. */
	virtual void SetParameterFloat( float value );
	/*@}*/
};

#endif
