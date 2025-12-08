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

#ifndef _CECAACTORSPEAK_H_
#define _CECAACTORSPEAK_H_

#include "ceConversationAction.h"
#include "../strip/ceStripList.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class ceConversation;

class deEngine;
#include <dragengine/resources/sound/deSound.h>



/**
 * \brief Actor Speak Conversation Action.
 * Make an actor speak. Starts playing a sound file while doing lip-sync using a speech
 * animation file.
 */
class ceCAActorSpeak : public ceConversationAction{
public:
	typedef deTObjectReference<ceCAActorSpeak> Ref;
	
	
private:
	deEngine *pEngine;
	deSound::Ref pEngSound;
	bool pLoaded;
	
	decString pActor;
	decUnicodeString pTextBoxText;
	decString pTextBoxTextTranslate;
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
	ceCAActorSpeak(deEngine *engine);
	/** Creates a new actor speak conversation action. */
	ceCAActorSpeak(const ceCAActorSpeak &action);
	/** Cleans up the actor speak conversation action. */
	~ceCAActorSpeak() override;
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
	void SetActor(const char *id);
	
	/** Retrieves the text box text. */
	inline const decUnicodeString &GetTextBoxText() const{ return pTextBoxText; }
	
	/** Sets the text box text. */
	void SetTextBoxText(const decUnicodeString &text);
	
	/** Translated text box text. */
	inline const decString &GetTextBoxTextTranslate() const{ return pTextBoxTextTranslate; }
	
	/** Set translated text box text. */
	void SetTextBoxTextTranslate(const char *text);
	
	/** Resolved text box text. */
	decUnicodeString ResolveTextBoxText(const ceConversation &conversation) const;
	
	/** \brief Retrieves the text box text style. */
	inline const decString &GetTextBoxTextStyle() const{ return pTextBoxTextStyle; }
	/** \brief Sets the text box text style. */
	void SetTextBoxTextStyle(const char *style);
	/** Retrieves the path to the sound file or empty to use none. */
	inline const decString &GetPathSound() const{ return pPathSound; }
	/** Sets the path to the sound file or empty to use none. */
	void SetPathSound(const char *path);
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
	void SetMovement(const char *movement);
	/** \brief Retrieves the minimum time the actor is speaking. */
	inline float GetMinSpeechTime() const{ return pMinSpeechTime; }
	/** \brief Sets the minimum time the actor is speaking. */
	void SetMinSpeechTime(float minSpeechTime);
	
	/** \brief Determines if speech animation is used for this action. */
	inline bool GetUseSpeechAnimation() const{ return pUseSpeechAnimation; }
	/** \brief Sets if speech animation is used for this action. */
	void SetUseSpeechAnimation(bool useSpeechAnimation);
	
	/** Create a copy of this action. */
    ceConversationAction *CreateCopy() const override;
	/*@}*/
	
private:
	void pLoadSound();
};

#endif
