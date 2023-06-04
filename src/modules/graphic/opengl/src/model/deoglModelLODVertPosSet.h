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

#ifndef _DEOGLMODELLODVERTPOSSET_H_
#define _DEOGLMODELLODVERTPOSSET_H_

#include <dragengine/common/math/decMath.h>


/**
 * Model LOD mesh vertex position set.
 */
class deoglModelLODVertPosSet{
public:
	struct sPosition{
		decVector position;
		int index;
	};
	
	int pVBOOffset;
	
	sPosition *pPositions;
	int pPositionCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create vertex position set. */
	deoglModelLODVertPosSet();
	
	/** Clean up vertex position set. */
	~deoglModelLODVertPosSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** VBO offset. */
	inline int GetVBOOffset() const{ return pVBOOffset; }
	
	/** Set VBO offset. */
	void SetVBOOffset( int offset );
	
	/** Positions. */
	inline sPosition *GetPositions() const{ return pPositions; }
	
	/** Count of positions. */
	inline int GetPositionCount() const{ return pPositionCount; }
	
	/** Set count of positions. */
	void SetPositionCount( int count );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
