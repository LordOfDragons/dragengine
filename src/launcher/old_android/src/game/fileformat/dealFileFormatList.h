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

#ifndef _DEALFILEFORMATLIST_H_
#define _DEALFILEFORMATLIST_H_

#include "../../common/collection/decObjectList.h"

class dealFileFormat;



/**
 * \brief File Format List.
 */
class dealFileFormatList{
private:
	decObjectList pFormats;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file format list. */
	dealFileFormatList();
	
	/** \brief Clean up file format list. */
	~dealFileFormatList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of file formats. */
	int GetFormatCount() const;
	
	/** \brief File format at the given position. */
	dealFileFormat *GetFormatAt(int index) const;
	
	/** \brief File format exists. */
	bool HasFormat(dealFileFormat *format) const;
	
	/** \brief Index of a file format or -1 if not found. */
	int IndexOfFormat(dealFileFormat *format) const;
	
	/** \brief Add file format. */
	void AddFormat(dealFileFormat *format);
	
	/** \brief Remove file format. */
	void RemoveFormat(dealFileFormat *format);
	
	/** \brief Remove all file formats. */
	void RemoveAllFormats();
	/*@}*/
};

#endif
