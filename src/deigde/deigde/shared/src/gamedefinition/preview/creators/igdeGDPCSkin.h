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

#ifndef _IGDEGDPPCSKIN_H_
#define _IGDEGDPPCSKIN_H_

#include "igdeGDPreviewCreator.h"
#include "../../../resourceloader/igdeResourceLoaderListenerReference.h"

#include <dragengine/resources/canvas/deCanvasRenderWorldReference.h>
#include <dragengine/resources/component/deComponentReference.h>
#include <dragengine/resources/model/deModelReference.h>
#include <dragengine/resources/skin/deSkinReference.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkinReference.h>
#include <dragengine/resources/camera/deCameraReference.h>
#include <dragengine/resources/world/deWorldReference.h>
#include <dragengine/resources/light/deLightReference.h>

class igdeGDSkin;
class igdeWSky;



/**
 * \brief Create preview for game definition skin asynchronously.
 */
class igdeGDPCSkin : public igdeGDPreviewCreator{
private:
	igdeGDSkin *pGDSkin;
	
	deWorldReference pWorld;
	deCameraReference pCamera;
	deLightReference pLight;
	deComponentReference pComponent;
	deModelReference pModel;
	deSkinReference pSkin;
	deDynamicSkinReference pDynamicSkin;
	deModelReference pLightBoxModel;
	deSkinReference pLightBoxSkin;
	igdeWSky *pSky;
	deCanvasRenderWorldReference pCanvasRenderWorld;
	
	igdeResourceLoaderListenerReference pResLoader;
	bool pResLoadFinished;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create preview creator. */
	igdeGDPCSkin( igdeEnvironment &environment, igdeGDSkin *gdskin, const decPoint &size );
	
	
	
protected:
	/**
	 * \brief Clean up listener.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeGDPCSkin();
	/*@}*/
	
	
	
public:
	/**  \name Management */
	/*@{*/
	/** \brief Game definition skin to create preview for. */
	inline igdeGDSkin *GetGDSkin() const{ return pGDSkin; }
	
	
	
protected:
	/** \brief Debug prefix. */
	virtual decString DebugPrefix();
	
	/** \brief Set up canvas to render. */
	virtual void PrepareCanvasForRender();
	
	/** \brief Wait for canvas to be ready for rendering. */
	virtual bool IsCanvasReadyForRender();
	
	/** \brief Update canvas to render animations. */
	virtual void UpdateCanvasForRender( float elapsed );
	/*@}*/
};

#endif
