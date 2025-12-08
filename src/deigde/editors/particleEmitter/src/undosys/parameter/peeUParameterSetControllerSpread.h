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

#ifndef _PEEUPARAMETERSETCONTROLLERSPREAD_H_
#define _PEEUPARAMETERSETCONTROLLERSPREAD_H_

#include <deigde/undo/igdeUndo.h>
#include "peeController.h"
#include "peeType.h"

class peeParameter;



/**
 * \brief Undo Action Set Parameter Spread Curve Controller.
 */
class peeUParameterSetControllerSpread : public igdeUndo{
private:
	peeType::Ref pType;
	peeParameter *pParameter;
	
	peeController::Ref pOldController;
	peeController::Ref pNewController;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<peeUParameterSetControllerSpread> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new undo action. */
	peeUParameterSetControllerSpread(peeType *type, peeParameter *parameter, peeController *newController);
	
protected:
	/** \brief Clean up the undo action. */
    virtual ~peeUParameterSetControllerSpread();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
