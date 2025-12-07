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

#ifndef _FBXANIMATIONCURVE_H_
#define _FBXANIMATIONCURVE_H_


#include <stdint.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class fbxNode;
class fbxProperty;


/**
 * \brief FBX managed animation curve.
 */
class fbxAnimationCurve : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<fbxAnimationCurve> Ref;
	
	
private:
	fbxNode &pNodeCurve;
	fbxProperty *pPropertyTime;
	fbxProperty *pPropertyValue;
	decVector pDefaultValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create move. */
	fbxAnimationCurve(fbxNode &nodeCurve);
	
protected:
	/** \brief Clean up move. */
	virtual ~fbxAnimationCurve();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Curve node. */
	inline fbxNode &GetNodeCurve() const{ return pNodeCurve; }
	
	/** \brief Time property. */
	inline fbxProperty &GetPropertyTime() const{ return *pPropertyTime; }
	
	/** \brief Value property. */
	inline fbxProperty &GetPropertyValue() const{ return *pPropertyValue; }
	
	/** \brief Default value. */
	inline const decVector &GetDefaultValue() const{ return pDefaultValue; }
	/*@}*/
};

#endif
