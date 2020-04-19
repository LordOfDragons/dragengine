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

// include only once
#ifndef _DEBPPROPFIELDTYPE_H_
#define _DEBPPROPFIELDTYPE_H_

// includes
#include <dragengine/common/math/decMath.h>

// predefinitions
class debpPropField;

class dePropFieldType;

// definitions



/** Prop Field Bend State. */
struct debpPropFieldBendState{
	// this is now a bit crappy here. the random implementation of c++
	// left us behind. currently this is a hacked together and slightly
	// randomized ( due to the offset parameter ) sine curve. better
	// solution would be to use a global flucuation table and using one
	// single value to iterate through the table. this should provide
	// random enough behavior without costing too much time.
	decVector position;
	float flucDir;
	float flucStr;
};

/**
 * @brief Prop Field Type.
 *
 * Type in a prop field peer.
 */
class debpPropFieldType{
private:
	debpPropField *pPropField;
	dePropFieldType *pType;
	
	debpPropFieldBendState *pBendStates;
	int pBendStateCount;
	int pBendStateSize;
	
	float pFlucTimer;
	bool pDirty;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new type. */
	debpPropFieldType( debpPropField *propField, dePropFieldType *type );
	/** Cleans up the type. */
	~debpPropFieldType();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of bend states. */
	inline int GetBendStateCount() const{ return pBendStateCount; }
	/** Sets the size of states keeping the state values intact where possible. */
	void SetBendStateSize( int count );
	/** Retrieves the instances. */
	inline debpPropFieldBendState *GetBendStates() const{ return pBendStates; }
	
	/** Marks the type dirty. */
	void MarkDirty();
	
	/** Updates the bend states. */
	void Update( float elapsed );
	/*@}*/
};

// end of include only once
#endif
