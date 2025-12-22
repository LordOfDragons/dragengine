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

#ifndef _IGDECLIPBOARD_H_
#define _IGDECLIPBOARD_H_

#include <dragengine/common/collection/decTList.h>

class igdeClipboardData;


/**
 * \brief Clipboard holding data copied by the user
 * 
 * Data is stored as clipboard data objects each having a clipboard type name
 * indicating what kind of data it contains. Multiple clipboard data objects
 * can be stored as long as they have unique clipboard type names. The
 * application uses the clipboard type name to locate data it can paste.
 */
class DE_DLL_EXPORT igdeClipboard {
public:
	typedef decTObjectList<igdeClipboardData> DataList;
	
	
private:
	DataList pData;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create clipboard. */
	igdeClipboard();
	
	/** \brief Clean up clipboard. */
	~igdeClipboard();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Data list. */
	inline const DataList &GetData() const{ return pData; }
	
	/** \brief Data matching type name is present. */
	bool HasWithTypeName(const char *typeName) const;
	
	/** \brief Data matching type name or nullptr if absent. */
	igdeClipboardData *GetWithTypeName(const char *typeName) const;
	
	/** \brief Set data replacing data with the same type name if present. */
	void Set(igdeClipboardData *data);
	
	/** \brief Remove data matching type name if present. */
	void Clear(const char *typeName);
	
	/** \brief Clear clipboard. */
	void ClearAll();
	/*@}*/
};

#endif
