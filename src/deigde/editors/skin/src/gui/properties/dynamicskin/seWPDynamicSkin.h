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

#ifndef _SEWPDYNAMICSKIN_H_
#define _SEWPDYNAMICSKIN_H_

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include "seSkin.h"
#include "seWPDynamicSkinListener.h"

class seDynamicSkinRenderable;
class seWindowProperties;



/**
 * \brief Dynamic skin panel.
 */
class seWPDynamicSkin : public igdeContainerScroll{
private:
	seWindowProperties &pWindowProperties;
	seWPDynamicSkinListener::Ref pListener;
	
	seSkin::Ref pSkin;
	bool pRequiresUpdate;
	
	igdeListBox::Ref pListRenderable;
	igdeTextField::Ref pEditName;
	igdeComboBox::Ref pCBRenderableType;
	
	igdeSwitcher::Ref pSwitcher;
	
	igdeEditSliderText::Ref pSldValue;
	igdeTextField::Ref pEditValueLower;
	igdeTextField::Ref pEditValueUpper;
	
	igdeColorBox::Ref pClrColor;
	igdeEditSliderText::Ref pSldColorRed;
	igdeEditSliderText::Ref pSldColorGreen;
	igdeEditSliderText::Ref pSldColorBlue;
	igdeEditSliderText::Ref pSldColorAlpha;
	
	igdeEditPath::Ref pEditImagePath;
	
	igdeEditPath::Ref pEditVideoPath;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPDynamicSkin(seWindowProperties &windowProperties);
	
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
	inline const seSkin::Ref &GetSkin() const{ return pSkin; }
	
	/** \brief Set skin. */
	void SetSkin(seSkin *skin);
	
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
