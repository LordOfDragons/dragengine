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

#ifndef _DECXMLDOCUMENT_H_
#define _DECXMLDOCUMENT_H_

#include "decXmlContainer.h"
#include "../string/decString.h"


class decXmlVisitorCleanCharData;
class decXmlVisitorStripComments;


/**
 * \brief XML Document.
 * 
 * This is the root of a parsed XML file and has therefore no parent. This is a XML
 * container which allows to have only one child element. All additional elements
 * are ignored.
 */
class DE_DLL_EXPORT decXmlDocument : public decXmlContainer{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<decXmlDocument> Ref;
	
	
	
private:
	decString pEncoding;
	decString pDocType;
	decString pSysLit;
	decString pPubLit;
	bool pStandalone;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml document. */
	decXmlDocument();
	
protected:
	/** \brief Clean up xml document. */
	virtual ~decXmlDocument();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Encoding. */
	inline const decString &GetEncoding() const{ return pEncoding; }
	
	/** \brief Set encoding. */
	void SetEncoding( const char *encoding );
	
	/** \brief Doc type. */
	inline const decString &GetDocType() const{ return pDocType; }
	
	/** \brief Set doc type. */
	void SetDocType( const char *docType );
	
	/** \brief System literal. */
	inline const decString &GetSystemLiteral() const{ return pSysLit; }
	
	/** \brief Set system literal. */
	void SetSystemLiteral( const char *sysLit );
	
	/** \brief Public literal. */
	inline const decString &GetPublicLiteral() const{ return pPubLit; }
	
	/** \brief Set public literal. */
	void SetPublicLiteral( const char *sysLit );
	
	/** \brief Standalone. */
	inline bool GetStandalone() const{ return pStandalone; }
	
	/** \brief Set standalone. */
	void SetStandalone( bool standalone );
	
	/** \brief Root element. */
	decXmlElementTag *GetRoot() const;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	void Visit( decXmlVisitor &visitor );
	/*@}*/
	
	
	
	/** \name Predefined Visitors */
	/*@{*/
	/** \brief Clean character data tags by processing the html formating tags in them. */
	void CleanCharData();
	
	/** \brief Remove all comment tags. */
	void StripComments();
	/*@}*/
	
	
	
	/** \name Casting */
	/*@{*/
	virtual bool CanCastToDocument() const;
	virtual decXmlDocument *CastToDocument();
	/*@}*/
};

#endif
