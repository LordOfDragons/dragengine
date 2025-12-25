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

#ifndef _SAEWORD_H_
#define _SAEWORD_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class saeSAnimation;



/**
 * Speech Animation Word.
 */
class saeWord : public deObject{
private:
	saeSAnimation *pSAnimation;
	
	decString pName;
	decUnicodeString pPhonetics;
	
	bool pActive;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<saeWord> Ref;

	/** \brief List of objects. */
	typedef decTOrderedSet<deTObjectReference<saeWord>, saeWord*> List;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create word. */
	saeWord();
	
	/** Create word. */
	explicit saeWord(const char *name);
	
	/** Create word. */
	saeWord(const char *name, const decUnicodeString &phonetics);
	
	/** Cleans up the word. */
	virtual ~saeWord();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the parent speech animation. */
	inline saeSAnimation *GetSAnimation() const{ return pSAnimation; }
	/** Sets the parent speech animation. */
	void SetSAnimation(saeSAnimation *sanimation);
	
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName(const char *name);
	/** Retrieves the phonetics. */
	inline const decUnicodeString &GetPhonetics() const{ return pPhonetics; }
	/** Sets the phonetics. */
	void SetPhonetics(const decUnicodeString &phonetics);
	/** Determines if this is the active word. */
	inline bool GetActive() const{ return pActive; }
	/** Sets if this is the active word. */
	void SetActive(bool active);
	/*@}*/
};

#endif
