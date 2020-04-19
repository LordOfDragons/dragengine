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

#ifndef _CESPEECHANIMATION_H_
#define _CESPEECHANIMATION_H_

#include "ceSAPhonemeList.h"
#include "ceSAWordList.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class deEngine;
class deAnimator;
class deAnimatorInstance;



/**
 * \brief Speech Animation.
 */
class ceSpeechAnimation{
private:
	struct sSpeakPhoneme{
		int phoneme;
		float length;
	};
	
	deEngine *pEngine;
	
	deAnimator *pEngAnimator;
	deAnimatorInstance *pEngAnimatorInstance;
	
	decString pNeutralMoveName;
	ceSAPhonemeList pPhonemeList;
	ceSAWordList pWordList;
	
	sSpeakPhoneme *pSpeakPhonemes;
	int pSpeakPhonemeCount;
	int pSpeakPhonemeSize;
	float pSpeakLength;
	int pSpeakPos;
	float pSpeakElapsed;
	bool pSpeaking;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new speech animation. */
	ceSpeechAnimation( deEngine *engine );
	/** Cleans up the speech animation. */
	~ceSpeechAnimation();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	/** Retrieves the animator. */
	inline deAnimator *GetEngineAnimator() const{ return pEngAnimator; }
	/** Retrieves the animator instance. */
	inline deAnimatorInstance *GetEngineAnimatorInstance() const{ return pEngAnimatorInstance; }
	
	/** Retrieves the neutral move name. */
	inline const decString &GetNeutralMoveName() const{ return pNeutralMoveName; }
	/** Sets the neutral move name. */
	void SetNeutralMoveName( const char *name );
	/** Retrieves the phoneme list. */
	inline ceSAPhonemeList &GetPhonemeList(){ return pPhonemeList; }
	inline const ceSAPhonemeList &GetPhonemeList() const{ return pPhonemeList; }
	/** Retrieves the word list. */
	inline ceSAWordList &GetWordList(){ return pWordList; }
	inline const ceSAWordList &GetWordList() const{ return pWordList; }
	/** Remove all speak phonemes. */
	void RemoveAllSpeakPhonemes();
	/** Adds a speak phoneme. */
	void AddSpeakPhoneme( int phoneme, float length );
	/** Retrieves the length of the speaking. */
	inline float GetSpeakLength() const{ return pSpeakLength; }
	/** Determines if the text is still spoken. */
	inline bool GetSpeaking() const{ return pSpeaking; }
	
	/** Create animator. */
	void CreateAnimator();
	
	/** Updates the speech animation. */
	void Update( float elapsed );
	/** Clears the speech animation. */
	void Clear();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
