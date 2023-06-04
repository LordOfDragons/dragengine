/* 
 * Drag[en]gine Animator Module
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

#ifndef _DEARCONTROLLERTARGET_H_
#define _DEARCONTROLLERTARGET_H_

#include <dragengine/common/math/decMath.h>

class deAnimatorControllerTarget;
class dearAnimatorInstance;
class dearControllerStates;


/**
 * Animator controller target.
 */
class dearControllerTarget{
private:
	int *pLinks;
	int pLinkCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create controller target. */
	dearControllerTarget( const deAnimatorControllerTarget &target, int firstLink );
	
	/** Clean up controller target. */
	~dearControllerTarget();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Number of links. */
	inline int GetLinkCount() const{ return pLinkCount; }
	
	/** Link at index. */
	int GetLinkAt( int index ) const;
	
	
	
	/** Value of target. */
	float GetValue( const dearAnimatorInstance &instance, float defaultValue ) const;
	
	/** Vector of target. */
	void GetVector( const dearAnimatorInstance &instance, decVector &vector ) const;
	
	/** Quaternion of target. */
	void GetQuaternion( const dearAnimatorInstance &instance, decQuaternion &quaternion ) const;
	/*@}*/
};

#endif
