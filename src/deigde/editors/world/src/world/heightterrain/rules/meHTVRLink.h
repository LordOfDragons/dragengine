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
#ifndef _MEHTVRLINK_H_
#define _MEHTVRLINK_H_

// includes
#include <dragengine/deObject.h>

// predefinitions
class meHTVRule;



/**
 * @brief Height Terrain Vegetation Rule Link.
 *
 * Link between two height terrain vegeation rules.
 */
class meHTVRLink : public deObject{
private:
	meHTVRule *pSrcRule;
	int pSrcSlot;
	meHTVRule *pDestRule;
	int pDestSlot;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meHTVRLink> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a link. */
	meHTVRLink(meHTVRule *sourceRule, int sourceSlot, meHTVRule *destinationRule, int destinationSlot);
	/** Cleans up the link. */
	virtual ~meHTVRLink();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the source rule. */
	inline meHTVRule *GetSourceRule() const{ return pSrcRule; }
	/** Retrieves the source slot. */
	inline int GetSourceSlot() const{ return pSrcSlot; }
	/** Retrieves the destination rule. */
	inline meHTVRule *GetDestinationRule() const{ return pDestRule; }
	/** Retrieves the destination slot. */
	inline int GetDestinationSlot() const{ return pDestSlot; }
	/*@}*/
};

// end of include only once
#endif
