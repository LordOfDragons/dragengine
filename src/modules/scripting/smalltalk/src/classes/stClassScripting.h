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

#ifndef _STCLASSSCRIPTING_H_
#define _STCLASSSCRIPTING_H_

#include "../stCommon.h"

class ScriptingSmalltalk;



/**
 * \brief Script class Scripting.
 *
 * Internal scripting class used only to bridge between smalltalk
 * land and c++ land. There exists only one instance of this
 * class and it is not accessible by to smalltalk at all.
 */
class stClassScripting{
private:
	ScriptingSmalltalk *pST;
	OOP pSingleton;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	stClassScripting(ScriptingSmalltalk *st);
	
	/** \brief Cleans up class. */
	virtual ~stClassScripting();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Script module instance.
	 * \details Static call as it is used to obtain the entry point into c++ land from smalltalk.
	 */
	static ScriptingSmalltalk &GetSTFromOOP(OOP object);
	
	/** \brief Singleton smalltalk object. */
	inline OOP GetSingleton() const{ return pSingleton; }
	
	/** \brief Create singleton. */
	void CreateSingleton();
	
	/** \brief Free singleton. */
	void FreeSingleton();
	/*@}*/
};

#endif
