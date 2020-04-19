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

#ifndef _DEOGLPFRAMERATELIMIT_H_
#define _DEOGLPFRAMERATELIMIT_H_

#include "../deoglParameterInt.h"


/**
 * \brief Frame rate limit parameter.
 */
class deoglPFrameRateLimit : public deoglParameterInt{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create parameter. */
	deoglPFrameRateLimit( deGraphicOpenGl &ogl );
	
	/** \brief Clean up parameter. */
	virtual ~deoglPFrameRateLimit();
	/*@}*/
	
	
	
	/** \name Parameter Value */
	/*@{*/
	/** \brief Current value. */
	virtual int GetParameterInt();
	
	/** \brief Set current value. */
	virtual void SetParameterInt( int value );
	/*@}*/
};

#endif
