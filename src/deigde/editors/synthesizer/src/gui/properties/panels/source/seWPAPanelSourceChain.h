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

#ifndef _SEWPAPANELSOURCECHAIN_H_
#define _SEWPAPANELSOURCECHAIN_H_

#include "seWPAPanelSource.h"

#include <deigde/gui/composed/igdeEditPathReference.h>


/**
 * \brief Chain source panel.
 */
class seWPAPanelSourceChain : public seWPAPanelSource{
private:
	igdeListBoxReference pListSounds;
	
	igdeEditPathReference pEditPathSound;
	igdeTextFieldReference pEditMinSpeed;
	igdeTextFieldReference pEditMaxSpeed;
	igdeTextFieldReference pLabSoundInfo;
	
	igdeActionReference pActionSoundUp;
	igdeActionReference pActionSoundDown;
	igdeActionReference pActionSoundAdd;
	igdeActionReference pActionSoundRemove;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPAPanelSourceChain( seWPSource &wpSource );
	
protected:
	/** \brief Clean up panel. */
	virtual ~seWPAPanelSourceChain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Update synthesizer. */
	virtual void UpdateSynthesizer();
	
	/** \brief Update source. */
	virtual void UpdateSource();
	
	/** \brief Update target list. */
	virtual void UpdateTargetList();
	
	/** \brief Update sound list. */
	void UpdateSoundList();
	
	/** \brief Update sound. */
	void UpdateSound();
	
	/** \brief Update sound information. */
	void UpdateSoundInfo();
	
	/** \brief Selected sound in list. */
	int GetSelectedSoundInList() const;
	
	/** \brief Select sound in list. */
	void SelectSoundInList( int index );
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionSoundUp() const{ return pActionSoundUp; }
	inline igdeAction *GetActionSoundDown() const{ return pActionSoundDown; }
	inline igdeAction *GetActionSoundAdd() const{ return pActionSoundAdd; }
	inline igdeAction *GetActionSoundRemove() const{ return pActionSoundRemove; }
	/*@}*/
};

#endif
