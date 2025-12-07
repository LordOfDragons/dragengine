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

#ifndef _SEWPEFFECT_H_
#define _SEWPEFFECT_H_

#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/event/igdeAction.h>
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
	
	igdeListBox::Ref pListEffect;
	
	igdeSwitcher::Ref pSwitcher;
	seWPAPanelEffectStretch *pPanelStretch;
	seWPAPanelEffect *pActivePanel;
	
	igdeAction::Ref pActionEffectCut;
	igdeAction::Ref pActionEffectCopy;
	igdeAction::Ref pActionEffectPasteAdd;
	igdeAction::Ref pActionEffectPasteInsert;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new panel. */
	seWPEffect(seViewSynthesizer &viewSynthesizer);
	
protected:
	/** \brief Cleans up the panel. */
	virtual ~seWPEffect();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief View synthesizer. */
	inline seViewSynthesizer &GetViewSynthesizer() const{return pViewSynthesizer;}
	
	/** \brief Synthesizer or \em NULL if not set. */
	inline seSynthesizer *GetSynthesizer() const{return pSynthesizer;}
	
	/** \brief Set synthesizer or \em NULL if not set. */
	void SetSynthesizer(seSynthesizer *synthesizer);
	
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
	inline igdeAction *GetActionEffectCut() const{return pActionEffectCut;}
	inline igdeAction *GetActionEffectCopy() const{return pActionEffectCopy;}
	inline igdeAction *GetActionEffectPasteAdd() const{return pActionEffectPasteAdd;}
	inline igdeAction *GetActionEffectPasteInsert() const{return pActionEffectPasteInsert;}
	/*@}*/
};

#endif
