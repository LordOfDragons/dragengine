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

#ifndef _PEEWINDOWCURVESLISTENER_H_
#define _PEEWINDOWCURVESLISTENER_H_

#include "../../emitter/peeEmitterListener.h"

class peeWindowCurves;



/**
 * \brief Curves Window Listener.
 */
class peeWindowCurvesListener : public peeEmitterListener{
private:
	peeWindowCurves &pWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	peeWindowCurvesListener( peeWindowCurves &window );
	
	/** \brief Clean up listener. */
	virtual ~peeWindowCurvesListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Type count or order changed. */
	virtual void TypeStructureChanged( peeEmitter *emitter );
	
	/** \brief A type parameter changed. */
	virtual void TypeParameterChanged( peeEmitter *emitter, peeType *type, peeParameter *parameter );
	
	/** \brief Active type parameter changed. */
	virtual void ActiveTypeParameterChanged( peeEmitter *emitter, peeType *type );
	
	/** \brief Active type changed. */
	virtual void ActiveTypeChanged( peeEmitter *emitter );
	/*@}*/
};

#endif
