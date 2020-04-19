/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALREMOVEDIRTREE_H_
#define _DEALREMOVEDIRTREE_H_

class deLogger;
class deVirtualFileSystem;



/**
 * \brief Remove directory tree.
 */
class dealRemoveDirTree{
private:
	deLogger &pLogger;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create remove directory tree. */
	dealRemoveDirTree( deLogger &logger );
	
	/** \brief Clean up remove directory tree. */
	~dealRemoveDirTree();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Remove directory tree. */
	void Remove( const char *path );
	/*@}*/
	
private:
	void pRemove( deVirtualFileSystem &vfs, const decPath &deletePath );
};

#endif
