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

#ifndef _IGDETEMPLATEFILELIST_H_
#define _IGDETEMPLATEFILELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeTemplateFile;



/**
 * \brief Project template file list.
 */
class igdeTemplateFileList{
private:
	decObjectOrderedSet pFiles;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create project template file list. */
	igdeTemplateFileList();
	
	/** \brief Create copy of project template file list. */
	igdeTemplateFileList( const igdeTemplateFileList &list );
	
	/** \brief Clean up project template file list. */
	~igdeTemplateFileList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of project template files. */
	int GetCount() const;
	
	/** \brief Project template file at index. */
	igdeTemplateFile *GetAt( int index ) const;
	
	/** \brief Index of project template file or -1 if absent. */
	int IndexOf( igdeTemplateFile *file ) const;
	
	/** \brief Project template file is present. */
	bool Has( igdeTemplateFile *file ) const;
	
	/** \brief Add project template file. */
	void Add( igdeTemplateFile *file );
	
	/** \brief Remove project template file. */
	void Remove( igdeTemplateFile *file );
	
	/** \brief Remove all project template files. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy list. */
	igdeTemplateFileList &operator=( const igdeTemplateFileList &list );
	/*@}*/
};

#endif
