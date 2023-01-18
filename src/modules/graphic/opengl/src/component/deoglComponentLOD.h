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

#ifndef _DEOGLCOMPONENTLOD_H_
#define _DEOGLCOMPONENTLOD_H_

class deoglRComponentLOD;
class deoglComponent;


/**
 * Component LOD.
 */
class deoglComponentLOD{
public:
	deoglComponent &pComponent;
	const int pLODIndex;
	
	deoglRComponentLOD *pRLOD;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create component lod. */
	deoglComponentLOD( deoglComponent &component, int lodIndex );
	
	/** Clean up component lod. */
	~deoglComponentLOD();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent component. */
	inline deoglComponent &GetComponent() const{ return pComponent; }
	
	/** LOD index. */
	inline int GetLODIndex() const{ return pLODIndex; }
	
	/** Render component lod. */
	inline deoglRComponentLOD *GetRLOD() const{ return pRLOD; }
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
