/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
class decXmlDocument : public decXmlContainer{
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
