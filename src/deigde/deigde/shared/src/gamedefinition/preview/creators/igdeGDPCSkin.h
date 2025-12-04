/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _IGDEGDPPCSKIN_H_
#define _IGDEGDPPCSKIN_H_

#include "igdeGDPreviewCreator.h"
#include "../../../resourceloader/igdeResourceLoaderListener.h"

#include <dragengine/resources/canvas/deCanvasRenderWorldReference.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModelReference.h>
#include <dragengine/resources/skin/deSkinReference.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/light/deLight.h>

class igdeGDSkin;
class igdeWSky;



/**
 * \brief Create preview for game definition skin asynchronously.
 */
class DE_DLL_EXPORT igdeGDPCSkin : public igdeGDPreviewCreator{
private:
	igdeGDSkin *pGDSkin;
	
	deWorld::Ref pWorld;
	deCamera::Ref pCamera;
	deLight::Ref pLight;
	deComponent::Ref pComponent;
	deModelReference pModel;
	deSkinReference pSkin;
	deDynamicSkin::Ref pDynamicSkin;
	deModelReference pLightBoxModel;
	deSkinReference pLightBoxSkin;
	igdeWSky *pSky;
	deCanvasRenderWorldReference pCanvasRenderWorld;
	
	igdeResourceLoaderListener::Ref pResLoader;
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
