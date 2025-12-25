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

#ifndef _SEWPAPANELSOURCE_H_
#define _SEWPAPANELSOURCE_H_

#include "../../seWPEffect.h"

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/layout/igdeContainerFlow.h>

#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>

class seLink;
class seSynthesizer;
class seSource;
class seWPSource;
class seWindowMain;
class seControllerTarget;



/**
 * \brief Base source option panel.
 */
class seWPAPanelSource : public igdeContainerFlow{
public:
	typedef deTObjectReference<seWPAPanelSource> Ref;
	
private:
	seWPSource &pWPSource;
	deSynthesizerSourceVisitorIdentify::eSourceTypes pRequiredType;
	
	igdeAction::Ref pActionLinkAdd;
	igdeAction::Ref pActionLinkRemove;
	
	igdeTextField::Ref pEditName;
	igdeComboBox::Ref pCBMixMode;
	igdeTextField::Ref pEditBlendFactor;
	igdeTextField::Ref pEditMinVolume;
	igdeTextField::Ref pEditMaxVolume;
	igdeTextField::Ref pEditMinPanning;
	igdeTextField::Ref pEditMaxPanning;
	igdeCheckBox::Ref pChkEnabled;
	
	igdeComboBox::Ref pCBTarget;
	igdeListBox::Ref pListLinks;
	igdeComboBox::Ref pCBLinks;
	igdeButton::Ref pBtnLinkAdd;
	
	seWPEffect::Ref pWPEffect;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPAPanelSource(seWPSource &wpSource, deSynthesizerSourceVisitorIdentify::eSourceTypes requiredType);
	
protected:
	/** \brief Clean up panel. */
	virtual ~seWPAPanelSource();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parent window. */
	inline seWPSource &GetWPSource() const{ return pWPSource; }
	
	/** \brief Required source type. */
	inline deSynthesizerSourceVisitorIdentify::eSourceTypes GetRequiredType() const{ return pRequiredType; }
	
	/** \brief Main window. */
	seWindowMain &GetWindowMain() const;
	
	/** \brief Synthesizer or \em nullptr if not set. */
	seSynthesizer *GetSynthesizer() const;
	
	/** \brief Source or \em nullptr if not set. */
	seSource *GetSource() const;
	
	/** \brief Target or \em nullptr if not set. */
	seControllerTarget *GetTarget() const;
	
	/** \brief Selected link in combo box or \em nullptr if not set. */
	seLink *GetCBLink() const;
	
	/** \brief Selected link in list box or \em nullptr if not set. */
	seLink *GetListLink() const;
	
	
	
	/** \brief Synthesizer changed. */
	virtual void SetSynthesizer(seSynthesizer *synthesizer);
	
	/** \brief Synthesizer path changed. */
	virtual void OnSynthesizerPathChanged();
	
	/** \brief Panel has been activated. */
	virtual void OnActivated();
	
	/** \brief Panel has been deactivated. */
	virtual void OnDeactivated();
	
	/** \brief Update synthesizer. */
	virtual void UpdateSynthesizer();
	
	/** \brief Update controller list. */
	virtual void UpdateControllerList();
	
	/** \brief Update link list. */
	virtual void UpdateLinkList();
	
	/** \brief Update source. */
	virtual void UpdateSource();
	
	/** \brief Update target list. */
	virtual void UpdateTargetList();
	
	/** \brief Update target. */
	void UpdateTarget();
	
	/** \brief Remove all targets. */
	void RemoveAllTargets();
	
	/** \brief Add target. */
	void AddTarget(const char *name, seControllerTarget *target);
	
	
	
	/** \brief Actions. */
	inline const igdeAction::Ref &GetActionLinkAdd() const{ return pActionLinkAdd; }
	inline const igdeAction::Ref &GetActionLinkRemove() const{ return pActionLinkRemove; }
	/*@}*/
};

#endif
