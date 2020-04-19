/* 
 * Drag[en]gine IGDE Font Editor
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
#ifndef _RECLIPBOARD_H_
#define _RECLIPBOARD_H_

// predefinitions
class feBaseClip;



/**
 * @brief Clipboard.
 * Manages a clipboard holding data copied by the user. The data is stored
 * as a clip object which can be subclasses. At most one clip object can
 * be in the clipboard at every time.
 */
class feClipboard{
private:
	feBaseClip *pClip;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new clipboard. */
	feClipboard();
	/** Cleans up the clipboatd. */
	~feClipboard();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Determines if a clip is held by the clipboatd. */
	bool HasClip() const;
	/** Retrieves the clip or NULL if not set. */
	inline feBaseClip *GetClip() const{ return pClip; }
	/** Sets the held clip replacing the old one if existing. */
	void SetClip( feBaseClip *clip );
	/*@}*/
};

// end of include only once
#endif
