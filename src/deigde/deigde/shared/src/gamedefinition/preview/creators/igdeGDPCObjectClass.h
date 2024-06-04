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

#ifndef _IGDEGDPPCOBJECTCLASS_H_
#define _IGDEGDPPCOBJECTCLASS_H_

#include "igdeGDPreviewCreator.h"
#include "../../../gui/wrapper/igdeWObject.h"

#include <dragengine/resources/canvas/deCanvasRenderWorldReference.h>
#include <dragengine/resources/camera/deCameraReference.h>
#include <dragengine/resources/world/deWorldReference.h>
#include <dragengine/resources/light/deLightReference.h>

class igdeGDClass;
class igdeWSky;



/**
 * \brief Create preview for game definition object class asynchronously.
 */
class DE_DLL_EXPORT igdeGDPCObjectClass : public igdeGDPreviewCreator{
private:
	class DE_DLL_EXPORT cAsyncFinished : public igdeWObject::cAsyncLoadFinished {
	public:
		bool asyncLoadFinished;
	public:
		cAsyncFinished();
		virtual void LoadFinished( igdeWObject &wrapper, bool succeeded );
	};
	
	igdeGDClass *pGDClass;
	
	deWorldReference pWorld;
	deCameraReference pCamera;
	deLightReference pLight;
	igdeWObject *pObject;
	igdeWSky *pSky;
	igdeWObject *pEnvObject;
	deCanvasRenderWorldReference pCanvasRenderWorld;
	
	cAsyncFinished pAsyncFinished;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create preview creator. */
	igdeGDPCObjectClass( igdeEnvironment &environment, igdeGDClass *gdclass, const decPoint &size );
	
	
	
protected:
	/**
	 * \brief Clean up listener.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeGDPCObjectClass();
	/*@}*/
	
	
	
public:
	/**  \name Management */
	/*@{*/
	/** \brief Game definition object class to create preview for. */
	inline igdeGDClass *GetGDClass() const{ return pGDClass; }
	
	
	
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
