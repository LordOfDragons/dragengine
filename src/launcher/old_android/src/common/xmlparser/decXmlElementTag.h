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

#ifndef _DECXMLELEMENTTAG_H_
#define _DECXMLELEMENTTAG_H_

#include "decXmlContainer.h"
#include "../string/decString.h"



/**
 * \brief XML Element Tag.
 */
class decXmlElementTag : public decXmlContainer{
private:
	decString pName;
	decString pNamespace;
	decString pLocalName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml element tag. */
	decXmlElementTag( const char *name );
	
protected:
	/** \brief Clean up xml element tag. */
	virtual ~decXmlElementTag();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Namespace part of the tag name. */
	inline const decString &GetNamespace() const{ return pNamespace; }
	
	/** \brief Local name part of the tag name. */
	inline const decString &GetLocalName() const{ return pLocalName; }
	/*@}*/
	
	
	
	/** \name Helper Functions */
	/*@{*/
	/** \brief First element tag beeing a decXmlCharacterData or NULL otherwise. */
	decXmlCharacterData *GetFirstData() const;
	
	/** \brief Index-th element tag beeing a decXmlElementTag or NULL otherwise. */
	decXmlElementTag *GetElementIfTag( int index ) const;
	
	/** \brief decXmlAttValue with the given name or NULL otherwise. */
	decXmlAttValue *FindAttribute( const char *name ) const;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	void Visit( decXmlVisitor &visitor );
	/*@}*/
	
	/** \name Casting */
	/*@{*/
	virtual bool CanCastToElementTag() const;
	virtual decXmlElementTag *CastToElementTag();
	/*@}*/
};

#endif
