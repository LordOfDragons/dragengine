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

#ifndef _FEUFONTIMPORTIMAGE_H_
#define _FEUFONTIMPORTIMAGE_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

class feFontImage;
class deImage;



/**
 * \brief Font Import Image Undo Action.
 */
class feUFontImportImage : public igdeUndo{
private:
	feFontImage *pImage;
	
	int pOldWidth;
	int pOldHeight;
	decColor *pOldColor;
	decString pOldPath;
	
	int pNewWidth;
	int pNewHeight;
	decColor *pNewColor;
	decString pNewPath;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	feUFontImportImage( feFontImage *fontImage, deImage *newImage, const char *newPath );
	
protected:
	/** \brief Clean up undo. */
	virtual ~feUFontImportImage();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pStoreColors( deImage &image, decColor *colors );
	void pRestoreColors( deImage &image, decColor *colors );
};

#endif
