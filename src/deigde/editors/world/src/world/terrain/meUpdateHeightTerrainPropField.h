/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
	meUpdateHeightTerrainPropField(meHeightTerrainPropField *propField);
	/** Cleans up the object. */
	virtual ~meUpdateHeightTerrainPropField();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the prop field. */
	inline meHeightTerrainPropField *GetPropField() const{return pPropField;}
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/**
	 * Requests the game scripts to fill the prop field with instances. The density
	 * indicates the percentage of instances to use compared to a fully populated
	 * prop field.
	 */
	virtual void CreateInstances(float density);
	/*@}*/
	
private:
	void pProjectToGround();
	float pGetMaskDensity(const meByteArray *mask, float positionX, float positionY) const;
};

// end of include only once
#endif
