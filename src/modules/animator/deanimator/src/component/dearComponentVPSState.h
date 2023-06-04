/* 
 * Drag[en]gine Animator Module
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

#ifndef _DEARCOMPONENTVPSSTATE_H_
#define _DEARCOMPONENTVPSSTATE_H_

#include <dragengine/common/math/decMath.h>


/** Component vertex position set state. */
class dearComponentVPSState{
private:
	float pWeight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create component vertex position set state. */
	dearComponentVPSState();
	
	/** Clean up component vertex position set state. */
	~dearComponentVPSState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Weight. */
	inline float GetWeight() const{ return pWeight; }
	
	/** Set weight. */
	void SetWeight( float weight );
	/*@}*/
};

#endif
