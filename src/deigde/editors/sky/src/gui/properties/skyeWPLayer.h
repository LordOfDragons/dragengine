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

#ifndef _SKYEWPLAYER_H_
#define _SKYEWPLAYER_H_

#include "skyeWPLayerListener.h"
#include "../../sky/skyeSky.h"

#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVector2.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/event/igdeAction.h>

class skyeLayer;
class skyeBody;
class skyeWindowProperties;



/**
 * \brief Sky layer panel.
 */
class skyeWPLayer : public igdeContainerScroll{
public:
	typedef deTObjectReference<skyeWPLayer> Ref;
	
private:
	skyeWindowProperties &pWindowProperties;
	skyeWPLayerListener::Ref pListener;
	
	skyeSky::Ref pSky;
	
	igdeAction::Ref pActionLayerAdd;
	igdeAction::Ref pActionLayerRemove;
	igdeAction::Ref pActionLayerUp;
	igdeAction::Ref pActionLayerDown;
	
	igdeAction::Ref pActionBodyAdd;
	igdeAction::Ref pActionBodyRemove;
	igdeAction::Ref pActionBodyUp;
	igdeAction::Ref pActionBodyDown;
	
	igdeAction::Ref pActionLinkAdd;
	igdeAction::Ref pActionLinkRemove;
	
	igdeListBox::Ref pListLayer;
	
	igdeTextField::Ref pEditName;
	
	igdeEditPath::Ref pEditSkin;
	igdeEditVector::Ref pEditOffset;
	igdeEditVector::Ref pEditOrientation;
	igdeColorBox::Ref pClrLayer;
	igdeTextField::Ref pEditIntensity;
	igdeEditSliderText::Ref pSldTransparency;
	igdeCheckBox::Ref pChkMulBySkyLight;
	igdeCheckBox::Ref pChkMulBySkyColor;
	
	igdeColorBox::Ref pClrLight;
	igdeTextField::Ref pEditLightIntensity;
	igdeTextField::Ref pEditAmbientIntensity;
	igdeEditVector::Ref pEditLightOrientation;
	
	igdeSpinTextField::Ref pSpinBody;
	igdeButton::Ref pBtnBody;
	
	igdeEditPath::Ref pEditBodySkin;
	igdeEditVector::Ref pEditBodyOrienation;
	igdeEditVector2::Ref pEditBodySize;
	igdeColorBox::Ref pClrBody;
	
	igdeComboBox::Ref pCBTarget;
	igdeComboBox::Ref pCBLinks;
	igdeListBox::Ref pListLinks;
	igdeButton::Ref pBtnLinkAdd;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	skyeWPLayer(skyeWindowProperties &windowProperties);
	
protected:
	/** \brief Clean up panel. */
	virtual ~skyeWPLayer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Monitored sky. */
	inline const skyeSky::Ref &GetSky() const{ return pSky; }
	
	/** \brief Set sky to monitor. */
	void SetSky(skyeSky *sky);
	
	/** \brief Sky path changed. */
	void OnSkyPathChanged();
	
	/** \brief Active layer or \em nullptr. */
	skyeLayer *GetLayer() const;
	
	/** \brief Active body or \em nullptr. */
	skyeBody *GetBody() const;
	
	/** \brief Update layer list. */
	void UpdateLayerList();
	
	/** \brief Select active layer. */
	void SelectActiveLayer();
	
	/** \brief Update layer. */
	void UpdateLayer();
	
	/** \brief Updates body list. */
	void UpdateBodyList();
	
	/** \brief Select active body. */
	void SelectActiveBody();
	
	/** \brief Update body. */
	void UpdateBody();
	
	/** \brief Select active target. */
	void SelectActiveTarget();
	
	/** \brief Update target. */
	void UpdateTarget();
	
	/** \brief Update link list. */
	void UpdateLinkList();
	
	
	
	/** \brief Actions. */
	inline const igdeAction::Ref &GetActionLayerAdd() const{ return pActionLayerAdd; }
	inline const igdeAction::Ref &GetActionLayerRemove() const{ return pActionLayerRemove; }
	inline const igdeAction::Ref &GetActionLayerUp() const{ return pActionLayerUp; }
	inline const igdeAction::Ref &GetActionLayerDown() const{ return pActionLayerDown; }
	
	inline const igdeAction::Ref &GetActionBodyAdd() const{ return pActionBodyAdd; }
	inline const igdeAction::Ref &GetActionBodyRemove() const{ return pActionBodyRemove; }
	inline const igdeAction::Ref &GetActionBodyUp() const{ return pActionBodyUp; }
	inline const igdeAction::Ref &GetActionBodyDown() const{ return pActionBodyDown; }
	
	inline const igdeAction::Ref &GetActionLinkAdd() const{ return pActionLinkAdd; }
	inline const igdeAction::Ref &GetActionLinkRemove() const{ return pActionLinkRemove; }
	/*@}*/
};

#endif
