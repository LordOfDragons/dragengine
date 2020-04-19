/* 
 * Drag[en]gine SmallTalk Script Module
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
#ifndef _STCLASSCOLOR_H_
#define _STCLASSCOLOR_H_

// includes
#include "../../stCommon.h"
#include <dragengine/common/math/decMath.h>

// predefinitions
class ScriptingSmalltalk;



/**
 * @brief Class Color.
 *
 * Color class.
 */
class stClassColor{
private:
	ScriptingSmalltalk *pST;
	OOP pOOPClass;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new class. */
	stClassColor( ScriptingSmalltalk *st );
	/** Cleans up the class. */
	virtual ~stClassColor();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline ScriptingSmalltalk *GetST() const{ return pST; }
	
	/** Set up links. */
	void SetUpLinks();
	/** Retrieves the color from an object. */
	const decColor &OOPToColor( OOP object ) const;
	/** Push render target object. */
	OOP ColorToOOP( const decColor &color );
	/*@}*/
	
	/** @name cCall Methods */
	/*@{*/
	/** Creates a new color object. */
	static OOP ccNew1( OOP self, double red, double green, double blue );
	/** Creates a new color object. */
	static OOP ccNew2( OOP self, double red, double green, double blue, double alpha );
	/** Creates a new color object. */
	static OOP ccNewInt1( OOP self, int red, int green, int blue );
	/** Creates a new color object. */
	static OOP ccNewInt2( OOP self, int red, int green, int blue, int alpha );
	
	/** Retrieves the red component. */
	static double ccRed( OOP self );
	/** Retrieves the green component. */
	static double ccGreen( OOP self );
	/** Retrieves the blue component. */
	static double ccBlue( OOP self );
	/** Retrieves the alpha component. */
	static double ccAlpha( OOP self );
	/** Retrieves a the color clamped to the range from 0 to 1. */
	static OOP ccClamp( OOP self );
	/** Determines if the color is opaque ( alpha is 1 ). */
	static bool ccOpaque( OOP self );
	/** Determines if the color is transparent ( alpha is between 0 and 1 ). */
	static bool ccTransparent( OOP self );
	/** Determines if the color is invisible ( alpha is 0 ). */
	static bool ccInvisible( OOP self );
	/** Creates a copy of the color. */
	static OOP ccCopy( OOP self );
	
	/** Determines if the color is equal to another color. */
	static bool ccEqual( OOP self, OOP other );
	/** Retrieves this color plus component wise to another color. */
	static OOP ccAdd( OOP self, OOP other );
	/** Retrieves this color minus component wise another color. */
	static OOP ccSubtract( OOP self, OOP other );
	/** Retrieves this color multiplied component wise with another color. */
	static OOP ccMultiply( OOP self, OOP other );
	/** Retrieves this color divided component wise with another color. */
	static OOP ccDivide( OOP self, OOP other );
	/** Retrieves this color scaled each component by a scalar. */
	static OOP ccScalarMultiply( OOP self, double scalar );
	/** Retrieves this color divided each component by a scalar. */
	static OOP ccScalarDivide( OOP self, double scalar );
	
	/** Retrieves the hash code. */
	static int ccHash( OOP self );
	/** Retrieve string representation. */
	static OOP ccAsString( OOP self );
	/*@}*/
};

#endif
