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

#ifndef _IGDETEXTSEGMENT_H_
#define _IGDETEXTSEGMENT_H_

#include "../event/igdeAction.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>


/**
 * \brief IGDE UI Text Segment.
 * 
 * Text segments define a style and optionally an action for a text segment defined
 * by a  start and end offset. The end offset is inclusive hence the length of the
 * segment is "end - start + 1". Thus text segments can not be empty.
 */
class DE_DLL_EXPORT igdeTextSegment : public deObject{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeTextSegment> Ref;
	
	
private:
	int pBegin;
	int pEnd;
	decString pStyle;
	igdeAction::Ref pAction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create text segment. */
	igdeTextSegment(int begin, int end);
	
	igdeTextSegment(int begin, int end, const char *style);
	
	igdeTextSegment(int begin, int end, const char *style, igdeAction *action);
	
	
	
protected:
	/**
	 * \brief Clean up text segment.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeTextSegment();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Offset to begin of text segment. */
	inline int GetBegin() const{ return pBegin; }
	
	/** \brief Offset to end of text segment. */
	inline int GetEnd() const{ return pEnd; }
	
	/** \brief Length of text segment. */
	inline int GetLength() const{ return pEnd - pBegin + 1; }
	
	/** \brief Style. */
	inline const decString &GetStyle() const{ return pStyle; }
	
	/** \brief Action or NULL if not used. */
	inline igdeAction *GetAction() const{ return pAction; }
	/*@}*/
};

#endif
