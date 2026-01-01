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

#ifndef _CESPEECHANIMATION_H_
#define _CESPEECHANIMATION_H_

#include "ceSAPhoneme.h"
#include "ceSAWord.h"

#include <dragengine/common/collection/decTDictionary.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>

class deEngine;



/**
 * \brief Speech Animation.
 */
class ceSpeechAnimation : public deObject{
public:
	typedef deTObjectReference<ceSpeechAnimation> Ref;
	
	
private:
	class cSpeakPhoneme : public deObject{
	public:
		typedef deTObjectReference<cSpeakPhoneme> Ref;
		int ipa;
		float length;
		cSpeakPhoneme(int aipa, float alength) : ipa(aipa), length(alength){}
	};
	
	deEngine *pEngine;
	
	deAnimator::Ref pEngAnimator;
	deAnimatorInstance::Ref pEngAnimatorInstance;
	
	decString pNeutralMoveName;
	decStringSet pNeutralVertexPositionSets;
	ceSAPhoneme::Map pPhonemes;
	ceSAWord::List pWords;
	
	decTObjectList<cSpeakPhoneme> pSpeakPhonemes;
	float pSpeakLength;
	int pSpeakPos;
	float pSpeakElapsed;
	bool pSpeaking;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create speech animation. */
	ceSpeechAnimation(deEngine *engine);
	
	/** Clean up speech animation. */
protected:
	~ceSpeechAnimation();
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** Animator. */
	inline const deAnimator::Ref &GetEngineAnimator() const{ return pEngAnimator; }
	
	/** Animator instance. */
	inline const deAnimatorInstance::Ref &GetEngineAnimatorInstance() const{ return pEngAnimatorInstance; }
	
	/** Neutral move name. */
	inline const decString &GetNeutralMoveName() const{ return pNeutralMoveName; }
	
	/** Set neutral move name. */
	void SetNeutralMoveName(const char *name);
	
	/** Neutral vertex position sets. */
	inline decStringSet &GetNeutralVertexPositionSets(){ return pNeutralVertexPositionSets; }
	inline const decStringSet &GetNeutralVertexPositionSets() const{ return pNeutralVertexPositionSets; }
	
	/** Phoneme list. */
	inline ceSAPhoneme::Map &GetPhonemes(){ return pPhonemes; }
	inline const ceSAPhoneme::Map &GetPhonemes() const{ return pPhonemes; }
	
	/** Word list. */
	inline ceSAWord::List &GetWordList(){ return pWords; }
	inline const ceSAWord::List &GetWordList() const{ return pWords; }
	
	/** Remove all speak phonemes. */
	void RemoveAllSpeakPhonemes();
	
	/** Add speak phoneme. */
	void AddSpeakPhoneme(int ipa, float length);
	
	/** Length of speaking. */
	inline float GetSpeakLength() const{ return pSpeakLength; }
	
	/** Text is still spoken. */
	inline bool GetSpeaking() const{ return pSpeaking; }
	
	/** Create animator. */
	void CreateAnimator();
	
	/** Update speech animation. */
	void Update(float elapsed);
	
	/** Clear speech animation. */
	void Clear();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
