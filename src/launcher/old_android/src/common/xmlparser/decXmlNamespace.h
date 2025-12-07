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

#ifndef _DECXMLNAMESPACE_H_
#define _DECXMLNAMESPACE_H_

#include "decXmlElement.h"
#include "../string/decString.h"



/**
 * \brief XML Namespace.
 */
class decXmlNamespace : public decXmlElement{
private:
	decString pName;
	decString pURL;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml namespace with the given name and url. */
	decXmlNamespace(const char *name, const char *url);
	
	/** \brief Create xml namespace with the given name and an empty url. */
	decXmlNamespace(const char *name);
	
protected:
	/** \brief Clean up xml namespace. */
	virtual ~decXmlNamespace();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{return pName;}
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief URL. */
	inline const decString &GetURL() const{return pURL;}
	
	/** \brief Set URL. */
	void SetURL(const char *url);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	void Visit(decXmlVisitor &visitor);
	/*@}*/
	
	/** \name Casting */
	/*@{*/
	virtual bool CanCastToNamespace() const;
	virtual decXmlNamespace *CastToNamespace();
	/*@}*/
};

#endif
