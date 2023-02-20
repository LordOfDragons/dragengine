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

#ifndef _IGDEGDSKY_H_
#define _IGDEGDSKY_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/image/deImageReference.h>

class igdeGDSkyController;



/**
 * \brief Game sky.
 */
class DE_DLL_EXPORT igdeGDSky : public deObject{
private:
	decString pPath;
	decString pName;
	decString pDescription;
	decString pCategory;
	
	decObjectOrderedSet pControllers;
	
	deImageReference pPreviewImage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky. */
	igdeGDSky( const char *path, const char *name );
	
	/** \brief Create copy of game definition sky. */
	igdeGDSky( const igdeGDSky &sky );
	
protected:
	/** \brief Clean up sky. */
	virtual ~igdeGDSky();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Path. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set path. */
	void SetPath( const char *path );
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription( const char *description );
	
	/** \brief Category. */
	inline const decString &GetCategory() const{ return pCategory; }
	
	/** \brief Set category. */
	void SetCategory( const char *category );
	
	/** \brief Preview image or NULL if not loaded yet. */
	inline deImage *GetPreviewImage() const{ return pPreviewImage; }
	
	/** \brief Set preview image or NULL if not loaded yet. */
	void SetPreviewImage( deImage *image );
	/*@}*/
	
	
	
	/** \name Controllers */
	/*@{*/
	/** \brief Number of controllers. */
	int GetControllerCount() const;
	
	/** \brief Controller at index. */
	igdeGDSkyController *GetControllerAt( int index ) const;
	
	/** \brief Add controller. */
	void AddController( igdeGDSkyController *controller );
	
	/** \brief Remove controllers. */
	void RemoveAllControllers();
	/*@}*/
};

#endif
