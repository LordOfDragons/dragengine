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

#ifndef _MEPATHFINDTESTTYPE_H_
#define _MEPATHFINDTESTTYPE_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>



/**
 * @brief Path Find Test Type.
 */
class mePathFindTestType : public deObject{
private:
	decString pName;
	int pTypeNumber;
	float pFixCost;
	float pCostPerMeter;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new path find test type. */
	mePathFindTestType( int typeNumber );
	/** Cleans up the path find test type. */
	virtual ~mePathFindTestType();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the type number. */
	inline int GetTypeNumber() const{ return pTypeNumber; }
	/** Sets the type number. */
	void SetTypeNumber( int typeNumber );
	/** Retrieves the name of the type. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name of the type. */
	void SetName( const char *name );
	/** Retrieves the fix cost. */
	inline float GetFixCost() const{ return pFixCost; }
	/** Sets the fix cost. */
	void SetFixCost( float fixCost );
	/** Retrieves the cost per meter. */
	inline float GetCostPerMeter() const{ return pCostPerMeter; }
	/** Sets the cost per meter. */
	void SetCostPerMeter( float costPerMeter );
	/*@}*/
};

#endif
