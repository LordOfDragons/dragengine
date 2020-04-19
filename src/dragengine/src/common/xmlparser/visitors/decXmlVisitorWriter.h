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

#ifndef _DECXMLVISITORWRITER_H_
#define _DECXMLVISITORWRITER_H_

#include "../decXmlVisitor.h"

class decXmlWriter;
class decBaseFileWriter;


/**
 * \brief Visitor writing xml document to file using decXmlWriter.
 */
class decXmlVisitorWriter : public decXmlVisitor{
private:
	bool pCompact;
	decXmlWriter *pWriter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	decXmlVisitorWriter();
	
	/** \brief Clean up visitor. */
	virtual ~decXmlVisitorWriter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Writer or NULL. */
	inline decXmlWriter *GetWriter(){ return pWriter; }
	
	/** \brief Write compact XML for example for network transfer. */
	inline bool GetCompact() const{ return pCompact; }
	
	/** \brief Set to write compact XML for example for network transfer. */
	void SetCompact( bool compact );
	
	/** \brief Write XML document to file. */
	void WriteDocument( decBaseFileWriter *file, decXmlDocument &document );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
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
