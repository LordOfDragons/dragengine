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

#ifndef _SEWPAPANELSOURCESOUND_H_
#define _SEWPAPANELSOURCESOUND_H_

#include <deigde/gui/composed/igdeEditPathReference.h>

#include "seWPAPanelSource.h"



/**
 * \brief Sound source panel.
 */
class seWPAPanelSourceSound : public seWPAPanelSource{
private:
	igdeEditPathReference pEditPathSound;
	igdeTextFieldReference pEditMinSpeed;
	igdeTextFieldReference pEditMaxSpeed;
	igdeCheckBoxReference pChkLooping;
	igdeTextFieldReference pLabSoundInfo;
	igdeTextFieldReference pLabSoundMismatch;
	igdeTextFieldReference pHiddenCopyText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPAPanelSourceSound( seWPSource &wpSource );
	
protected:
	/** \brief Clean up panel. */
	virtual ~seWPAPanelSourceSound();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Update synthesizer. */
	virtual void UpdateSynthesizer();
	
	/** \brief Synthesizer path changed. */
	virtual void OnSynthesizerPathChanged();
	
	/** \brief Update source. */
	virtual void UpdateSource();
	
	/** \brief Update target list. */
	virtual void UpdateTargetList();
	
	/** \brief Update sound information. */
	void UpdateSoundInfo();
	/*@}*/
};

#endif
