/* 
 * Drag[en]gine IGDE Rig Editor
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

// include only once
#ifndef _REBASECLIP_H_
#define _REBASECLIP_H_



/**
 * @brief Base Clipboard Clip.
 * Base class for clips to be held by a clipboard object.
 */
class reBaseClip{
public:
	enum eClipTypes{
		ectObject
	};
	
private:
	int pType;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new base clipboard clip. */
	reBaseClip( int type );
	/** Cleans up the clipboard clip. */
	virtual ~reBaseClip();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the type of the clip. */
	inline int GetType() const{ return pType; }
	/*@}*/
};

// end of include only once
#endif
