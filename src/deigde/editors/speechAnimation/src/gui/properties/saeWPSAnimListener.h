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

#ifndef _SAEWPSANIMLISTENER_H_
#define _SAEWPSANIMLISTENER_H_

#include "../../sanimation/saeSAnimationListener.h"

class saeWPSAnim;



/**
 * Speech Animation panel listener.
 */
class saeWPSAnimListener : public saeSAnimationListener{
public:
	typedef deTObjectReference<saeWPSAnimListener> Ref;
	
private:
	saeWPSAnim &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create listener. */
	saeWPSAnimListener(saeWPSAnim &panel);
	
	/** Clean up listener. */
protected:
	~saeWPSAnimListener() override;
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Speech animation changed. */
	void SAnimationChanged(saeSAnimation *sanimation) override;
	
	/** Model changed. */
	void ModelChanged(saeSAnimation *sanimation) override;
	
	/** Phoneme count or order changed. */
	void PhonemeStructureChanged(saeSAnimation *sanimation) override;
	
	/** Phoneme changed. */
	void PhonemeChanged(saeSAnimation *sanimation, saePhoneme *phoneme) override;
	
	/** Active phoneme changed. */
	void ActivePhonemeChanged(saeSAnimation *sanimation) override;
	
	/** Word count or order changed. */
	void WordStructureChanged(saeSAnimation *sanimation) override;
	
	/** Word name changed. */
	void WordNameChanged(saeSAnimation *sanimation, saeWord *word) override;
	
	/** Word changed. */
	void WordChanged(saeSAnimation *sanimation, saeWord *word) override;
	
	/** Active word changed. */
	void ActiveWordChanged(saeSAnimation *sanimation) override;
	/*@}*/
};

#endif
