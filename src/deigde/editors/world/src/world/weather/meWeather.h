/* 
 * Drag[en]gine IGDE World Editor
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

// include only once
#ifndef _MEWEATHER_H_
#define _MEWEATHER_H_

// includes

// predefinitions
class meWorld;



/**
 * @brief Weather.
 *
 * Manages the weather in a world. This is not a persistent class
 * that is saved to a file but is used to help visualize how the
 * world looks like in the game later on.
 */
class meWeather{
private:
	meWorld *pWorld;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new weather object. */
	meWeather( meWorld *world );
	/** Cleans up the weather object. */
	~meWeather();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the world. */
	inline meWorld *GetWorld() const{ return pWorld; }
	
	/** Updates the weather. */
	void Update( float elapsed );
	/*@}*/
};

// end of include only once
#endif
