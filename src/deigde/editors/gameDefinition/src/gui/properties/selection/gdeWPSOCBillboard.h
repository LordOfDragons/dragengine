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

#ifndef _GDEWPSOCBILLBOARD_H_
#define _GDEWPSOCBILLBOARD_H_

#include "../../../gamedef/objectClass/billboard/gdeOCBillboard.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "gdeWPSOCBillboardListener.h"

#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVector2.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class gdeObjectClass;
class gdeOCBillboard;
class gdeWindowProperties;



/**
 * \brief Object class billboard property panel.
 */
class gdeWPSOCBillboard : public igdeContainerScroll{
public:
	typedef deTObjectReference<gdeWPSOCBillboard> Ref;
	
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSOCBillboardListener::Ref pListener;
	
	gdeGameDefinition::Ref pGameDefinition;
	
	igdeEditPath::Ref pEditPathSkin;
	igdeEditVector::Ref pEditAxis;
	igdeEditVector2::Ref pEditSize;
	igdeEditVector2::Ref pEditOffset;
	igdeCheckBox::Ref pChkLocked;
	igdeCheckBox::Ref pChkSpherical;
	igdeCheckBox::Ref pChkRelativeSize;
	igdeCheckBox::Ref pChkDoNotScale;
	igdeCheckBox::Ref pChkRenderEnvMap;
	igdeCheckBox::Ref pChkPartialHide;
	igdeEditVector::Ref pEditPosition;
	igdeTextField::Ref pEditBoneName;
	igdeComboBox::Ref pCBPropertyNames;
	igdeComboBoxFilter::Ref pCBPropertyNameTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSOCBillboard(gdeWindowProperties &windowMain);
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSOCBillboard();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Game definition or \em nullptr if not set. */
	inline const gdeGameDefinition::Ref &GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em nullptr if not set. */
	void SetGameDefinition(gdeGameDefinition *gameDefinition);
	
	
	
	/** \brief Active object class or \em nullptr if absent. */
	gdeObjectClass *GetObjectClass() const;
	
	/** \brief Active object class billboard or \em nullptr if not set. */
	gdeOCBillboard *GetBillboard() const;
	
	/** \brief Selected property name. */
	const gdeOCBillboard::eProperties GetPropertyName() const;
	
	
	
	/** \brief Update object class property list. */
	void UpdatePropertyList();
	
	
	
	/** \brief Update billboard. */
	void UpdateBillboard();
	
	/** \brief Update billboard property name. */
	void UpdatePropertyName();
	/*@}*/
};

#endif
