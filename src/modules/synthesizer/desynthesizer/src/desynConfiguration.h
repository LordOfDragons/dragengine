/* 
 * Drag[en]gine Synthesizer Module
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

#ifndef _DESYNCONFIGURATION_H_
#define _DESYNCONFIGURATION_H_

#include <dragengine/common/string/decString.h>

class decXmlAttValue;
class decXmlElementTag;
class decBaseFileReader;
class deDESynthesizer;



/**
 * \brief DESynthesizer configuration.
 */
class desynConfiguration{
private:
	deDESynthesizer &pModule;
	
	int pStreamBufSizeThreshold;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create configuration. */
	desynConfiguration( deDESynthesizer &module );
	
	/** \brief Clean up configuration. */
	~desynConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Buffer size threshold to stream sound samples. */
	inline int GetStreamBufSizeThreshold() const{ return pStreamBufSizeThreshold; }
	
	/** \brief Set buffer size threshold to stream sound samples. */
	void SetStreamBufSizeThreshold( int threshold );
	
	
	
	/** \brief Load configuration. */
	void LoadConfig();
	
	/** \brief Save configuration. */
	void SaveConfig();
	
	
	
private:
	void pCleanUp();
	void pLoadConfig( decBaseFileReader *file );
	decXmlElementTag *pGetTagAt( const decXmlElementTag &tag, int index );
	decXmlAttValue *pFindAttribute( const decXmlElementTag &tag, const char *name );
	const char *pGetAttributeString( const decXmlElementTag &tag, const char *name );
	int pGetAttributeInt( const decXmlElementTag &tag, const char *name );
	float pGetAttributeFloat( const decXmlElementTag &tag, const char *name );
	const char *pGetCData( const decXmlElementTag &tag, const char *defaultValue );
	bool pGetCDataBool( const decXmlElementTag &tag, bool defaultValue );
	int pGetCDataInt( const decXmlElementTag &tag, int defaultValue );
};

#endif
