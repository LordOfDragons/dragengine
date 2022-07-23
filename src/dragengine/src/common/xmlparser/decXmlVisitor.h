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

#ifndef _DECXMLVISITOR_H_
#define _DECXMLVISITOR_H_

#include "../../dragengine_export.h"

class decXmlAttValue;
class decXmlCDSect;
class decXmlCharacterData;
class decXmlCharReference;
class decXmlComment;
class decXmlContainer;
class decXmlDocument;
class decXmlElement;
class decXmlElementTag;
class decXmlEntityReference;
class decXmlNamespace;
class decXmlPI;


/**
 * \brief XML element visitor.
 */
class DE_DLL_EXPORT decXmlVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml element visitor. */
	decXmlVisitor();
	
	/** \brief Clean up xml element visitor. */
	virtual ~decXmlVisitor();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit element. */
	virtual void VisitElement( decXmlElement &element );
	
	/** \brief Visit container. */
	virtual void VisitContainer( decXmlContainer &container );
	
	/** \brief Visit document. */
	virtual void VisitDocument( decXmlDocument &document );
	
	/** \brief Visit comment. */
	virtual void VisitComment( decXmlComment &comment );
	
	/** \brief Visit processing instructions. */
	virtual void VisitPI( decXmlPI &pi );
	
	/** \brief Visit element tag. */
	virtual void VisitElementTag( decXmlElementTag &tag );
	
	/** \brief Visit element character data. */
	virtual void VisitCharacterData( decXmlCharacterData &data );
	
	/** \brief Visit element entity reference. */
	virtual void VisitEntityReference( decXmlEntityReference &ref );
	
	/** \brief Visit character reference. */
	virtual void VisitCharReference( decXmlCharReference &ref );
	
	/** \brief Visit character data section. */
	virtual void VisitCDSect( decXmlCDSect &cdsect );
	
	/** \brief Visit attribute value. */
	virtual void VisitAttValue( decXmlAttValue &value );
	
	/** \brief Visit namespace. */
	virtual void VisitNamespace( decXmlNamespace &ns );
	/*@}*/
};

#endif
