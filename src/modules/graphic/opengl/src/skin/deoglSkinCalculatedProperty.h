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

#ifndef _DEOGLSKINCALCULATEDPROPERTY_H_
#define _DEOGLSKINCALCULATEDPROPERTY_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/skin/property/deSkinPropertyMapped.h>


class deoglSkinState;
class deoglSkinStateCalculated;
class deoglRComponent;

class decCurveBezierEvaluator;


/**
 * Skin calculated property.
 */
class deoglSkinCalculatedProperty : public deObject{
public:
	typedef deTObjectReference<deoglSkinCalculatedProperty> Ref;
	
	
private:
	int pMappedComponent[4];
	bool pRequiresTexture;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new skin calculated property. */
	deoglSkinCalculatedProperty();
	
protected:
	/** Cleans up the skin calculated property. */
	virtual ~deoglSkinCalculatedProperty();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Mapped component. */
	int GetMappedComponent(int index) const;
	
	/** Set mapped component. */
	void SetMappedComponent(int index, int mapped);
	
	/** Requires texture. */
	inline bool GetRequiresTexture() const{ return pRequiresTexture; }
	
	/** Set if texture is required. */
	void SetRequiresTexture(bool requiresTexture);
	
	/** Calculated value. */
	decColor Calculate(const deoglSkinState &skinState) const;
	/*@}*/
};

#endif
