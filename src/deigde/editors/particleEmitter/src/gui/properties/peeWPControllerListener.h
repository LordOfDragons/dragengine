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

#ifndef _PEEWPCONTROLLERLISTENER_H_
#define _PEEWPCONTROLLERLISTENER_H_

#include "../../emitter/peeEmitterListener.h"

class peeWPController;



/**
 * \brief Controller Properties Panel Listener.
 */
class peeWPControllerListener : public peeEmitterListener{
private:
	peeWPController &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	peeWPControllerListener( peeWPController &panel );
	
	/** \brief Clean up listener. */
	virtual ~peeWPControllerListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Controller added or removed. */
	virtual void ControllerStructureChanged ( peeEmitter *emitter );
	
	/** \brief Controller changed. */
	virtual void ControllerChanged( peeEmitter *emitter, peeController *controller );
	
	/** \brief Controller name changed. */
	virtual void ControllerNameChanged( peeEmitter *emitter, peeController *controller );
	
	/** \brief Controller changed value. */
	virtual void ControllerValueChanged( peeEmitter *emitter, peeController *controller );
	
	/** \brief Active controller changed. */
	virtual void ActiveControllerChanged( peeEmitter *emitter );
	/*@}*/
};

#endif
