/* 
 * Drag[en]gine Console Launcher
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

#ifndef _DECLBASEXML_H_
#define _DECLBASEXML_H_

#include <dragengine/common/string/decString.h>

class decXmlWriter;
class decXmlElementTag;
class deLogger;



/**
 * @brief Basic XML Load/Save Class.
 * Class used as base class for xml load/save classes. Provides a set of helper
 * function required by all xml load/save classes.
 */
class declBaseXML{
private:
	deLogger *pLogger;
	decString pLoggerSource;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new base xml read/save. */
	declBaseXML( deLogger *logger, const char *loggerSource );
	/** Cleans up the base xml read/save. */
	virtual ~declBaseXML();
	/*@}*/
	
protected:
	/** @name Management */
	/*@{*/
	/** Retrieves the logger. */
	inline deLogger *GetLogger() const{ return pLogger; }
	/** Retrieves the logger source. */
	inline const decString &GetLoggerSource() const{ return pLoggerSource; }
	
	bool pHasAttribute( const decXmlElementTag &tag, const char *name ) const;
	const char *pGetAttributeString( const decXmlElementTag &tag, const char *name ) const;
	int pGetAttributeInt( const decXmlElementTag &tag, const char *name ) const;
	float pGetAttributeFloat( const decXmlElementTag &tag, const char *name ) const;
	bool pGetAttributeBool( const decXmlElementTag &tag, const char *name ) const;
	
	int pGetCDataInt( const decXmlElementTag &tag ) const;
	float pGetCDataFloat( const decXmlElementTag &tag ) const;
	const char *pGetCDataString( const decXmlElementTag &tag ) const;
	bool pGetCDataBool( const decXmlElementTag &tag ) const;
	
	void pErrorUnknownTag( const decXmlElementTag &root, const decXmlElementTag &tag );
	void pErrorUnknownValue( const decXmlElementTag &tag, const char *value );
	void pErrorMissingTag( const decXmlElementTag &root, const char *tagName );
	/*@}*/
};

#endif
