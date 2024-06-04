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

#ifndef _DEOGLPROPFIELDTYPE_H_
#define _DEOGLPROPFIELDTYPE_H_

#include <dragengine/common/math/decMath.h>

class deoglPropField;
class deoglPropFieldCluster;
class deoglRPropFieldType;

class dePropFieldType;



/**
 * Prop Field Type.
 * <date 2015
 */
class deoglPropFieldType{
private:
	deoglPropField &pPropField;
	const dePropFieldType &pType;
	
	deoglRPropFieldType *pRType;
	
	bool pDirtyType;
	bool pDirtyInstances;
	bool pDirtyBendStates;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create prop field type. */
	deoglPropFieldType( deoglPropField &propField, const dePropFieldType &type );
	
	/** Clean up prop field type. */
	~deoglPropFieldType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Prop field. */
	inline deoglPropField &GetPropField() const{ return pPropField; }
	
	/** Prop field type. */
	inline const dePropFieldType &GetType() const{ return pType; }
	
	/** Render prop field type. */
	inline deoglRPropFieldType *GetRType() const{ return pRType; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** Type changed. */
	void TypeChanged();
	
	/** Instances changed. */
	void InstancesChanged();
	
	/** Instances to Bend States assignments changed. */
	void AssignmentsChanged();
	
	/** Bend States changed. */
	void BendStatesChanged();
	
	/** Instance count changed. */
	void InstanceCountChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
