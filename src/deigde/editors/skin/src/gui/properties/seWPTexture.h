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

#ifndef _SEWPTEXTURE_H_
#define _SEWPTEXTURE_H_

#include "../../skin/property/seProperty.h"

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeColorBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeSwitcherReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeSpinTextFieldReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditPointReference.h>
#include <deigde/gui/composed/igdeEditPoint3Reference.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class seSkin;
class seTexture;
class seWindowProperties;
class seWPTextureListener;



/**
 * \brief Texture panel.
 */
class seWPTexture : public igdeContainerScroll{
private:
	seWindowProperties &pWindowProperties;
	seWPTextureListener *pListener;
	
	seSkin *pSkin;
	bool pRequiresUpdate;
	
	igdeListBoxReference pListTexture;
	igdeTextFieldReference pEditTexName;
	
	igdeListBoxReference pListProperty;
	igdeTextFieldReference pEditPropName;
	igdeTextFieldReference pEditPropRenderable;
	
	igdeComboBoxReference pCBPropertyType;
	igdeSwitcherReference pSwitcher;
	
	igdeTextFieldReference pEditPvtValue;
	
	igdeColorBoxReference pClrPvtColor;
	
	igdeEditPathReference pEditPvtImagePath;
	igdeTextFieldReference pLabPvtImageInfo;
	
	igdeEditPathReference pEditPvtVideoPath;
	igdeTextFieldReference pLabPvtVideoInfo;
	igdeCheckBoxReference pChkPvtVideoSharedTime;
	
	igdeSpinTextFieldReference pSpinPvtMappedComponent;
	igdeViewCurveBezierReference pEditPvtMappedCurve;
	igdeComboBoxReference pCBPvtMappedInputType;
	igdeTextFieldReference pEditPvtMappedInputLower;
	igdeTextFieldReference pEditPvtMappedInputUpper;
	igdeCheckBoxReference pChkPvtMappedInputClamped;
	igdeTextFieldReference pEditPvtMappedBone;
	
	igdeColorBoxReference pConstructedClrColor;
	igdeEditPoint3Reference pConstructedEditSize;
	igdeCheckBoxReference pConstructedChkTileX;
	igdeCheckBoxReference pConstructedChkTileY;
	
	igdeEditPointReference pPreviewEditTCOffset;
	igdeEditPointReference pPreviewEditTCScaling;
	igdeTextFieldReference pPreviewEditTCRotation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPTexture( seWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~seWPTexture();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline seWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Skin. */
	inline seSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin. */
	void SetSkin( seSkin *skin );
	
	/** \brief Active texture. */
	seTexture *GetTexture() const;
	
	/** \brief Active property. */
	seProperty *GetProperty() const;
	
	/** \brief Update texture list. */
	void UpdateTextureList();
	
	/** \brief Change selection to the active texture. */
	void SelectActiveTexture();
	
	/** \brief Update texture parameters. */
	void UpdateTexture();
	
	/** \brief Update property list. */
	void UpdatePropertyList();
	
	/** \brief Update preview parameters. */
	void UpdatePreviewParameters();
	
	/** \brief Change selection to the active property. */
	void SelectActiveProperty();
	
	/** \brief Show active property panel. */
	void ShowPropertyPanel();
	
	/** \brief Update property parameters. */
	void UpdateProperty();
	
	/** \brief Selected mapped component index. */
	int GetPropertyMappedComponentIndex() const;
	
	/** \brief Update property parameter mapped component. */
	void UpdatePropertyMappedComponent();
	/*@}*/
};

#endif
