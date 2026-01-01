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

#ifndef _MEHTVRSLOT_H_
#define _MEHTVRSLOT_H_

#include "meHTVRLink.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>

class meHTVRule;



/**
 * Height Terrain Vegetation Rule Slot.
 */
class meHTVRSlot : public deObject{
public:
	typedef deTObjectReference<meHTVRSlot> Ref;
	typedef decTObjectOrderedSet<meHTVRSlot> List;
	
	
private:
	bool pIsInput;
	meHTVRLink::List pLinks;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create input slot. */
	meHTVRSlot();
	
protected:
	/** Clean up input slot. */
	~meHTVRSlot();
	
public:
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Is input slot. */
	inline bool GetIsInput() const{ return pIsInput; }
	
	/** Set is input slot. */
	void SetIsInput(bool isInput);
	
	/** Links. */
	inline const meHTVRLink::List &GetLinks() const{ return pLinks; }
	
	/** Add link. */
	void AddLink(meHTVRLink *link);
	
	/** Remove link. */
	void RemoveLink(meHTVRLink *link);
	
	/** Remove all links. */
	void RemoveAllLinks();
	
	/** Link with the given source node and slot. */
	bool HasLinkWithSource(const meHTVRule *rule, int slot) const;
	
	/** Link with the given source node and slot or nullptr if not found. */
	meHTVRLink *GetLinkWithSource(const meHTVRule *rule, int slot) const;
	
	/** Link with the given destination node and slot. */
	bool HasLinkWithDestination(const meHTVRule *rule, int slot) const;
	
	/** Link with the given destination node and slot or nullptr if not found. */
	meHTVRLink *GetLinkWithDestination(const meHTVRule *rule, int slot) const;
	/*@}*/
};

#endif
