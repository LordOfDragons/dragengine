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

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/common/string/decString.h>

class meIDGroupID;


/**
 * \brief Identifier group.
 */
class meIDGroup : public deObject{
private:
	decString pName;
	decObjectDictionary pIDs;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create id group. */
	meIDGroup( const char *name );
	
	/** \brief Clean up id group. */
	virtual ~meIDGroup();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Identifier. */
	inline const decString &GetName() const{ return pName; }
	
	
	
	/** \brief Number of identifiers. */
	int GetCount() const;
	
	/** \brief List of identifiers. */
	decStringList GetIDList() const;
	
	/** \brief Get identifier usage count or 0 if absent. */
	int GetUsageCountFor( const char *id ) const;
	
	/** \brief Identifier is present. */
	bool Has( const char *id ) const;
	
	/** \brief Increase usage count of identifier adding it if absent. */
	void Add( const char *id );
	
	/** \brief Decrease usage count of identifier removing it if dropping to 0. */
	void Remove( const char *id );
	
	/** \brief Remove all identifiers. */
	void RemoveAll();
	/*@}*/
};

#endif
