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

#ifndef _DECXMLATTVALUE_H_
#define _DECXMLATTVALUE_H_

#include "decXmlElement.h"
#include "../string/decString.h"


/**
 * \brief XML Attribute Value.
 */
class DE_DLL_EXPORT decXmlAttValue : public decXmlElement{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<decXmlAttValue> Ref;
	
	
private:
	decString pName;
	decString pValue;
	decString pNamespace;
	decString pLocalName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml attribute value. */
	decXmlAttValue(const char *name);
	
protected:
	/** \brief Clean up xml attribute value. */
	~decXmlAttValue();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{return pName;}
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Value. */
	inline const decString &GetValue() const{return pValue;}
	
	/** \brief Set value. */
	void SetValue(const char *value);
	
	/** \brief Namespace part of name. */
	inline const decString &GetNamespace() const{return pNamespace;}
	
	/** \brief Local name part of name. */
	inline const decString &GetLocalName() const{return pLocalName;}
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	void Visit(decXmlVisitor &visitor);
	/*@}*/
	
	
	
	/** \name Casting */
	/*@{*/
	virtual bool CanCastToAttValue() const;
	virtual decXmlAttValue *CastToAttValue();
	/*@}*/
};

#endif
