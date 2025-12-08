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

#ifndef _SEWPSYNTHESIZER_H_
#define _SEWPSYNTHESIZER_H_

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

#include "../../synthesizer/seSynthesizer.h"
class seViewSynthesizer;
#include "seWPSynthesizerListener.h"



/**
 * \brief Synthesizer properties panel.
 */
class seWPSynthesizer : public igdeContainerScroll{
private:
	seViewSynthesizer &pViewSynthesizer;
	seWPSynthesizerListener::Ref pListener;
	
	seSynthesizer::Ref pSynthesizer;
	
	igdeAction::Ref pActionPlay;
	igdeAction::Ref pActionPause;
	igdeAction::Ref pActionStop;
	
	igdeComboBox::Ref pCBChannelCount;
	igdeComboBox::Ref pCBSampleRate;
	igdeComboBox::Ref pCBBytesPerSample;
	igdeTextField::Ref pEditSampleCount;
	igdeTextField::Ref pEditPlayTime;
	igdeCheckBox::Ref pChkLooping;
	igdeButton::Ref pBtnPlay;
	igdeButton::Ref pBtnPause;
	igdeButton::Ref pBtnStop;
	
	bool pPreventUpdate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create properties panel. */
	seWPSynthesizer(seViewSynthesizer &viewSynthesizer);
	
protected:
	/** \brief Clean up properties panel. */
	virtual ~seWPSynthesizer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief View. */
	inline seViewSynthesizer &GetViewSynthesizer() const{ return pViewSynthesizer; }
	
	/** \brief Synthesizer or \em NULL if not set. */
	inline const seSynthesizer::Ref &GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set synthesizer or \em NULL if not set. */
	void SetSynthesizer(seSynthesizer *synthesizer);
	
	
	
	/** \brief Update synthesizer. */
	void UpdateSynthesizer();
	
	/** \brief Update playback. */
	void UpdatePlayback();
	
	
	
	/** \brief Actions. */
	inline const igdeAction::Ref &GetActionPlay() const{ return pActionPlay; }
	inline const igdeAction::Ref &GetActionPause() const{ return pActionPause; }
	inline const igdeAction::Ref &GetActionStop() const{ return pActionStop; }
	/*@}*/
};

#endif
