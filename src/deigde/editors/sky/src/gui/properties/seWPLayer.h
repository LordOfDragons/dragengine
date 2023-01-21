/* 
 * Drag[en]gine IGDE Sky Editor
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
#include <deigde/gui/event/igdeActionReference.h>

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
	
	igdeActionReference pActionLayerAdd;
	igdeActionReference pActionLayerRemove;
	igdeActionReference pActionLayerUp;
	igdeActionReference pActionLayerDown;
	
	igdeActionReference pActionBodyAdd;
	igdeActionReference pActionBodyRemove;
	igdeActionReference pActionBodyUp;
	igdeActionReference pActionBodyDown;
	
	igdeActionReference pActionLinkAdd;
	igdeActionReference pActionLinkRemove;
	
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
