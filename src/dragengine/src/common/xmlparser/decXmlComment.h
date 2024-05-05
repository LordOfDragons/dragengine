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

#ifndef _DECXMLCOMMENT_H_
#define _DECXMLCOMMENT_H_

#include "decXmlElement.h"
#include "../string/decString.h"


/**
 * \brief XML Comment.
 */
class DE_DLL_EXPORT decXmlComment : public decXmlElement{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<decXmlComment> Ref;
	
	
	
private:
	decString pComment;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml comment. */
	decXmlComment( const char *comment );
	
protected:
	/** \brief Clean up xml comment. */
	virtual ~decXmlComment();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Comment.*/
	inline const decString &GetComment() const{ return pComment; }
	
	/** \brief Set comment. */
	void SetComment( const char *comment );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	void Visit( decXmlVisitor &visitor );
	/*@}*/
	
	
	
	/** \name Casting */
	/*@{*/
	virtual bool CanCastToComment() const;
	virtual decXmlComment *CastToComment();
	/*@}*/
};

#endif
