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

#ifndef _DEMAPANIMATIONVPS_H_
#define _DEMAPANIMATIONVPS_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>

class deAnimation;
class dearRule;
class dearVPSStateList;


/**
 * Map from rule vertex position set index to animation vertex position set indices.
 */
class dearMapAnimationVPS{
private:
	int *pIndices;
	int pCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create map. */
	dearMapAnimationVPS();
	
	/** Clean up map. */
	~dearMapAnimationVPS();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of entries. */
	inline int GetCount() const{ return pCount; }
	
	/** Clear map. */
	void Clear();
	
	/** Init map. */
	void Init( const dearRule &rule );
	
	/** Animation vertex position set index for rule vertex position set index or -1. */
	int GetAt( int ruleVPSIndex ) const;
	/*@}*/
};

#endif
