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

#ifndef _STCLASSGRAPHICSYSTEM_H_
#define _STCLASSGRAPHICSYSTEM_H_

#include "../../stCommon.h"

class ScriptingSmalltalk;



/**
 * \brief Script class graphic system.
 */
class stClassGraphicSystem{
private:
	ScriptingSmalltalk &pST;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	stClassGraphicSystem(ScriptingSmalltalk &st);
	
	/** \brief Clean up class. */
	virtual ~stClassGraphicSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline ScriptingSmalltalk &GetST() const{return pST;}
	
	/** \brief Set up links. */
	void SetUpLinks();
	/*@}*/
	
	
	
	/** \name cCall Methods */
	/*@{*/
	/** \brief Width of the render window. */
	static int ccGetWindowWidth(OOP self);
	
	/** \brief Height of the render window. */
	static int ccGetWindowHeight(OOP self);
	
	/** \brief Set window geometry. */
	static void ccSetWindowSize(OOP self, int width, int height);
	
	/** \brief Set window title. */
	static void ccSetWindowTitle(OOP self, const char *title);
	
	/** \brief Primary render target canvas view. */
	static OOP ccGetPrimaryCanvas(OOP self);
	/*@}*/
};

#endif
