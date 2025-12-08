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

#ifndef _MEMAPIDGROUP_H_
#define _MEMAPIDGROUP_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include "meIDGroup.h"
#include "igdeGDProperty.h"



/**
 * Mapping between game definition property and Identifier group.
 */
class meMapIDGroup : public deObject{
public:
	typedef deTObjectReference<meMapIDGroup> Ref;
	
	
private:
	igdeGDProperty::Ref pProperty;
	meIDGroup::Ref pGroup;
	const decString pPropertyPrefix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create mapping between game definition property and Identifier group. */
	meMapIDGroup(igdeGDProperty *property, meIDGroup *group, const decString &propertyPrefix);
	
	/** Clean up mapping between game definition property and Identifier group. */
	virtual ~meMapIDGroup();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Game definition property. */
	inline const igdeGDProperty::Ref &GetProperty() const{ return pProperty; }
	
	/** Identifier group. */
	inline const meIDGroup::Ref &GetGroup() const{ return pGroup; }
	
	/** Property prefix. */
	inline const decString &GetPropertyPrefix() const{ return pPropertyPrefix; }
	/*@}*/
};

#endif
