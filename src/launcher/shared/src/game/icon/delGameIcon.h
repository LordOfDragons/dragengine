/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DELGAMEICON_H_
#define _DELGAMEICON_H_

#include <dragengine/deObject.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/string/decString.h>


/**
 * \brief Game icon.
 * 
 * Format of content depends on path extension.
 */
class delGameIcon : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delGameIcon> Ref;
	
	
	
private:
	int pSize;
	decString pPath;
	decMemoryFile::Ref pContent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create icon. */
	delGameIcon( int size, const char *path );
	
	/** \brief Clean up icon. */
	virtual ~delGameIcon();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Icon size in pixel. */
	inline const int GetSize() const{ return pSize; }
	
	/** \brief Path. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Content or nullptr if not loaded. */
	inline decMemoryFile *GetContent() const{ return pContent; }
	
	/** \brief Set content or nullptr if not loaded. */
	void SetContent( decMemoryFile *content );
	/*@}*/
	
	
	
protected:
	/**
	 * \brief Content changed due to call by SetContent().
	 * 
	 * Default implementation does nothing. For subclass use.
	 */
	virtual void OnContentChanged();
};

#endif
