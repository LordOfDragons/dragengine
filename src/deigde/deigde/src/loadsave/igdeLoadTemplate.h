/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDELOADTEMPLATE_H_
#define _IGDELOADTEMPLATE_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/string/decString.h>

class igdeTemplate;
class igdeTemplateFile;
class igdeTemplateReplace;

class decBaseFileReader;


/**
 * \brief Load project template.
 */
class igdeLoadTemplate : public igdeBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load template. */
	igdeLoadTemplate( deLogger *logger );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load template using file reader. */
	void Load( decBaseFileReader &reader, igdeTemplate &atemplate );
	/*@}*/
	
	
	
private:
	void pReadTemplate( const decXmlElementTag &root, igdeTemplate &atemplate );
	void pReadFile( const decXmlElementTag &root, igdeTemplateFile &file );
	void pReadReplace( const decXmlElementTag &root, igdeTemplateReplace &replace );
};

#endif
