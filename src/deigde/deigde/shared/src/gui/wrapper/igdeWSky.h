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

#ifndef _IGDEWSKY_H_
#define _IGDEWSKY_H_

#include "../../gamedefinition/sky/igdeGDSkyReference.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/sky/deSkyInstanceReference.h>
#include <dragengine/resources/world/deWorldReference.h>

class igdeEnvironment;

class deSky;
class deSkyController;


/**
 * \brief Sky wrapper.
 * 
 * Provides a simple way to display a sky from game definition or a custom sky.
 * The sky can be modified by changing controller values.
 */
class igdeWSky{
public:
	/** \brief Asynchronous loading finished. */
	class cAsyncLoadFinished{
	public:
		/** \brief Create listener. */
		cAsyncLoadFinished();
		
		/** \brief Clean up listener. */
		virtual ~cAsyncLoadFinished();
		
		/** \brief Loading finished. */
		virtual void LoadFinished( igdeWSky &wrapper, bool succeeded ) = 0;
	};
	
	
	
private:
	igdeEnvironment &pEnvironment;
	
	deWorldReference pEngWorld;
	deSkyInstanceReference pEngSkyInstance;
	float pMaxLightIntensity;
	igdeGDSkyReference pGDSky;
	decString pPath;
	
	cAsyncLoadFinished *pAsyncLoadFinished;
	int pAsyncLoadCounter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky wrapper. */
	igdeWSky( igdeEnvironment &environment );
	
	/** \brief Clean up wrapper. */
	~igdeWSky();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief World or NULL. */
	inline deWorld *GetWorld() const{ return pEngWorld; }
	
	/** \brief Set world or NULL. */
	void SetWorld( deWorld *world );
	
	/** \brief Maximum light intensity found in the sky. */
	inline float GetMaxLightIntensity() const{ return pMaxLightIntensity; }
	
	/** \brief Get sky instance controller count. */
	int GetControllerCount() const;
	
	/** \brief Get sky instance controller. */
	const deSkyController &GetControllerAt( int index ) const;
	
	/** \brief Set controller value. */
	void SetControllerValue( int index, float value );
	
	/** \brief Get sky or NULL. */
	const deSky *GetSky() const;
	
	/** \brief Game definition sky or NULL if sky is set manually. */
	inline igdeGDSky *GetGDSky() const{ return pGDSky; }
	
	/** \brief Sky path or NULL if sky is set manually. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set sky to use. */
	void SetSky( deSky *sky );
	
	/** \brief Set sky from default sky game definition if present. */
	void SetGDDefaultSky();
	
	/** \brief Set sky from game definition sky. */
	void SetGDSky( igdeGDSky *gdSky );
	
	/**
	 * \brief Set sky from path if present.
	 * 
	 * If path exists in game definition stores also the game definition.
	 */
	void SetPath( const char *path );
	
	
	
	/** \brief Asynchronous load finished listener or NULL. */
	inline cAsyncLoadFinished *GetAsyncLoadFinished() const{ return pAsyncLoadFinished; }
	
	/**
	 * \brief Set asynchronous load finished listener or NULL.
	 * 
	 * Caller is responsible to keep the listener alive. Wrapper does not hold reference.
	 */
	void SetAsyncLoadFinished( cAsyncLoadFinished *listener );
	
	
	
	/**
	 * \brief Project game definition changed.
	 * 
	 * Call this method if you received an OnGameDefinitionChanged() on the parent module.
	 */
	void OnGameDefinitionChanged();
	/*@}*/
	
	
	
private:
	void pLoadSky( const char *path );
	void pCheckAsyncLoadFinished();
};

#endif
