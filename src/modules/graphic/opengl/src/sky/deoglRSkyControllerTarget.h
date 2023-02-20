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
#ifndef _DEOGLRSKYCONTROLLERTARGET_H_
#define _DEOGLRSKYCONTROLLERTARGET_H_

#include <dragengine/common/math/decMath.h>

class deSkyControllerTarget;
class deoglRSkyInstance;


/**
 * Sky controller target.
 */
class deoglRSkyControllerTarget{
private:
	int *pLinks;
	int pLinkCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create sky controller target. */
	deoglRSkyControllerTarget( const deSkyControllerTarget &target );
	
	/** Clean up sky controller target. */
	~deoglRSkyControllerTarget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of links. */
	inline int GetLinkCount() const{ return pLinkCount; }
	
	/** Link at index. */
	int GetLinkAt( int index ) const;
	
	/** Value of target. */
	float GetValue( const deoglRSkyInstance &instance, float defaultValue ) const;
	/*@}*/
};

#endif
