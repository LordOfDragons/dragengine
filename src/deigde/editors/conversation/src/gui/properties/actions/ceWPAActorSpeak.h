/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CEWPAACTORSPEAK_H_
#define _CEWPAACTORSPEAK_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>

#include "ceWPAction.h"

class ceCAActorSpeak;



/**
 * \brief Actor Speak Conversation Action property window.
 */
class ceWPAActorSpeak : public ceWPAction{
private:
	igdeComboBoxReference pCBActorID;
	igdeTextFieldReference pEditTextBoxText;
	igdeButtonReference pBtnTextBoxText;
	igdeTextFieldReference pEditTextBoxTextTranslate;
	igdeTextFieldReference pEditShowTranslation;
	igdeButtonReference pBtnTextBoxTextTranslate;
	igdeTextFieldReference pEditTextBoxTextStyle;
	igdeTextFieldReference pEditMovement;
	igdeEditPathReference pEditPathSound;
	igdeTextFieldReference pEditMinSpeechTime;
	igdeCheckBoxReference pChkUseSpeechAnimation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	ceWPAActorSpeak( ceWPTopic &parentPanel );
	
protected:
	/** \brief Clean up panel. */
	virtual ~ceWPAActorSpeak();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Active action. */
	ceCAActorSpeak *GetAction() const;
	
	/** \brief Update active action. */
	void UpdateAction();
	
	/** \brief Update actor id lists. */
	virtual void UpdateActorIDLists();
	
	/** \brief Conversation path changed. */
	virtual void OnConversationPathChanged();
	/*@}*/
};

#endif
