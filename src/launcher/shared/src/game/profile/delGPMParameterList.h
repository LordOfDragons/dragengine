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

#ifndef _DELGPMPARAMETERLIST_H_
#define _DELGPMPARAMETERLIST_H_

#include <dragengine/common/collection/decObjectList.h>

class delGPMParameter;


/**
 * \brief Game Profile Module Parameter List.
 */
class DE_DLL_EXPORT delGPMParameterList{
private:
	decObjectList pParameters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	delGPMParameterList();
	
	/** \brief Clean up list. */
	~delGPMParameterList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of parameters. */
	int GetCount() const;
	
	/** \brief Parameter at index. */
	delGPMParameter *GetAt(int index) const;
	
	/** \brief Named parameter or nullptr if absent. */
	delGPMParameter *GetNamed(const char *name) const;
	
	/** \brief Parameter is present. */
	bool Has(delGPMParameter *parameter) const;
	
	/** \brief Named parameter is present. */
	bool HasNamed(const char *name) const;
	
	/** \brief Index of parameter or -1 if absent. */
	int IndexOf(delGPMParameter *parameter) const;
	
	/** \brief Index of named parameter or -1 if absent. */
	int IndexOfNamed(const char *name) const;
	
	/** \brief Add parameter. */
	void Add(delGPMParameter *parameter);
	
	/** \brief Remove parameter. */
	void Remove(delGPMParameter *parameter);
	
	/** \brief Remove all parameters. */
	void RemoveAll();
	/*@}*/
};

#endif
