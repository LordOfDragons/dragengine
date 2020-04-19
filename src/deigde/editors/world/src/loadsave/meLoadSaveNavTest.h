/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MELOADSAVENAVTEST_H_
#define _MELOADSAVENAVTEST_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class mePathFindTestType;
class meWorld;
class meLoadSaveSystem;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlWriter;
class decXmlElementTag;



/**
 * \brief Load/Save Navigation Test.
 */
class meLoadSaveNavTest : public igdeBaseXML{
private:
	meLoadSaveSystem *pLSSys;
	decString pName;
	decString pPattern;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new loader. */
	meLoadSaveNavTest( meLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource );
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** \brief Retrieves the pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Loads the navigation test from file. */
	void LoadNavTest( meWorld &world, decBaseFileReader &reader );
	/** \brief Saves the navigation test to file. */
	void SaveNavTest( const meWorld &world, decBaseFileWriter &writer );
	/*@}*/
	
private:
	void pWriteNavTest( decXmlWriter &writer, const meWorld &world );
	void pWriteNavTestType( decXmlWriter &writer, const mePathFindTestType &type );
	
	void pReadNavTest( const decXmlElementTag &root, meWorld &world );
	void pReadNavTestType( const decXmlElementTag &root, meWorld &world );
};

#endif
