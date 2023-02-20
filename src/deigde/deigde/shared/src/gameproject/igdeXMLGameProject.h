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

#ifndef _IGDEXMLGAMEPROJECT_H_
#define _IGDEXMLGAMEPROJECT_H_

#include "../utils/igdeBaseXML.h"

class igdeGameProject;

class deLogger;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlElementTag;
class decXmlWriter;



/**
 * \brief Load and save XML game project files.
 */
class DE_DLL_EXPORT igdeXMLGameProject : public igdeBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game project xml object. */
	igdeXMLGameProject( deLogger *logger );
	
	/** \brief Clean up game project xml object. */
	virtual ~igdeXMLGameProject();
	/*@}*/
	
	
	
	/** \name Loading and Saving */
	/*@{*/
	/** \brief Load a game project from. */
	void Load( decBaseFileReader &reader, igdeGameProject &project );
	
	/** \brief Saves the given game definition. */
	void Save( decBaseFileWriter &writer, const igdeGameProject &project );
	/*@}*/
	
	
	
private:
	void pReadProject( const decXmlElementTag &root, igdeGameProject &project );
	
	void pWriteProject( decXmlWriter &writer, const igdeGameProject &project );
};

#endif
