#if 0
/* 
 * Drag[en]gine Library -- Game Engine
 *
 * Copyright (C) 2010, Plüss Roland ( roland@rptd.ch )
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

// include only once
#ifndef _DETHOGGFILE_H_
#define _DETHOGGFILE_H_

// includes
#include <theora/codec.h>
#include <theora/theoradec.h>

#include <dragengine/deObject.h>

// predefinitions
class dethOggStream;
class dethOggReader;



/**
 * @brief Ogg File.
 */
class dethOggFile : public deObject{
private:
	th_info pInfo;
	th_comment pComment;
	th_setup_info *pSetupInfo;
	
	dethOggStream **pStreams;
	int pStreamCount;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/**
	 * Creates a new ogg file. Reads the headers to build a stream object for each stream
	 * contained in the ogg file. Reads also over all pages without processing them to
	 * determine the play time as well as building the seek map.
	 */
	dethOggFile( dethOggReader &reader );
	/** Cleans up the ogg file. */
	virtual ~dethOggFile();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/*@}*/
};

// end of include only once
#endif

#endif
