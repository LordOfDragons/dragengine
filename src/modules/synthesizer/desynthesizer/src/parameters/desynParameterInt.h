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

#ifndef _DESYNPARAMETERINT_H_
#define _DESYNPARAMETERINT_H_

#include "desynParameter.h"


/**
 * \brief Int parameter.
 * 
 * Base class for all openal parameters. Every parameter stores information about
 * the parameter itself and provides methods to retrieves or alter the current value.
 */
class desynParameterInt : public desynParameter{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create parameter. */
	desynParameterInt(deDESynthesizer &synthesizer);
	/*@}*/
	
	
	
	/** \name Parameter Value */
	/*@{*/
	/** \brief Current value. */
	virtual decString GetParameterValue();
	
	/** \brief Set current value. */
	void SetParameterValue(const char *value) override;
	
	/** \brief Current value. */
	virtual int GetParameterInt() = 0;
	
	/** \brief Set current value. */
	virtual void SetParameterInt(int value) = 0;
	/*@}*/
};

#endif
