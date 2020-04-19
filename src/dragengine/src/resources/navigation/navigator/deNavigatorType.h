/* 
 * Drag[en]gine Game Engine
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

#ifndef _DENAVIGATORTYPE_H_
#define _DENAVIGATORTYPE_H_


/**
 * \brief Navigator Type.
 * Defines the costs to apply during path finding for all navigation spaces with a given type number.
 */
class deNavigatorType{
private:
	int pType;
	float pFixCost;
	float pCostPerMeter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new navigator type. */
	deNavigatorType();
	
	/** \brief Clean up navigator type. */
	~deNavigatorType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Type number. */
	inline int GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType( int type );
	
	/** \brief Fix cost. */
	inline float GetFixCost() const{ return pFixCost; }
	
	/** \brief Set fix cost. */
	void SetFixCost( float cost );
	
	/** \brief Retrieves cost per meter. */
	inline float GetCostPerMeter() const{ return pCostPerMeter; }
	
	/** \brief Set cost per meter. */
	void SetCostPerMeter( float costPerMeter );
	/*@}*/
};

#endif
