/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEDSPARAMETERLIST_H_
#define _DEDSPARAMETERLIST_H_

#include "dedsParameter.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>


/**
 * Parameter list.
 */
class dedsParameterList{
private:
	decObjectOrderedSet pParameters;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create parameter list. */
	dedsParameterList() = default;
	
	/** Clean up parameter list. */
	~dedsParameterList() = default;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Number of parameters. */
	int GetParameterCount() const;
	
	/** Index of named parameter or -1 if absent. */
	int IndexOfParameterNamed(const char *name) const;
	
	/** Parameter at index. */
	dedsParameter &GetParameterAt(int index) const;
	
	/** Named parameter. */
	dedsParameter &GetParameterNamed(const char *name) const;
	
	/** Add parameter. */
	void AddParameter(const dedsParameter::Ref &parameter);
	
	/** Remove all parameters. */
	void RemoveAllParameters();
	/*@}*/
};

#endif
