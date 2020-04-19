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

#ifndef _IGDEGDPREVIEWCREATOR_H_
#define _IGDEGDPREVIEWCREATOR_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/canvas/deCanvasViewReference.h>
#include <dragengine/resources/canvas/capture/deCaptureCanvasReference.h>
#include <dragengine/resources/image/deImageReference.h>

class igdeGDPreviewListener;
class igdeEnvironment;


/**
 * \brief Base class for creating previews for game definition element asynchronously.
 */
class igdeGDPreviewCreator : public deObject{
private:
	enum eStates{
		esInitial,
		esPrepareCanvas,
		esWaitCanvasReady,
		esFrameUpdate,
		esBeginCapture,
		esWaitCaptureFinished,
		esCaptureFinished
	};
	
	igdeEnvironment &pEnvironment;
	
	deCaptureCanvasReference pCaptureCanvas;
	deCanvasViewReference pCanvas;
	
	eStates pState;
	deImageReference pImage;
	decObjectOrderedSet pListeners;
	bool pEnableDebug;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create preview creator. */
	igdeGDPreviewCreator( igdeEnvironment &environment, const decPoint &size );
	
protected:
	/**
	 * \brief Clean up listener.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeGDPreviewCreator();
	/*@}*/
	
	
	
public:
	/**  \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment(){ return pEnvironment; }
	inline const igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Canvas to create image from. */
	inline deCanvasView *GetCanvas() const{ return pCanvas; }
	
	
	
	/** \brief Image or NULL if not created. */
	inline deImage *GetImage() const{ return pImage; }
	
	/** \brief Set image or NULL if not created. */
	void SetImage( deImage *image );
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeGDPreviewListener *listener );
	
	
	
	/** \brief Update preview with elapsed time to create a new frame. */
	void Update( float elapsed );
	
	/**
	 * \brief Begin creating preview.
	 */
	void BeginCreation();
	
	/**
	 * \brief Finish preview creation.
	 * \returns \em true if creation finished or \em false if still running.
	 */
	bool FinishCreation();
	
	/** \brief Abort preview creation. */
	void AbortCreation();
	
	
	
protected:
	/** \brief Notify listeners image has been created. */
	void NotifyImageCreated( deImage *image );
	
	/** \brief Debug prefix. */
	virtual decString DebugPrefix() = 0;
	
	/** \brief Set up canvas to render. */
	virtual void PrepareCanvasForRender() = 0;
	
	/** \brief Wait for canvas to be ready for rendering. */
	virtual bool IsCanvasReadyForRender() = 0;
	
	/** \brief Update canvas to render animations. */
	virtual void UpdateCanvasForRender( float elapsed ) = 0;
	
	/** \brief Debug log if enabled. */
	void DebugLog( const char *message );
	/*@}*/
	
	
	
private:
	void pUpdate();
};

#endif
