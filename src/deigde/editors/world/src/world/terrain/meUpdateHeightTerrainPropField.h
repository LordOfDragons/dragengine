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
#ifndef _MEUPDATEHEIGHTTERRAINPROPFIELDS_H_
#define _MEUPDATEHEIGHTTERRAINPROPFIELDS_H_

// includes
#include <dragengine/systems/modules/scripting/deBaseScriptingPropField.h>

// predefinitions
class meHeightTerrainPropField;
class meByteArray;

class dePropFieldGround;



/**
 * @brief Update Height Terrain Prop Fields.
 *
 * Listener used to update the prop fields of a height terrain.
 */
class meUpdateHeightTerrainPropField : public deBaseScriptingPropField{
private:
	meHeightTerrainPropField *pPropField;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a object. */
	meUpdateHeightTerrainPropField( meHeightTerrainPropField *propField );
	/** Cleans up the object. */
	virtual ~meUpdateHeightTerrainPropField();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the prop field. */
	inline meHeightTerrainPropField *GetPropField() const{ return pPropField; }
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/**
	 * Requests the game scripts to fill the prop field with instances. The density
	 * indicates the percentage of instances to use compared to a fully populated
	 * prop field.
	 */
	virtual void CreateInstances( float density );
	/*@}*/
	
private:
	void pProjectToGround();
	float pGetMaskDensity( const meByteArray *mask, float positionX, float positionY ) const;
};

// end of include only once
#endif
