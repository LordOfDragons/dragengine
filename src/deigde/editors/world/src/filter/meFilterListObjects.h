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
#ifndef _MEFILTERLISTOBJECTS_H_
#define _MEFILTERLISTOBJECTS_H_

// includes
#include "meFilterObjects.h"

// predefinitions
class meObject;



/**
 * @brief Filter List Objects.
 *
 * A list of object filters acting as one object filter. For each
 * object the list of filters is checked. If the acceptAny flag
 * is set an object is accepted if at last one filter in the list
 * accepts the object. Otherwise the object is only accepted if
 * all filters accept it. By default acceptAny is true.
 */
class meFilterListObjects : public meFilterObjects{
	meFilterObjects **pFilters;
	int pFilterCount;
	
	bool pAcceptAny;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new filter list. */
	meFilterListObjects();
	/** Cleans up the filter list. */
	virtual ~meFilterListObjects();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Determines if any filter can accept the object or only all together. */
	inline bool GetAcceptAny() const{return pAcceptAny;}
	/** Sets if any filter can accept the object or only all together. */
	void SetAcceptAny(bool acceptAny);
	
	/** Adds a filter. */
	void AddFilter(meFilterObjects *filter);
	/** Removes all filters. */
	void RemoveAllFilters();
	
	/**
	 * Determines if the given object is accepted by the filter.
	 */
	virtual bool AcceptObject(meObject *object) const;
	/*@}*/
};

// end of include only once
#endif
