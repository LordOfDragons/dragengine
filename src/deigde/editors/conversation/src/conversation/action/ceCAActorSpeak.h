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

#ifndef _CECAACTORSPEAK_H_
#define _CECAACTORSPEAK_H_

#include "ceConversationAction.h"
#include "../strip/ceStripList.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class deEngine;
class deSound;



/**
 * \brief Actor Speak Conversation Action.
 * Make an actor speak. Starts playing a sound file while doing lip-sync using a speech
 * animation file.
 */
class ceCAActorSpeak : public ceConversationAction{
private:
	deEngine *pEngine;
	deSound *pEngSound;
	bool pLoaded;
	
	decString pActor;
	decUnicodeString pTextBoxText;
	decString pTextBoxTextStyle;
	decString pPathSound;
	ceStripList pWords;
	ceStripList pFacePoses;
	ceStripList pGestures;
	ceStripList pBodyLookAt;
	ceStripList pHeadLookAt;
	ceStripList pEyesLookAt;
	decString pMovement;
	float pMinSpeechTime;
	bool pUseSpeechAnimation;
	bool pHold;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new actor speak conversation action. */
	ceCAActorSpeak( deEngine *engine );
	/** Creates a new actor speak conversation action. */
	ceCAActorSpeak( const ceCAActorSpeak &action );
	/** Cleans up the actor speak conversation action. */
	virtual ~ceCAActorSpeak();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	/** Retrieves the sound file loading it if not loaded already. */
	deSound *GetEngineSound();
	
	/** Retrieves the actor id. */
	inline const decString &GetActor() const{ return pActor; }
	/** Sets the actor id. */
	void SetActor( const char *id );
	/** Retrieves the text box text. */
	inline const decUnicodeString &GetTextBoxText() const{ return pTextBoxText; }
	/** Sets the text box text. */
	void SetTextBoxText( const decUnicodeString &text );
	/** \brief Retrieves the text box text style. */
	inline const decString &GetTextBoxTextStyle() const{ return pTextBoxTextStyle; }
	/** \brief Sets the text box text style. */
	void SetTextBoxTextStyle( const char *style );
	/** Retrieves the path to the sound file or empty to use none. */
	inline const decString &GetPathSound() const{ return pPathSound; }
	/** Sets the path to the sound file or empty to use none. */
	void SetPathSound( const char *path );
	/** Retrieves the words list for lip-sync. */
	inline ceStripList &GetWordList(){ return pWords; }
	inline const ceStripList &GetWordList() const{ return pWords; }
	/** Retrieves the face pose list. */
	inline ceStripList &GetFacePoseList(){ return pFacePoses; }
	inline const ceStripList &GetFacePoseList() const{ return pFacePoses; }
	/** Retrieves the gesture list. */
	inline ceStripList &GetGestureList(){ return pGestures; }
	inline const ceStripList &GetGestureList() const{ return pGestures; }
	/** Retrieves the body look-at list. */
	inline ceStripList &GetBodyLookAtList(){ return pBodyLookAt; }
	inline const ceStripList &GetBodyLookAtList() const{ return pBodyLookAt; }
	/** Retrieves the head look-at list. */
	inline ceStripList &GetHeadLookAtList(){ return pHeadLookAt; }
	inline const ceStripList &GetHeadLookAtList() const{ return pHeadLookAt; }
	/** Retrieves the eyes look-at list. */
	inline ceStripList &GetEyesLookAtList(){ return pEyesLookAt; }
	inline const ceStripList &GetEyesLookAtList() const{ return pEyesLookAt; }
	/** Retrieves the movement to use or empty to not change. */
	inline const decString &GetMovement() const{ return pMovement; }
	/** Sets the movement to use or empty to not change. */
	void SetMovement( const char *movement );
	/** \brief Retrieves the minimum time the actor is speaking. */
	inline float GetMinSpeechTime() const{ return pMinSpeechTime; }
	/** \brief Sets the minimum time the actor is speaking. */
	void SetMinSpeechTime( float minSpeechTime );
	
	/** \brief Determines if speech animation is used for this action. */
	inline bool GetUseSpeechAnimation() const{ return pUseSpeechAnimation; }
	/** \brief Sets if speech animation is used for this action. */
	void SetUseSpeechAnimation( bool useSpeechAnimation );
	
	/** Create a copy of this action. */
    virtual ceConversationAction *CreateCopy() const;
	/*@}*/
	
private:
	void pLoadSound();
};

#endif
