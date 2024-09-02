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

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class seSynthesizer;
class seViewSynthesizer;
class seWPSynthesizerListener;



/**
 * \brief Synthesizer properties panel.
 */
class seWPSynthesizer : public igdeContainerScroll{
private:
	seViewSynthesizer &pViewSynthesizer;
	seWPSynthesizerListener *pListener;
	
	seSynthesizer *pSynthesizer;
	
	igdeActionReference pActionPlay;
	igdeActionReference pActionPause;
	igdeActionReference pActionStop;
	
	igdeComboBoxReference pCBChannelCount;
	igdeComboBoxReference pCBSampleRate;
	igdeComboBoxReference pCBBytesPerSample;
	igdeTextFieldReference pEditSampleCount;
	igdeTextFieldReference pEditPlayTime;
	igdeCheckBoxReference pChkLooping;
	igdeButtonReference pBtnPlay;
	igdeButtonReference pBtnPause;
	igdeButtonReference pBtnStop;
	
	bool pPreventUpdate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create properties panel. */
	seWPSynthesizer( seViewSynthesizer &viewSynthesizer );
	
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
	inline seSynthesizer *GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set synthesizer or \em NULL if not set. */
	void SetSynthesizer( seSynthesizer *synthesizer );
	
	
	
	/** \brief Update synthesizer. */
	void UpdateSynthesizer();
	
	/** \brief Update playback. */
	void UpdatePlayback();
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionPlay() const{ return pActionPlay; }
	inline igdeAction *GetActionPause() const{ return pActionPause; }
	inline igdeAction *GetActionStop() const{ return pActionStop; }
	/*@}*/
};

#endif
