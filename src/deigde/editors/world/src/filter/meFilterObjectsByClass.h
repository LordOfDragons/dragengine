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
#ifndef _MEFILTEROBJECTSBYCLASS_H_
#define _MEFILTEROBJECTSBYCLASS_H_

// includes
#include "meFilterObjects.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>

// predefinitions
class meObject;



/**
 * @brief Filter Objects By Class.
 *
 * A filter accepting objects based on their class name. Supports
 * inclusive or exclusive matching. Using inclusive matching only
 * objects with a class name matching any of the specified class
 * names are accepted. Using exclusive matching only objects with
 * a class name not matching any of the specified class names are
 * accepted. By default exclusive matching is used. In addition
 * an object to ignore can be specified. This is useful for
 * situations where an object should not accept itself. Furthermore
 * ghost objects can be accepted if required. By default ghost
 * objects are rejected.
 */
class meFilterObjectsByClass : public meFilterObjects{
private:
	decStringSet pClassNames;
	bool pMatchInclusive;
	
	meObject *pRejectObject;
	bool pRejectGhosts;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new filter. */
	meFilterObjectsByClass();
	/** Cleans up the filter. */
	virtual ~meFilterObjectsByClass();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Determines if inclusive or exlusive matching has to be used. */
	inline bool GetMatchInclusive() const{ return pMatchInclusive; }
	/** Sets if inclusive or exclusive matching has to be used. */
	void SetMatchInclusive(bool matchInclusive);
	/** Determines if ghost objects are rejected. */
	inline bool GetRejectGhosts() const{ return pRejectGhosts; }
	/** Sets if ghost objects are rejected. */
	void SetRejectGhosts(bool rejectGhosts);
	/** Retrieves the object to reject or NULL. */
	inline meObject *GetRejectObject() const{ return pRejectObject; }
	/** Sets the object to reject or NULL. */
	void SetRejectObject(meObject *object);
	
	/** Adds a class name. */
	void AddClassName(const char *className);
	/** Removes all class names. */
	void RemoveAllClassNames();
	/** Sets class names set. */
	void SetClassNamesFrom(const decStringSet &set);
	
	/**
	 * Determines if the given object is accepted by the filter.
	 */
	virtual bool AcceptObject(meObject *object) const;
	/*@}*/
};

// end of include only once
#endif
