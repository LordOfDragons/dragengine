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

// include only once
#ifndef _DEOBJECT_H_
#define _DEOBJECT_H_


/**
 * @brief Basic Object Interface.
 *
 * Provides the basic class for all engine objects. Each object has a
 * reference count. Objects stay alive as long as the reference count
 * is larger than 0. If you want to add a reference use the AddReference
 * function. If you want to free the reference you have added before use
 * the FreeReference function and do not call 'delete'. Upon creation
 * each object is assigned a reference count of 1 so you do not have to
 * add a refernece after creating an object.
 */
class deObject{
private:
	int pRefCount;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new object with a reference count of 1. */
	deObject();
	/** Cleans up the resource. */
	virtual ~deObject();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the reference count. */
	inline int GetRefCount() const{ return pRefCount; }
	/** Adds a reference and increases the reference count by 1. */
	void AddReference();
	/** Decreases the reference count by one and deletes the object if the count reaches 0. */
	void FreeReference();
	/*@}*/
};

// end of include only once
#endif
