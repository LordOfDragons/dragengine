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

#ifndef _IGDETEMPLATEREPLACELIST_H_
#define _IGDETEMPLATEREPLACELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeTemplateReplace;



/**
 * \brief Project template replace list.
 */
class igdeTemplateReplaceList{
private:
	decObjectOrderedSet pReplaces;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create project template replace list. */
	igdeTemplateReplaceList();
	
	/** \brief Create copy of project template replace list. */
	igdeTemplateReplaceList( const igdeTemplateReplaceList &list );
	
	/** \brief Clean up project template replace list. */
	~igdeTemplateReplaceList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of project template replaces. */
	int GetCount() const;
	
	/** \brief Project template replace at index. */
	igdeTemplateReplace *GetAt( int index ) const;
	
	/** \brief Index of project template replace or -1 if absent. */
	int IndexOf( igdeTemplateReplace *replace ) const;
	
	/** \brief Project template replace is present. */
	bool Has( igdeTemplateReplace *replace ) const;
	
	/** \brief Add project template replace. */
	void Add( igdeTemplateReplace *replace );
	
	/** \brief Remove project template replace. */
	void Remove( igdeTemplateReplace *replace );
	
	/** \brief Remove all project template replaces. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy list. */
	igdeTemplateReplaceList &operator=( const igdeTemplateReplaceList &list );
	/*@}*/
};

#endif
