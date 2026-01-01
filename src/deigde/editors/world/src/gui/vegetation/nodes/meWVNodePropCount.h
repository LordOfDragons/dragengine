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

#ifndef _MEWVNODEPROPCOUNT_H_
#define _MEWVNODEPROPCOUNT_H_

#include "meWVNode.h"

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeActionContextMenu.h>

class meHTVRulePropCount;



/**
 * \brief Vegetation Editing Window Node Prop Count.
 */
class meWVNodePropCount : public meWVNode{
public:
	typedef deTObjectReference<meWVNodePropCount> Ref;
	
private:
	meHTVRulePropCount *pRulePC;
	
	igdeActionContextMenu::Ref pActionMenuClass;
	
	igdeContainer::Ref pFraParameters;
	igdeComboBoxFilter::Ref pCBPropClass;
	igdeButton::Ref pBtnPropClass;
	igdeTextField::Ref pEditSearchRadius;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meWVNodePropCount(meWindowVegetation &windowVegetation, meHTVRulePropCount *rule);
	
protected:
	/** \brief Clean up object. */
	~meWVNodePropCount() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rule. */
	inline meHTVRulePropCount *GetRulePropCount() const{ return pRulePC; }
	
	/** \brief Update node. */
	void Update() override;
	
	/** \brief Update class lists. */
	void UpdateClassLists() override;
	
	
	/** \brief Game project game definition changed. */
	void OnGameDefinitionChanged() override;
	/*@}*/
};

#endif
