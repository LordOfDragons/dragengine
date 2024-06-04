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

#ifndef _MEWVNODESLOT_H_
#define _MEWVNODESLOT_H_

#include <deigde/gui/nodeview/igdeNVSlot.h>


class meWVNode;


/**
 * \brief Vegetation Edit Node Slot.
 */
class meWVNodeSlot : public igdeNVSlot{
public:
	/** \brief Slot Types. */
	enum eSlotTypes{
		estValue,
		estVector
	};
	
	
	
private:
	meWVNode &pParentNode;
	const eSlotTypes pType;
	const int pRuleSlot;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create slot. */
	meWVNodeSlot( igdeEnvironment &environment, const char *text, const char *description,
		bool isInput, meWVNode &node, eSlotTypes type, int ruleSlot );
	
protected:
	/** \brief Clean up slot. */
	virtual ~meWVNodeSlot();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parent node. */
	inline meWVNode &GetParentNode() const{ return pParentNode; }
	
	/** \brief Slot type. */
	inline eSlotTypes GetType() const{ return pType; }
	
	/** \brief Slot number in the rule the parent node belongs to. */
	inline int GetRuleSlot() const{ return pRuleSlot; }
	/*@}*/
};

#endif
