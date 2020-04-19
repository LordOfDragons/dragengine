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

#ifndef _SEWPDYNAMICSKIN_H_
#define _SEWPDYNAMICSKIN_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeColorBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeSwitcherReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditSliderTextReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class seSkin;
class seDynamicSkinRenderable;
class seWindowProperties;
class seWPDynamicSkinListener;



/**
 * \brief Dynamic skin panel.
 */
class seWPDynamicSkin : public igdeContainerScroll{
private:
	seWindowProperties &pWindowProperties;
	seWPDynamicSkinListener *pListener;
	
	seSkin *pSkin;
	bool pRequiresUpdate;
	
	igdeListBoxReference pListRenderable;
	igdeTextFieldReference pEditName;
	igdeComboBoxReference pCBRenderableType;
	
	igdeSwitcherReference pSwitcher;
	
	igdeEditSliderTextReference pSldValue;
	igdeTextFieldReference pEditValueLower;
	igdeTextFieldReference pEditValueUpper;
	
	igdeColorBoxReference pClrColor;
	igdeEditSliderTextReference pSldColorRed;
	igdeEditSliderTextReference pSldColorGreen;
	igdeEditSliderTextReference pSldColorBlue;
	igdeEditSliderTextReference pSldColorAlpha;
	
	igdeEditPathReference pEditImagePath;
	
	igdeEditPathReference pEditVideoPath;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPDynamicSkin( seWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~seWPDynamicSkin();
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
	
	/** \brief Active renderable. */
	seDynamicSkinRenderable *GetRenderable() const;
	
	/** \brief Update renderable list. */
	void UpdateRenderableList();
	
	/** \brief Update renderable parameters. */
	void UpdateRenderable();
	
	/** \brief Change selection to the active renderable. */
	void SelectActiveRenderable();
	
	/** \brief Show value panel. */
	void ShowValuePanel();
	/*@}*/
};

#endif
