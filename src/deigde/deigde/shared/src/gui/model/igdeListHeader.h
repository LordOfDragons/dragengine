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

#ifndef _IGDELISTHEADER_H_
#define _IGDELISTHEADER_H_

#include "../resources/igdeIcon.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>



/**
 * \brief IGDE UI List Header.
 * 
 * Stores information about a column header.
 */
class DE_DLL_EXPORT igdeListHeader : public deObject{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeListHeader> Ref;
	
	
private:
	decString pTitle;
	igdeIcon::Ref pIcon;
	int pSize;
	
	
	
public:
	/** \title Constructors and Destructors */
	/*@{*/
	/** \brief Create list header. */
	igdeListHeader(const char *title, int size);
	
	igdeListHeader(const char *title, igdeIcon *icon, int size);
	
	/** \brief Create copy of list header. */
	igdeListHeader(const igdeListHeader &header);
	
	
	
protected:
	/**
	 * \brief Clean up list header.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeListHeader();
	/*@}*/
	
	
	
public:
	/** \title Management */
	/*@{*/
	/** \brief Title. */
	inline const decString &GetTitle() const{ return pTitle; }
	
	/** \brief Set title. */
	void SetTitle(const char *title);
	
	/** \brief Icon or NULL. */
	inline const igdeIcon::Ref &GetIcon() const{ return pIcon; }
	
	/** \brief Set icon or NULL. */
	void SetIcon(igdeIcon *icon);
	
	/** \brief Size in pixels. */
	inline int GetSize() const{ return pSize; }
	
	/** \brief Set size in pixels. */
	void SetSize(int size);
	
	
	
	/** \brief Set from another header. */
	virtual igdeListHeader &operator=(const igdeListHeader &header);
	/*@}*/
};

#endif
