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

#ifndef _IGDEUNDO_H_
#define _IGDEUNDO_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Undo action.
 * 
 * Supports Undo/Redo operations.
 */
class DE_DLL_EXPORT igdeUndo : public deObject{
private:
	decString pShortInfo;
	decString pLongInfo;
	
	int pMemoryConsumption;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	igdeUndo();
	
	
	
protected:
	/**
	 * \brief Clean up unfo.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeUndo();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Short info. */
	inline const decString &GetShortInfo() const{ return pShortInfo; }
	
	/** \brief Set short info. */
	void SetShortInfo( const char *info );
	
	/** \brief Long info. */
	inline const decString &GetLongInfo() const{ return pLongInfo; }
	
	/** \brief Set long info. */
	void SetLongInfo( const char *info );
	
	/** \brief Memory consumption. */
	inline int GetMemoryConsumption() const{ return pMemoryConsumption; }
	
	/** \brief Set memory consumption. */
	void SetMemoryConsumption( int bytes );
	
	
	
	/** \brief Undo. */
	virtual void Undo() = 0;
	
	/** \brief Redo. */
	virtual void Redo() = 0;
	/*@}*/
};

#endif
