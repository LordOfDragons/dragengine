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

#ifndef _SAEWINDOWMAINLISTENER_H_
#define _SAEWINDOWMAINLISTENER_H_

#include "../sanimation/saeSAnimationListener.h"

class saeWindowMain;



/**
 * Window Main Listener.
 */
class saeWindowMainListener : public saeSAnimationListener{
private:
	saeWindowMain &pWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create listener. */
	saeWindowMainListener( saeWindowMain &window );
	
	/** Clean up listener. */
	virtual ~saeWindowMainListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Changed or saved state changed. */
	virtual void StateChanged( saeSAnimation *sanimation );
	
	/** Undo changed. */
	virtual void UndoChanged( saeSAnimation *sanimation );
	
	/** View properties changed. */
	virtual void ViewChanged( saeSAnimation *sanimation );
	
	/** Speech animation changed. */
	virtual void SAnimationChanged( saeSAnimation *sanimation );
	
	
	
	/** Phoneme count or order changed. */
	virtual void PhonemeStructureChanged( saeSAnimation *sanimation );
	
	/** Active phoneme changed. */
	virtual void ActivePhonemeChanged( saeSAnimation *sanimation );
	
	
	
	/** Word count or order changed. */
	virtual void WordStructureChanged( saeSAnimation *sanimation );
	
	/** Active word changed. */
	virtual void ActiveWordChanged( saeSAnimation *sanimation );
	/*@}*/
};

#endif
