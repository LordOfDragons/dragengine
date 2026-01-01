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

#ifndef _MEIDGROUP_H_
#define _MEIDGROUP_H_

#include "meIDGroupID.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decString.h>


/**
 * Identifier group.
 */
class meIDGroup : public deObject{
public:
	typedef deTObjectReference<meIDGroup> Ref;
	typedef decTObjectOrderedSet<meIDGroup> List;
	
	
private:
	decString pName;
	meIDGroupID::Map pIDs;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create id group. */
	explicit meIDGroup(const char *name);
	
protected:
	/** Clean up id group. */
	~meIDGroup() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Identifier. */
	inline const decString &GetName() const{ return pName; }
	
	
	
	/** Identifier map. */
	inline const meIDGroupID::Map &GetIDMap() const{ return pIDs; }
	
	/** List of identifiers. */
	decStringList GetIDList() const;
	
	/** Get identifier usage count or 0 if absent. */
	int GetUsageCountFor(const char *id) const;
	
	/** Increase usage count of identifier adding it if absent. */
	void Add(const char *id);
	
	/** Decrease usage count of identifier removing it if dropping to 0. */
	void Remove(const char *id);
	
	/** Remove all identifiers. */
	void RemoveAll();
	/*@}*/
};

#endif
