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

#ifndef _DEOGLCAPCHECKCLEARENTIRECUBEMAP_H_
#define _DEOGLCAPCHECKCLEARENTIRECUBEMAP_H_

#include "../../deoglBasics.h"

class deoglCapabilities;



/**
 * Capability check clear entire cube map.
 */
class deoglCapCheckClearEntireCubeMap{
private:
	deoglCapabilities &pCapabilities;
	bool pWorking;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create capability check. */
	deoglCapCheckClearEntireCubeMap( deoglCapabilities &capabilities );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Clear entire cube map is working correctly. */
	inline bool Working() const{ return pWorking; }
	
	/** Clear entire cube map is not working correctly. */
	inline bool Broken() const{ return ! pWorking; }
	
	/** Run check. */
	void Check( GLuint fbo );
	/*@}*/
};

#endif
