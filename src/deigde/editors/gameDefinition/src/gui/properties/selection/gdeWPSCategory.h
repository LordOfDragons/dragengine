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

#ifndef _GDEWPSCATEGORY_H_
#define _GDEWPSCATEGORY_H_

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

#include "../../../undosys/category/gdeUCategoryBase.h"

class gdeWindowProperties;
#include "gdeWPSCategoryListener.h"
class gdeCategory;
#include "../../../gamedef/gdeGameDefinition.h"



/**
 * \brief Category property panel.
 */
class gdeWPSCategory : public igdeContainerScroll{
public:
	typedef deTObjectReference<gdeWPSCategory> Ref;
	
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSCategoryListener::Ref pListener;
	
	gdeGameDefinition::Ref pGameDefinition;
	
	igdeTextField::Ref pEditName;
	igdeTextArea::Ref pEditDescription;
	igdeWidget::Ref pAutoCategorizePattern;
	igdeCheckBox::Ref pChkHidden;
	
	igdeListBox::Ref pListElements;
	igdeButton::Ref pBtnJumpToElement;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSCategory(gdeWindowProperties &windowProperties);
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSCategory();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Game definition or \em nullptr if not set. */
	inline const gdeGameDefinition::Ref &GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em nullptr if not set. */
	void SetGameDefinition(gdeGameDefinition *gameDefinition);
	
	
	
	/** \brief Category or \em nullptr if not set. */
	gdeCategory *GetCategory() const;
	
	/** \brief Category type if category is set. */
	gdeUCategoryBase::eCategoryType GetCategoryType() const;
	
	
	
	/** \brief Update category. */
	void UpdateCategory();
	
	/** \brief Update list of elements using this category. */
	void UpdateListElements();
	/*@}*/
};

#endif
 
