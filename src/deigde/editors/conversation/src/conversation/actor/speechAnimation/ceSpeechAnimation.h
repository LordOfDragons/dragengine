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
#include <dragengine/common/string/decStringSet.h>
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
	decStringSet pNeutralVertexPositionSets;
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
	/** Create speech animation. */
	ceSpeechAnimation( deEngine *engine );
	
	/** Clean up speech animation. */
	~ceSpeechAnimation();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** Animator. */
	inline deAnimator *GetEngineAnimator() const{ return pEngAnimator; }
	
	/** Animator instance. */
	inline deAnimatorInstance *GetEngineAnimatorInstance() const{ return pEngAnimatorInstance; }
	
	/** Neutral move name. */
	inline const decString &GetNeutralMoveName() const{ return pNeutralMoveName; }
	
	/** Set neutral move name. */
	void SetNeutralMoveName( const char *name );
	
	/** Neutral vertex position sets. */
	inline decStringSet &GetNeutralVertexPositionSets(){ return pNeutralVertexPositionSets; }
	inline const decStringSet &GetNeutralVertexPositionSets() const{ return pNeutralVertexPositionSets; }
	
	/** Phoneme list. */
	inline ceSAPhonemeList &GetPhonemeList(){ return pPhonemeList; }
	inline const ceSAPhonemeList &GetPhonemeList() const{ return pPhonemeList; }
	
	/** Word list. */
	inline ceSAWordList &GetWordList(){ return pWordList; }
	inline const ceSAWordList &GetWordList() const{ return pWordList; }
	
	/** Remove all speak phonemes. */
	void RemoveAllSpeakPhonemes();
	
	/** Add speak phoneme. */
	void AddSpeakPhoneme( int phoneme, float length );
	
	/** Length of speaking. */
	inline float GetSpeakLength() const{ return pSpeakLength; }
	
	/** Text is still spoken. */
	inline bool GetSpeaking() const{ return pSpeaking; }
	
	/** Create animator. */
	void CreateAnimator();
	
	/** Update speech animation. */
	void Update( float elapsed );
	
	/** Clear speech animation. */
	void Clear();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
