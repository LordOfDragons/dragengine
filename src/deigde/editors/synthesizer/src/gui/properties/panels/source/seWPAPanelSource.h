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

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/layout/igdeContainerFlow.h>

#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>

class seLink;
class seSynthesizer;
class seSource;
class seWPEffect;
class seWPSource;
class seWindowMain;
class seControllerTarget;



/**
 * \brief Base source option panel.
 */
class seWPAPanelSource : public igdeContainerFlow{
private:
	seWPSource &pWPSource;
	deSynthesizerSourceVisitorIdentify::eSourceTypes pRequiredType;
	
	igdeActionReference pActionLinkAdd;
	igdeActionReference pActionLinkRemove;
	
	igdeTextFieldReference pEditName;
	igdeComboBoxReference pCBMixMode;
	igdeTextFieldReference pEditBlendFactor;
	igdeTextFieldReference pEditMinVolume;
	igdeTextFieldReference pEditMaxVolume;
	igdeTextFieldReference pEditMinPanning;
	igdeTextFieldReference pEditMaxPanning;
	igdeCheckBoxReference pChkEnabled;
	
	igdeComboBoxReference pCBTarget;
	igdeListBoxReference pListLinks;
	igdeComboBoxReference pCBLinks;
	igdeButtonReference pBtnLinkAdd;
	
	seWPEffect *pWPEffect;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPAPanelSource( seWPSource &wpSource, deSynthesizerSourceVisitorIdentify::eSourceTypes requiredType );
	
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
	
	/** \brief Synthesizer or \em NULL if not set. */
	seSynthesizer *GetSynthesizer() const;
	
	/** \brief Source or \em NULL if not set. */
	seSource *GetSource() const;
	
	/** \brief Target or \em NULL if not set. */
	seControllerTarget *GetTarget() const;
	
	/** \brief Selected link in combo box or \em NULL if not set. */
	seLink *GetCBLink() const;
	
	/** \brief Selected link in list box or \em NULL if not set. */
	seLink *GetListLink() const;
	
	
	
	/** \brief Synthesizer changed. */
	virtual void SetSynthesizer( seSynthesizer *synthesizer );
	
	/** \brief Synthesizer path changed. */
	virtual void OnSynthesizerPathChanged();
	
	/** \brief Panel has been activated. */
	virtual void OnActivated();
	
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
	void AddTarget( const char *name, seControllerTarget *target );
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionLinkAdd() const{ return pActionLinkAdd; }
	inline igdeAction *GetActionLinkRemove() const{ return pActionLinkRemove; }
	/*@}*/
};

#endif
