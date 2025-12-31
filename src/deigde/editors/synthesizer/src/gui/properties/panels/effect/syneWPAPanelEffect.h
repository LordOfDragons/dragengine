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

#ifndef _SYNEWPAPANELEFFECT_H_
#define _SYNEWPAPANELEFFECT_H_

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/layout/igdeContainerFlow.h>

#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectVisitorIdentify.h>


class syneLink;
class syneSynthesizer;
class syneEffect;
class syneWPEffect;
class syneWindowMain;
class syneControllerTarget;



/**
 * \brief Base effect option panel.
 */
class syneWPAPanelEffect : public igdeContainerFlow{
private:
	syneWPEffect &pWPEffect;
	deSynthesizerEffectVisitorIdentify::eEffectTypes pRequiredType;
	
	igdeAction::Ref pActionLinkAdd;
	igdeAction::Ref pActionLinkRemove;
	
	igdeTextField::Ref pEditStrength;
	igdeCheckBox::Ref pChkEnabled;
	
	igdeComboBox::Ref pCBTarget;
	igdeListBox::Ref pListLinks;
	igdeComboBox::Ref pCBLinks;
	igdeButton::Ref pBtnLinkAdd;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	syneWPAPanelEffect(syneWPEffect &wpEffect, deSynthesizerEffectVisitorIdentify::eEffectTypes requiredType);
	
protected:
	/** \brief Clean up panel. */
	virtual ~syneWPAPanelEffect();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parent window. */
	inline syneWPEffect &GetWPEffect() const{ return pWPEffect; }
	
	/** \brief Required effect type. */
	inline deSynthesizerEffectVisitorIdentify::eEffectTypes GetRequiredType() const{ return pRequiredType; }
	
	/** \brief Main window. */
	syneWindowMain &GetWindowMain() const;
	
	/** \brief Synthesizer or \em nullptr if not set. */
	syneSynthesizer *GetSynthesizer() const;
	
	/** \brief Effect or \em nullptr if not set. */
	syneEffect *GetEffect() const;
	
	/** \brief Target or \em nullptr if not set. */
	syneControllerTarget *GetTarget() const;
	
	/** \brief Selected link in combo box or \em nullptr if not set. */
	syneLink *GetCBLink() const;
	
	/** \brief Selected link in list box or \em nullptr if not set. */
	syneLink *GetListLink() const;
	
	
	
	/** \brief Panel has been activated. */
	virtual void OnActivated();
	
	/** \brief Update controller list. */
	virtual void UpdateControllerList();
	
	/** \brief Update link list. */
	virtual void UpdateLinkList();
	
	/** \brief Update effect. */
	virtual void UpdateEffect();
	
	/** \brief Update target list. */
	virtual void UpdateTargetList();
	
	/** \brief Update target. */
	void UpdateTarget();
	
	/** \brief Remove all targets. */
	void RemoveAllTargets();
	
	/** \brief Add target. */
	void AddTarget(const char *name, syneControllerTarget *target);
	
	
	
	/** \brief Actions. */
	inline const igdeAction::Ref &GetActionLinkAdd() const{ return pActionLinkAdd; }
	inline const igdeAction::Ref &GetActionLinkRemove() const{ return pActionLinkRemove; }
	/*@}*/
};

#endif
