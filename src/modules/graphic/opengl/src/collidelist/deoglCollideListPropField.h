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

#ifndef _DEOGLCOLLIDELISTPROPFIELD_H_
#define _DEOGLCOLLIDELISTPROPFIELD_H_

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

class deoglRPropField;
class deoglRPropFieldType;
class deoglCollideListPropFieldType;
class deoglOcclusionTest;


/**
 * Collide List Prop Field.
 */
class deoglCollideListPropField{
private:
	deoglRPropField *pPropField;
	
	decPointerList pTypes;
	int pTypeCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create collide list prop field. */
	deoglCollideListPropField();
	
	/** Clean up collide list prop field. */
	~deoglCollideListPropField();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Clear collide list prop field. */
	void Clear();
	
	/** Start occlusion test. */
	void StartOcclusionTest( deoglOcclusionTest &occlusionTest, const decDVector &referencePosition );
	
	
	
	/** Prop field is empty. */
	bool GetIsEmpty() const;
	
	/** Prop field. */
	inline deoglRPropField *GetPropField() const{ return pPropField; }
	
	/** Set prop field. */
	void SetPropField( deoglRPropField *propField );
	
	/** Count of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	
	/** Type at the index. */
	deoglCollideListPropFieldType *GetTypeAt( int index ) const;
	
	/** Add type. */
	deoglCollideListPropFieldType *AddType( deoglRPropFieldType *type );
	
	/** Remove all types. */
	void RemoveAllTypes();
	/*@}*/
};

#endif
