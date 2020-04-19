/* 
 * Drag[en]gine Bullet Physics Module
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

#ifndef _DEBPFORCEFIELDFLUCTUATION_H_
#define _DEBPFORCEFIELDFLUCTUATION_H_

#include <dragengine/common/math/decMath.h>



/**
 * \brief Force field fluctuation simulation.
 *
 * Simulates the fluctuation of a force field.
 */
class debpForceFieldFluctuation{
private:
	float pDirection;
	float pStrength;
	float pTimer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create force field fluctuation simulation. */
	debpForceFieldFluctuation();
	
	/** \brief Clean up vortex. */
	~debpForceFieldFluctuation();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Fluctuation direction. */
	inline float GetDirection() const{ return pDirection; }
	
	/** \brief Flucutation strength. */
	inline float GetStrength() const{ return pStrength; }
	
	
	
	/** \brief Reset. */
	void Reset();
	
	/** \brief Update. */
	void Update( float elapsed );
	/*@}*/
};

#endif
