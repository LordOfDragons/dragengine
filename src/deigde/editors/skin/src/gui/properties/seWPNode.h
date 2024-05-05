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

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeColorBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeSwitcherReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeTreeListReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditPointReference.h>
#include <deigde/gui/composed/igdeEditPoint3Reference.h>
#include <deigde/gui/composed/igdeEditSliderTextReference.h>
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
	
	igdeTreeListReference pTreeOutline;
	igdeTextFieldReference pLabSelection;
	
	igdeEditPoint3Reference pEditPosition;
	igdeEditPoint3Reference pEditSize;
	igdeTextFieldReference pEditRotation;
	igdeTextFieldReference pEditShear;
	igdeTextFieldReference pEditBrightness;
	igdeTextFieldReference pEditContrast;
	igdeTextFieldReference pEditGamma;
	igdeColorBoxReference pClrColorize;
	igdeEditSliderTextReference pSldTransparency;
	igdeTextFieldReference pLabMask;
	igdeComboBoxReference pCBCombineMode;
	
	igdeSwitcherReference pSwitcher;
	
	igdeEditPathReference pImageEditImage;
	igdeEditPointReference pImageEditRepeat;
	igdeTextFieldReference pImageLabImageInfo;
	
	igdeComboBoxReference pShapeCBType;
	igdeColorBoxReference pShapeClrFill;
	igdeColorBoxReference pShapeClrLine;
	igdeTextFieldReference pShapeEditThickness;
	
	igdeEditPathReference pTextEditFont;
	igdeTextFieldReference pTextLabFontInfo;
	igdeTextFieldReference pTextEditFontSize;
	igdeTextFieldReference pTextEditText;
	igdeColorBoxReference pTextClrColor;
	
	igdeComboBoxReference pCBMappedType;
	igdeComboBoxReference pCBMappedTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	seWPNode( seWindowProperties &windowProperties );
	
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
	void SetSkin( seSkin *skin );
	
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
	void UpdateOutline( igdeTreeItem *item, sePropertyNode *node, const decString &prefix );
};

#endif
