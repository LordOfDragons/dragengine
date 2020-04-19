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

#ifndef _MELSWORLD_H_
#define _MELSWORLD_H_

class meWorld;
class decXmlElementTag;
class decBaseFileReader;
class decBaseFileWriter;
class meLoadSaveSystem;
class igdeStepableTask;



/**
 * Base class for loading and saving worlds.
 */
class meLSWorld{
private:
	char *pName;
	char *pPattern;
public:
	// constructor, destructor
	meLSWorld();
	virtual ~meLSWorld();
	// management
	inline const char *GetName() const{ return ( const char * )pName; }
	void SetName( const char *name );
	inline const char *GetPattern() const{ return ( const char * )pPattern; }
	void SetPattern( const char *pattern );
	// loading and saving
	virtual void SaveWorld( meLoadSaveSystem &lssys, const meWorld &world, decBaseFileWriter *file ) = 0;
	
	/** \brief Create stepable loader. */
	virtual igdeStepableTask *CreateLoadTask( meWorld *world, decBaseFileReader *file ) = 0;
	
private:
	void pLSWCleanUp();
};

// end of include only once
#endif
