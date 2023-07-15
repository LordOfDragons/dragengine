/* 
 * Drag[en]gine IGDE Skin Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
