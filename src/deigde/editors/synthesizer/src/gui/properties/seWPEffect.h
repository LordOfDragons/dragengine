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

#ifndef _SEWPEFFECT_H_
#define _SEWPEFFECT_H_

#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeSwitcherReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/layout/igdeContainerFlow.h>

class seEffect;
class seSource;
class seSynthesizer;
class seWPAPanelEffect;
class seWPAPanelEffectStretch;
class seViewSynthesizer;
class seWPEffectListener;



/**
 * \brief Base source effect properties panel.
 */
class seWPEffect : public igdeContainerFlow{
private:
	seViewSynthesizer &pViewSynthesizer;
	seWPEffectListener *pListener;
	
	seSynthesizer *pSynthesizer;
	
	igdeListBoxReference pListEffect;
	
	igdeSwitcherReference pSwitcher;
	seWPAPanelEffectStretch *pPanelStretch;
	seWPAPanelEffect *pActivePanel;
	
	igdeActionReference pActionEffectCut;
	igdeActionReference pActionEffectCopy;
	igdeActionReference pActionEffectPasteAdd;
	igdeActionReference pActionEffectPasteInsert;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new panel. */
	seWPEffect( seViewSynthesizer &viewSynthesizer );
	
protected:
	/** \brief Cleans up the panel. */
	virtual ~seWPEffect();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief View synthesizer. */
	inline seViewSynthesizer &GetViewSynthesizer() const{ return pViewSynthesizer; }
	
	/** \brief Synthesizer or \em NULL if not set. */
	inline seSynthesizer *GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set synthesizer or \em NULL if not set. */
	void SetSynthesizer( seSynthesizer *synthesizer );
	
	/** \brief Source or \em NULL if not set. */
	seSource *GetSource() const;
	
	/** \brief Effect or \em NULL if not set. */
	seEffect *GetEffect() const;
	
	
	
	/** \brief Update the controller list. */
	void UpdateControllerList();
	
	/** \brief Update the link list. */
	void UpdateLinkList();
	
	/** \brief Update the effect list. */
	void UpdateEffectList();
	
	/** \brief Update the active effect. */
	void UpdateEffect();
	
	/** \brief Select active effect. */
	void SelectActiveEffect();
	
	/** \brief Show panel matching active effect. */
	void ShowActiveEffectPanel();
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionEffectCut() const{ return pActionEffectCut; }
	inline igdeAction *GetActionEffectCopy() const{ return pActionEffectCopy; }
	inline igdeAction *GetActionEffectPasteAdd() const{ return pActionEffectPasteAdd; }
	inline igdeAction *GetActionEffectPasteInsert() const{ return pActionEffectPasteInsert; }
	/*@}*/
};

#endif
