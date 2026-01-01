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

#ifndef _SECLIPBOARDDATAMAPPED_H_
#define _SECLIPBOARDDATAMAPPED_H_

#include "../skin/mapped/seMapped.h"

#include <deigde/clipboard/igdeClipboardData.h>


/** Clipboard data mapped. */
class seClipboardDataMapped : public igdeClipboardData{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<seClipboardDataMapped> Ref;
	
	/** Type name. */
	static const char * const TYPE_NAME;
	
	
	
private:
	seMapped::List pMapped;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create clipboard data from selected mapped. */
	explicit seClipboardDataMapped(const seMapped::List &list);
	
protected:
	/**
	 * Clean up object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~seClipboardDataMapped();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Mapped list. */
	inline const seMapped::List &GetMapped() const{ return pMapped; }
	/*@}*/
};

#endif
