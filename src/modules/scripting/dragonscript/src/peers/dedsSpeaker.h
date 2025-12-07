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

#ifndef _DEDSSPEAKER_H_
#define _DEDSSPEAKER_H_

#include <dragengine/systems/modules/scripting/deBaseScriptingSpeaker.h>

class deScriptingDragonScript;
class deSpeaker;
class dsValue;
class dsRealObject;



/**
 * \brief Speaker peer.
 */
class dedsSpeaker : public deBaseScriptingSpeaker{
private:
	deScriptingDragonScript &pDS;
	deSpeaker *pSpeaker;
	
	dsValue *pValueOwner;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	dedsSpeaker(deScriptingDragonScript &ds, deSpeaker *speaker);
	
	/** \brief Clean up peer. */
	virtual ~dedsSpeaker();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Owner object or \em NULL. */
	dsRealObject *GetOwner() const;
	
	/** \brief Set owner object or \em NULL. */
	void SetOwner(dsRealObject *object);
	/*@}*/
};

#endif
