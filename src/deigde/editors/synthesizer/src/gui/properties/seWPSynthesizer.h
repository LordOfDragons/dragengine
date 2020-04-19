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
