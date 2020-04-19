/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CESPEECHANIMATIONXML_H_
#define _CESPEECHANIMATIONXML_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/string/decString.h>

class ceSpeechAnimation;
class ceSAPhoneme;
class ceSAWord;

class decBaseFileReader;
class decXmlElementTag;



/**
 * \brief Load Speech Animation XML.
 */
class ceSpeechAnimationXML : public igdeBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new loader. */
	ceSpeechAnimationXML( deLogger *logger, const char *loggerSource );
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Reads the speech animation from file. */
	void ReadFromFile( const decString &pathSAnimation, decBaseFileReader &reader, ceSpeechAnimation &sanimation );
	/*@}*/
	
private:
	void pReadSAnimation( const decXmlElementTag &root, const char *basePath, ceSpeechAnimation &sanimation );
	void pReadPhoneme( const decXmlElementTag &root, ceSpeechAnimation &sanimation );
	void pReadWord( const decXmlElementTag &root, ceSpeechAnimation &sanimation );
};

#endif
