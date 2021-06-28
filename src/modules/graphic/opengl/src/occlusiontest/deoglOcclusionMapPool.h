/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLOCCLUSIONMAPPOOL_H_
#define _DEOGLOCCLUSIONMAPPOOL_H_

#include <dragengine/common/collection/decPointerList.h>

class deoglRenderThread;
class deoglOcclusionMap;


/**
 * Occlusion map pool.
 */
class deoglOcclusionMapPool{
private:
	deoglRenderThread &pRenderThread;
	
	decPointerList pOcclusionMaps;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create ooclusion mpa manager. */
	deoglOcclusionMapPool( deoglRenderThread &renderThread );
	
	/** Clean up occlusion map manager. */
	~deoglOcclusionMapPool();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Get occlusion map for size. */
	deoglOcclusionMap *Get( int width, int height );
	
	/** Return occlusion map. */
	void Return( deoglOcclusionMap *occlusionMap );
	/*@}*/
};

#endif
