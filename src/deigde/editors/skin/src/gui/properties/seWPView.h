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

#ifndef _SEWPVIEW_H_
#define _SEWPVIEW_H_

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/properties/igdeWPCamera.h>
#include <deigde/gui/properties/igdeWPSky.h>
#include <deigde/gui/properties/igdeWPWObject.h>

class seSkin;
class seWindowProperties;
class seWPViewListener;



/**
 * \brief View Panel.
 */
class seWPView : public igdeContainerScroll{
private:
	seWindowProperties &pWindowProperties;
	seWPViewListener::Ref pListener;
	bool pPreventUpdate;
	
	seSkin::Ref pSkin;
	
	igdeComboBox::Ref pCBPreviewMode;
	
	igdeEditPath::Ref pEditModelPath;
	igdeEditPath::Ref pEditRigPath;
	igdeEditPath::Ref pEditAnimPath;
	igdeComboBoxFilter::Ref pCBAnimMoves;
	igdeCheckBox::Ref pChkPlayback;
	
	igdeButton::Ref pBtnRewindTextures;
	
	igdeWPCamera::Ref pWPCamera;
	igdeWPSky::Ref pWPSky;
	igdeWPWObject::Ref pWPEnvObject;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPView(seWindowProperties &windowProperties);
	
protected:
	/** \brief Cleanup panel. */
	virtual ~seWPView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline seWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Skin. */
	inline const seSkin::Ref &GetSkin() const{ return pSkin; }
	
	/** \brief Set skin. */
	void SetSkin(seSkin *sky);
	
	/** \brief Update view. */
	void UpdateView();
	
	/** \brief Update camera. */
	void UpdateCamera();
	
	/** \brief Update sky. */
	void UpdateSky();
	
	/** \brief Update environment object. */
	void UpdateEnvObject();
	
	/** \brief Update animation moves list. */
	void UpdateMoveList();
	/*@}*/
};

#endif
