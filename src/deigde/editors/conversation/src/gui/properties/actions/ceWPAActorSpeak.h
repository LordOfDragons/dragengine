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
