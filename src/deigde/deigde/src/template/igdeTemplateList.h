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

#ifndef _IGDETEMPLATELIST_H_
#define _IGDETEMPLATELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeTemplate;



/**
 * \brief Project template list.
 */
class igdeTemplateList{
private:
	decObjectOrderedSet pTemplates;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create project template list. */
	igdeTemplateList();
	
	/** \brief Create copy of project template list. */
	igdeTemplateList( const igdeTemplateList &list );
	
	/** \brief Clean up project template list. */
	~igdeTemplateList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of project templates. */
	int GetCount() const;
	
	/** \brief Project template at index. */
	igdeTemplate *GetAt( int index ) const;
	
	/** \brief Index of project template or -1 if absent. */
	int IndexOf( igdeTemplate *atemplate ) const;
	
	/** \brief Project template is present. */
	bool Has( igdeTemplate *atemplate ) const;
	
	/** \brief Add project template. */
	void Add( igdeTemplate *atemplate );
	
	/** \brief Remove project template. */
	void Remove( igdeTemplate *atemplate );
	
	/** \brief Remove all project templates. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy list. */
	igdeTemplateList &operator=( const igdeTemplateList &list );
	/*@}*/
};

#endif
