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

#ifndef _PROJTRPPARAMETERLIST_H_
#define _PROJTRPPARAMETERLIST_H_

#include "projTRPParameter.h"

#include <dragengine/common/collection/decTOrderedSet.h>


/**
 * Profile module parameters.
 */
class projTRPParameterList{
private:
	decTObjectOrderedSet<projTRPParameter> pParameters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create profile module parameters. */
	projTRPParameterList();
	
	/** Clean up profile module parameters. */
	~projTRPParameterList();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Parameters. */
	inline const decTObjectOrderedSet<projTRPParameter> &GetParameters() const{ return pParameters; }
	
	/** Named parameter with module or \em nullptr if absent. */
	projTRPParameter *GetWith(const char *module, const char *name) const;
	
	/** Named parameter with module is present. */
	bool HasWith(const char *module, const char *name) const;
	
	/** Index of named parameter with module or -1 if absent. */
	int IndexOfWith(const char *module, const char *name) const;
	
	/** Add parameter. */
	void Add(projTRPParameter *parameter);
	
	/** Remove parameter. */
	void Remove(projTRPParameter *parameter);
	
	/** Remove named parameter with module if present. */
	void RemoveWith(const char *module, const char *name);
	
	/** Remove all parameters. */
	void RemoveAll();
	
	
	/** Set parameter. */
	void Set(const char *module, const char *name, const char *value);
	
	/** Set parameter. */
	void Set(const projTRPParameter &parameter);
	/*@}*/
};

#endif
