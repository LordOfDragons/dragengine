/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDENATIVEBEOSAPPLICATION_H_
#define _IGDENATIVEBEOSAPPLICATION_H_

#include "beostoolkit.h"
#include "../../igdeApplication.h"

class igdeApplication;


/**
 * BeOS Native application.
 */
class igdeNativeBeosApplication : public igdeApplication::cNativeApplication{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native application. */
	igdeNativeBeosApplication();
	
	/** \brief Clean up native application. */
	virtual ~igdeNativeBeosApplication();
	
	/** \brief Create native application. */
	static igdeNativeBeosApplication* CreateNativeApplication(igdeApplication &owner);
	
	/** \brief Post create native application. */
	virtual void PostCreateNativeApplication();
	
	/** \brief Run application. */
	virtual int RunApplication();
	/*@}*/
};

typedef igdeNativeBeosApplication igdeNativeApplication;

#endif
