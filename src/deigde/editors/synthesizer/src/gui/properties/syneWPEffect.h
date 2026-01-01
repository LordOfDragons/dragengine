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

#ifndef _SYNEWPEFFECT_H_
#define _SYNEWPEFFECT_H_

#include "syneWPEffectListener.h"
#include "panels/effect/syneWPAPanelEffectStretch.h"
#include "../../synthesizer/syneSynthesizer.h"

#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/layout/igdeContainerFlow.h>

class syneEffect;
class syneSource;
class syneWPAPanelEffect;
class syneViewSynthesizer;



/**
 * \brief Base source effect properties panel.
 */
class syneWPEffect : public igdeContainerFlow{
public:
	typedef deTObjectReference<syneWPEffect> Ref;
	
private:
	syneViewSynthesizer &pViewSynthesizer;
	syneWPEffectListener::Ref pListener;
	
	syneSynthesizer::Ref pSynthesizer;
	
	igdeListBox::Ref pListEffect;
	
	igdeSwitcher::Ref pSwitcher;
	syneWPAPanelEffectStretch::Ref pPanelStretch;
	syneWPAPanelEffect *pActivePanel;
	
	igdeAction::Ref pActionEffectCut;
	igdeAction::Ref pActionEffectCopy;
	igdeAction::Ref pActionEffectPasteAdd;
	igdeAction::Ref pActionEffectPasteInsert;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new panel. */
	syneWPEffect(syneViewSynthesizer &viewSynthesizer);
	
protected:
	/** \brief Cleans up the panel. */
	virtual ~syneWPEffect();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief View synthesizer. */
	inline syneViewSynthesizer &GetViewSynthesizer() const{ return pViewSynthesizer; }
	
	/** \brief Synthesizer or \em nullptr if not set. */
	inline const syneSynthesizer::Ref &GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set synthesizer or \em nullptr if not set. */
	void SetSynthesizer(syneSynthesizer *synthesizer);
	
	/** \brief Source or \em nullptr if not set. */
	syneSource *GetSource() const;
	
	/** \brief Effect or \em nullptr if not set. */
	syneEffect *GetEffect() const;
	
	
	
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
	inline const igdeAction::Ref &GetActionEffectCut() const{ return pActionEffectCut; }
	inline const igdeAction::Ref &GetActionEffectCopy() const{ return pActionEffectCopy; }
	inline const igdeAction::Ref &GetActionEffectPasteAdd() const{ return pActionEffectPasteAdd; }
	inline const igdeAction::Ref &GetActionEffectPasteInsert() const{ return pActionEffectPasteInsert; }
	/*@}*/
};

#endif
