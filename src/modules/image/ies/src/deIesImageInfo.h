/* 
 * Drag[en]gine IES Photometric Image Module
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

#ifndef _DEIESIMAGEINFO_H_
#define _DEIESIMAGEINFO_H_

#include <dragengine/common/string/decStringList.h>
#include <dragengine/systems/modules/image/deBaseImageInfo.h>

class decBaseFileReader;


/**
 * \brief Image information class.
 */
class deIesImageInfo : public deBaseImageInfo{
protected:
	int pWidth;
	int pHeight;
	int pDepth;
	decStringList pLines;
	int pCurLine;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create image information. */
	deIesImageInfo();
	
	/** \brief Clean up image information. */
	virtual ~deIesImageInfo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the width in pixels. */
	virtual int GetWidth();
	
	/** \brief Retrieves the height in pixels. */
	virtual int GetHeight();
	
	/** \brief Retrieves the depth in pixels. */
	virtual int GetDepth();
	
	/** \brief Retrieves the number of components. */
	virtual int GetComponentCount();
	
	/** \brief Retrieves the bit count of each component. */
	virtual int GetBitCount();
	
	/** \brief Load header. */
	virtual bool LoadHeader( decBaseFileReader &reader ) = 0;
	
	/** \brief Load file. */
	virtual void LoadFile( unsigned short *pixels ) = 0;
	/*@}*/
	
	
	
protected:
	void pReadLines( decBaseFileReader &reader );
};

#endif
