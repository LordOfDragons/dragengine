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

#ifndef _SEWPTEXTURE_H_
#define _SEWPTEXTURE_H_

#include "../../skin/property/seProperty.h"

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPoint.h>
#include <deigde/gui/composed/igdeEditPoint3.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class seSkin;
class seTexture;
class seWindowProperties;
class seWPTextureListener;



/**
 * Texture panel.
 */
class seWPTexture : public igdeContainerScroll{
private:
	seWindowProperties &pWindowProperties;
	seWPTextureListener::Ref pListener;
	
	seSkin::Ref pSkin;
	bool pRequiresUpdate;
	bool pPreventUpdateMappedTarget;
	
	igdeListBox::Ref pListTexture;
	igdeTextField::Ref pEditTexName;
	
	igdeListBox::Ref pListProperty;
	igdeTextField::Ref pEditPropName;
	igdeTextField::Ref pEditPropRenderable;
	igdeTextField::Ref pEditPropBone;
	
	igdeComboBox::Ref pCBPropertyType;
	igdeSwitcher::Ref pSwitcher;
	
	igdeTextField::Ref pEditPvtValue;
	
	igdeColorBox::Ref pClrPvtColor;
	
	igdeEditPath::Ref pEditPvtImagePath;
	igdeTextField::Ref pLabPvtImageInfo;
	
	igdeEditPath::Ref pEditPvtVideoPath;
	igdeTextField::Ref pLabPvtVideoInfo;
	igdeCheckBox::Ref pChkPvtVideoSharedTime;
	
	igdeSpinTextField::Ref pSpinPvtMappedComponent;
	igdeComboBox::Ref pCBPvtMappedTarget;
	
	igdeColorBox::Ref pConstructedClrColor;
	igdeEditPoint3::Ref pConstructedEditSize;
	igdeCheckBox::Ref pConstructedChkTileX;
	igdeCheckBox::Ref pConstructedChkTileY;
	igdeComboBox::Ref pConstructedCBBitCount;
	
	igdeEditPoint::Ref pPreviewEditTCOffset;
	igdeEditPoint::Ref pPreviewEditTCScaling;
	igdeTextField::Ref pPreviewEditTCRotation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	seWPTexture(seWindowProperties &windowProperties);
	
protected:
	/** Clean up panel. */
	virtual ~seWPTexture();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Properties window. */
	inline seWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** Skin. */
	inline seSkin *GetSkin() const{ return pSkin; }
	
	/** Set skin. */
	void SetSkin(seSkin *skin);
	
	/** Skin path changed. */
	void OnSkinPathChanged();
	
	/** Active texture. */
	seTexture *GetTexture() const;
	
	/** Active property. */
	seProperty *GetProperty() const;
	
	/** Update texture list. */
	void UpdateTextureList();
	
	/** Change selection to the active texture. */
	void SelectActiveTexture();
	
	/** Update texture parameters. */
	void UpdateTexture();
	
	/** Update property list. */
	void UpdatePropertyList();
	
	/** Update preview parameters. */
	void UpdatePreviewParameters();
	
	/** Change selection to the active property. */
	void SelectActiveProperty();
	
	/** Show active property panel. */
	void ShowPropertyPanel();
	
	/** Update property parameters. */
	void UpdateProperty();
	
	/** Selected mapped component index. */
	int GetPropertyMappedComponentIndex() const;
	
	/** Update property parameter mapped component. */
	void UpdatePropertyMappedComponent();
	
	/** Update mapped component target list. */
	void UpdatePropertyMappedTargetList();
	/*@}*/
};

#endif
