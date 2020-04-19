/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#ifndef _SEWPLINK_H_
#define _SEWPLINK_H_

#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeSpinTextFieldReference.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class seSynthesizer;
class seLink;
class seViewSynthesizer;
class seWPLinkListener;



/**
 * \brief Link properties panel.
 */
class seWPLink : public igdeContainerScroll{
private:
	seViewSynthesizer &pViewSynthesizer;
	seWPLinkListener *pListener;
	
	seSynthesizer *pSynthesizer;
	
	igdeActionReference pActionLinkAdd;
	igdeActionReference pActionLinkRemove;
	
	igdeListBoxReference pListLink;
	
	igdeTextFieldReference pEditName;
	igdeComboBoxReference pCBController;
	igdeSpinTextFieldReference pSpinRepeat;
	igdeViewCurveBezierReference pEditCurve;
	
	bool pPreventUpdate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create properties panel. */
	seWPLink( seViewSynthesizer &viewSynthesizer );
	
protected:
	/** \brief Clean up properties panel. */
	virtual ~seWPLink();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief View. */
	inline seViewSynthesizer &GetViewSynthesizer() const{ return pViewSynthesizer; }
	
	/** \brief Synthesizer or \em NULL if not set. */
	inline seSynthesizer *GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set synthesizer or \em NULL if not set. */
	void SetSynthesizer( seSynthesizer *synthesizer );
	
	/** \brief Link or \em NULL if not set. */
	seLink *GetLink() const;
	
	
	
	/** \brief Update link list. */
	void UpdateLinkList();
	
	/** \brief Select active link. */
	void SelectActiveLink();
	
	/** \brief Update link. */
	void UpdateLink();
	
	/** \brief Update controller list. */
	void UpdateControllerList();
	
	/** \brief Prevent update for listener use only. */
	inline bool GetPreventUpdate() const{ return pPreventUpdate; }
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionLinkAdd() const{ return pActionLinkAdd; }
	inline igdeAction *GetActionLinkRemove() const{ return pActionLinkRemove; }
	/*@}*/
};

#endif
