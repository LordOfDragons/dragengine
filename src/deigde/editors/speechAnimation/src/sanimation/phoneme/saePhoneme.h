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

#ifndef _SAEPHONEME_H_
#define _SAEPHONEME_H_

#include <dragengine/deObject.h>

#include <dragengine/common/string/decString.h>

class saeSAnimation;


/**
 * Speech Animation Phoneme.
 */
class saePhoneme : public deObject{
private:
	saeSAnimation *pSAnimation;
	int pEngController;
	
	int pIPA;
	decString pSampleText;
	decString pMoveName;
	decString pVertexPositionSet;
	float pLength;
	bool pActive;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<saePhoneme> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create phoneme. */
	saePhoneme();
	
	/** Create phoneme. */
	saePhoneme(int ipa);
	
protected:
	/** Clean up phoneme. */
	virtual ~saePhoneme();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Parent speech animation. */
	inline saeSAnimation *GetSAnimation() const{return pSAnimation;}
	
	/** Set parent speech animation. */
	void SetSAnimation(saeSAnimation *sanimation);
	
	/** Engine controller or -1 if not used. */
	inline int GetEngineController() const{return pEngController;}
	
	/** Set engine controller or -1 if not used. */
	void SetEngineController(int controller);
	
	/** IPA symbol. */
	inline int GetIPA() const{return pIPA;}
	
	/** Set IPA symbol. */
	void SetIPA(int ipa);
	
	/** Length of phoneme. */
	inline float GetLength() const{return pLength;}
	
	/** Set length of phoneme. */
	void SetLength(float length);
	
	/** Sample text. */
	inline const decString &GetSampleText() const{return pSampleText;}
	
	/** Set sample text. */
	void SetSampleText(const char *sampleText);
	
	/** Move name. */
	inline const decString &GetMoveName() const{return pMoveName;}
	
	/** Set move name. */
	void SetMoveName(const char *name);
	
	/** Vertex position set. */
	inline const decString &GetVertexPositionSet() const{return pVertexPositionSet;}
	
	/** Set vertex position set. */
	void SetVertexPositionSet(const char *name);
	
	/** Active phoneme. */
	inline bool GetActive() const{return pActive;}
	
	/** Set if active phoneme. */
	void SetActive(bool active);
	/*@}*/
};

#endif
