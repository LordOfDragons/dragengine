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

#ifndef _DELFILEFORMATLIST_H_
#define _DELFILEFORMATLIST_H_

#include <dragengine/common/collection/decObjectList.h>

class delFileFormat;


/**
 * \brief File Format List.
 */
class DE_DLL_EXPORT delFileFormatList{
private:
	decObjectList pFormats;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**  \brief Create file format list. */
	delFileFormatList();
	
	/** \brief Clean up file format list. */
	~delFileFormatList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of file formats. */
	int GetCount() const;
	
	/** \brief File format at index. */
	delFileFormat *GetAt( int index ) const;
	
	/** \brief File format is present. */
	bool Has( delFileFormat *format ) const;
	
	/** \brief Index of file format or -1 if absent. */
	int IndexOf( delFileFormat *format ) const;
	
	/** \brief Add file format. */
	void Add( delFileFormat *format );
	
	/** \brief Remove file format. */
	void Remove( delFileFormat *format );
	
	/** \brief Remove all file formats. */
	void RemoveAll();
	/*@}*/
};

#endif
