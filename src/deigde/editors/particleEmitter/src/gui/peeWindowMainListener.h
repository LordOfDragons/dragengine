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

#ifndef _PEEWINDOWMAINLISTENER_H_
#define _PEEWINDOWMAINLISTENER_H_

#include "../emitter/peeEmitterListener.h"

class peeWindowMain;



/**
 * \brief Window Main Listener.
 */
class peeWindowMainListener : public peeEmitterListener{
private:
	peeWindowMain &pWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	peeWindowMainListener(peeWindowMain &window);
	
	/** \brief Clean up listener. */
	~peeWindowMainListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Changed or saved state changed. */
	virtual void StateChanged(peeEmitter *emitter);
	
	/** \brief Undo changed. */
	virtual void UndoChanged(peeEmitter *emitter);
	
	/** \brief Emitter changed. */
	virtual void EmitterChanged(peeEmitter *emitter);
	
	/** \brief Controller count or order changed. */
	virtual void ControllerStructureChanged(peeEmitter *emitter);
	
	/** Active controller changed. */
	virtual void ActiveControllerChanged(peeEmitter *emitter);
	
	/** \brief Type count or order changed. */
	virtual void TypeStructureChanged(peeEmitter *emitter);
	
	/** \brief Active type active trail controller changed. */
	virtual void TypeActiveTrailControllerChanged(peeEmitter *emitter, peeType *type);
	
	/** \brief Active type active emit controller changed. */
	virtual void TypeActiveEmitControllerChanged(peeEmitter *emitter, peeType *type);
	
	/** \brief Active type parameter changed. */
	virtual void ActiveTypeParameterChanged(peeEmitter *emitter, peeType *type);
	
	/** \brief Active type changed. */
	virtual void ActiveTypeChanged(peeEmitter *emitter);
	/*@}*/
};

#endif
