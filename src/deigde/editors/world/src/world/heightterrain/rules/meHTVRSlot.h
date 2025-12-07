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

#include <dragengine/common/collection/decObjectOrderedSet.h>

class meHTVRLink;
class meHTVRule;



/**
 * \brief Height Terrain Vegetation Rule Slot.
 */
class meHTVRSlot{
private:
	bool pIsInput;
	decObjectOrderedSet pLinks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create input slot. */
	meHTVRSlot();
	
	/** \brief Clean up input slot. */
	~meHTVRSlot();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Is input slot. */
	inline bool GetIsInput() const{return pIsInput;}
	
	/** \brief Set is input slot. */
	void SetIsInput(bool isInput);
	
	/** \brief Number of links. */
	int GetLinkCount() const;
	
	/** \brief Link at the given index. */
	meHTVRLink *GetLinkAt(int index) const;
	
	/** \brief Index of the link or -1 if not found. */
	int IndexOfLink(meHTVRLink *link) const;
	
	/** \brief Link exists. */
	bool HasLink(meHTVRLink *link) const;
	
	/** \brief Add link. */
	void AddLink(meHTVRLink *link);
	
	/** \brief Remove link. */
	void RemoveLink(meHTVRLink *link);
	
	/** \brief Remove all links. */
	void RemoveAllLinks();
	
	/** \brief Link with the given source node and slot. */
	bool HasLinkWithSource(meHTVRule *rule, int slot) const;
	
	/** \brief Link with the given source node and slot or NULL if not found. */
	meHTVRLink *GetLinkWithSource(meHTVRule *rule, int slot) const;
	
	/** \brief Link with the given destination node and slot. */
	bool HasLinkWithDestination(meHTVRule *rule, int slot) const;
	
	/** \brief Link with the given destination node and slot or NULL if not found. */
	meHTVRLink *GetLinkWithDestination(meHTVRule *rule, int slot) const;
	/*@}*/
};

#endif
