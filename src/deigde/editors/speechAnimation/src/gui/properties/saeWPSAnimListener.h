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
private:
	saeWPSAnim &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create listener. */
	saeWPSAnimListener( saeWPSAnim &panel );
	
	/** Clean up listener. */
	virtual ~saeWPSAnimListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Speech animation changed. */
	virtual void SAnimationChanged( saeSAnimation *sanimation );
	
	/** Model changed. */
	virtual void ModelChanged( saeSAnimation *sanimation );
	
	/** Phoneme count or order changed. */
	virtual void PhonemeStructureChanged( saeSAnimation *sanimation );
	
	/** Phoneme changed. */
	virtual void PhonemeChanged( saeSAnimation *sanimation, saePhoneme *phoneme );
	
	/** Active phoneme changed. */
	virtual void ActivePhonemeChanged( saeSAnimation *sanimation );
	
	/** Word count or order changed. */
	virtual void WordStructureChanged( saeSAnimation *sanimation );
	
	/** Word name changed. */
	virtual void WordNameChanged( saeSAnimation *sanimation, saeWord *word );
	
	/** Word changed. */
	virtual void WordChanged( saeSAnimation *sanimation, saeWord *word );
	
	/** Active word changed. */
	virtual void ActiveWordChanged( saeSAnimation *sanimation );
	/*@}*/
};

#endif
