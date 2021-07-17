/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DELBASEXML_H_
#define _DELBASEXML_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/logger/deLogger.h>

class decXmlWriter;
class decXmlElementTag;



/**
 * \brief Basic XML Load/Save Class.
 */
class delBaseXML{
private:
	deLogger::Ref pLogger;
	decString pLoggerSource;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create base xml read/save. */
	delBaseXML( deLogger *logger, const char *loggerSource );
	
	/** \brief Clean up base xml read/save. */
	virtual ~delBaseXML();
	/*@}*/
	
	
	
protected:
	/** \name Management */
	/*@{*/
	/** \brief Logger. */
	inline deLogger *GetLogger() const{ return pLogger; }
	
	/** \brief Logger source. */
	inline const decString &GetLoggerSource() const{ return pLoggerSource; }
	
	/** \brief Named attribute is present. */
	bool HasAttribute( const decXmlElementTag &tag, const char *name ) const;
	
	/**
	 * \brief Named attribute value.
	 * \throws EInvalidParam Named attribute is absent.
	 */
	const decString &GetAttributeString( const decXmlElementTag &tag, const char *name ) const;
	
	/**
	 * \brief Named attribute value.
	 * \throws EInvalidParam Named attribute is absent.
	 */
	int GetAttributeInt( const decXmlElementTag &tag, const char *name ) const;
	
	/**
	 * \brief Named attribute value.
	 * \throws EInvalidParam Named attribute is absent.
	 */
	float GetAttributeFloat( const decXmlElementTag &tag, const char *name ) const;
	
	/** \brief First CDATA value or 0 if absent. */
	int GetCDataInt( const decXmlElementTag &tag ) const;
	
	/** \brief First CDATA value or 0 if absent. */
	float GetCDataFloat( const decXmlElementTag &tag ) const;
	
	/** \brief First CDATA value or empty string if absent. */
	const decString &GetCDataString( const decXmlElementTag &tag ) const;
	
	/**
	 * \brief First CDATA value.
	 * \throws EInvalidParam CDATA absent.
	 */
	bool GetCDataBool( const decXmlElementTag &tag ) const;
	
	/** \brief Log error unknown tag. */
	void ErrorUnknownTag( const decXmlElementTag &root, const decXmlElementTag &tag );
	
	/** \brief Log error unknown value. */
	void ErrorUnknownValue( const decXmlElementTag &tag, const char *value );
	
	/** \brief Log error missing tag. */
	void ErrorMissingTag( const decXmlElementTag &root, const char *tagName );
	/*@}*/
};

#endif
