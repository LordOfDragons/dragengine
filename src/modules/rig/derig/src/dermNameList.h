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

#ifndef _DERMNAMELIST_H_
#define _DERMNAMELIST_H_

#include <dragengine/common/string/decString.h>


/**
 * List of names.
 */
class dermNameList{
private:
	struct sName{
		decString name;
		int number;
	};
	
	sName *pNames;
	int pNameCount;
	int pNameSize;
	


public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new name list. */
	dermNameList();

	/** Cleans up the name list. */
	~dermNameList();
	/*@}*/
	


	/** @name Management */
	/*@{*/
	/** Retrieves the number of names. */
	inline int GetNameCount() const{return pNameCount;}

	/** Retrieves the name at the given index. */
	const decString &GetNameAt(int index) const;

	/** Retrieves the number of the name at the given index. */
	int GetNameNumberAt(int index) const;

	/** Sets the number of the name at the given index. */
	void SetNameNumberAt(int index, int number);

	/** Retrieves the index of the given name or -1 if not found. */
	int IndexOfName(const char *name) const;

	/** Determines if the given name exists. */
	bool HasName(const char *name) const;

	/** Adds a new name. */
	int AddName(const char *name);
	/*@}*/
};

#endif
