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

#ifndef _SEWPLAYER_H_
#define _SEWPLAYER_H_

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeColorBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeSpinTextFieldReference.h>
#include <deigde/gui/composed/igdeEditSliderTextReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/composed/igdeEditVector2Reference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/event/igdeAction::Ref.h>

class seSky;
class seLayer;
class seBody;
class seWindowProperties;
class seWPLayerListener;



/**
 * \brief Sky layer panel.
 */
class seWPLayer : public igdeContainerScroll{
private:
	seWindowProperties &pWindowProperties;
	seWPLayerListener *pListener;
	
	seSky *pSky;
	
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
	
	igdeListBoxReference pListLayer;
	
	igdeTextFieldReference pEditName;
	
	igdeEditPathReference pEditSkin;
	igdeEditVectorReference pEditOffset;
	igdeEditVectorReference pEditOrientation;
	igdeColorBoxReference pClrLayer;
	igdeTextFieldReference pEditIntensity;
	igdeEditSliderTextReference pSldTransparency;
	igdeCheckBoxReference pChkMulBySkyLight;
	igdeCheckBoxReference pChkMulBySkyColor;
	
	igdeColorBoxReference pClrLight;
	igdeTextFieldReference pEditLightIntensity;
	igdeTextFieldReference pEditAmbientIntensity;
	igdeEditVectorReference pEditLightOrientation;
	
	igdeSpinTextFieldReference pSpinBody;
	igdeButtonReference pBtnBody;
	
	igdeEditPathReference pEditBodySkin;
	igdeEditVectorReference pEditBodyOrienation;
	igdeEditVector2Reference pEditBodySize;
	igdeColorBoxReference pClrBody;
	
	igdeComboBoxReference pCBTarget;
	igdeComboBoxReference pCBLinks;
	igdeListBoxReference pListLinks;
	igdeButtonReference pBtnLinkAdd;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPLayer( seWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~seWPLayer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Monitored sky. */
	inline seSky *GetSky() const{ return pSky; }
	
	/** \brief Set sky to monitor. */
	void SetSky( seSky *sky );
	
	/** \brief Sky path changed. */
	void OnSkyPathChanged();
	
	/** \brief Active layer or \em NULL. */
	seLayer *GetLayer() const;
	
	/** \brief Active body or \em NULL. */
	seBody *GetBody() const;
	
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
	inline igdeAction *GetActionLayerAdd() const{ return pActionLayerAdd; }
	inline igdeAction *GetActionLayerRemove() const{ return pActionLayerRemove; }
	inline igdeAction *GetActionLayerUp() const{ return pActionLayerUp; }
	inline igdeAction *GetActionLayerDown() const{ return pActionLayerDown; }
	
	inline igdeAction *GetActionBodyAdd() const{ return pActionBodyAdd; }
	inline igdeAction *GetActionBodyRemove() const{ return pActionBodyRemove; }
	inline igdeAction *GetActionBodyUp() const{ return pActionBodyUp; }
	inline igdeAction *GetActionBodyDown() const{ return pActionBodyDown; }
	
	inline igdeAction *GetActionLinkAdd() const{ return pActionLinkAdd; }
	inline igdeAction *GetActionLinkRemove() const{ return pActionLinkRemove; }
	/*@}*/
};

#endif
