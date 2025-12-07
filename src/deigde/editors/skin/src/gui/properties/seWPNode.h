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

#ifndef _SEWPNODE_H_
#define _SEWPNODE_H_

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPoint.h>
#include <deigde/gui/composed/igdeEditPoint3.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

#include "../../skin/property/node/sePropertyNode.h"

class seSkin;
class seTexture;
class seProperty;
class seWindowProperties;
class seWPNodeListener;
class igdeTreeItem;


/**
 * Node panel.
 */
class seWPNode : public igdeContainerScroll{
private:
	seWindowProperties &pWindowProperties;
	seWPNodeListener *pListener;
	
	seSkin *pSkin;
	bool pPreventUpdate;
	
	igdeTreeList::Ref pTreeOutline;
	igdeTextField::Ref pLabSelection;
	
	igdeEditPoint3::Ref pEditPosition;
	igdeEditPoint3::Ref pEditSize;
	igdeTextField::Ref pEditRotation;
	igdeTextField::Ref pEditShear;
	igdeTextField::Ref pEditBrightness;
	igdeTextField::Ref pEditContrast;
	igdeTextField::Ref pEditGamma;
	igdeColorBox::Ref pClrColorize;
	igdeEditSliderText::Ref pSldTransparency;
	igdeTextField::Ref pLabMask;
	igdeComboBox::Ref pCBCombineMode;
	
	igdeSwitcher::Ref pSwitcher;
	
	igdeEditPath::Ref pImageEditImage;
	igdeEditPoint::Ref pImageEditRepeat;
	igdeTextField::Ref pImageLabImageInfo;
	
	igdeComboBox::Ref pShapeCBType;
	igdeColorBox::Ref pShapeClrFill;
	igdeColorBox::Ref pShapeClrLine;
	igdeTextField::Ref pShapeEditThickness;
	
	igdeEditPath::Ref pTextEditFont;
	igdeTextField::Ref pTextLabFontInfo;
	igdeTextField::Ref pTextEditFontSize;
	igdeTextField::Ref pTextEditText;
	igdeColorBox::Ref pTextClrColor;
	
	igdeComboBox::Ref pCBMappedType;
	igdeComboBox::Ref pCBMappedTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	seWPNode(seWindowProperties &windowProperties);
	
protected:
	/** Clean up panel. */
	virtual ~seWPNode();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Window properties. */
	inline seWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** Skin. */
	inline seSkin *GetSkin() const{ return pSkin; }
	
	/** Set skin. */
	void SetSkin(seSkin *skin);
	
	/** Skin path changed. */
	void OnSkinPathChanged();
	
	/** Active texture or \em NULL if none. */
	seTexture *GetTexture() const;
	
	/** Active property or \em NULL if none. */
	seProperty *GetProperty() const;
	
	/** Active node or \em NULL if none. */
	sePropertyNode *GetNode() const;
	
	/** Selected mapped type. */
	int GetSelectedMappedType() const;
	
	/** Show node panel. */
	void ShowNodePanel();
	
	/** Update node. */
	void UpdateNode();
	
	/** Update outline. */
	void UpdateOutline();
	
	/** Select active node in outliner. */
	void OutlinerSelectActive();
	
	/** Update mapped. */
	void UpdateMapped();
	
	/** Update mapped type list. */
	void UpdateMappedTypeList();
	
	/** Update mapped target list. */
	void UpdateMappedTargetList();
	/*@}*/
	
	
	
private:
	/** Update outline for tree item. */
	void UpdateOutline(igdeTreeItem *item, sePropertyNode *node, const decString &prefix);
};

#endif
