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

#ifndef _IGDEGDPREVIEWCREATOR_H_
#define _IGDEGDPREVIEWCREATOR_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/capture/deCaptureCanvas.h>
#include <dragengine/resources/image/deImage.h>

class igdeGDPreviewListener;
class igdeEnvironment;


/**
 * \brief Base class for creating previews for game definition element asynchronously.
 */
class DE_DLL_EXPORT igdeGDPreviewCreator : public deObject{
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
	
	deCaptureCanvas::Ref pCaptureCanvas;
	deCanvasView::Ref pCanvas;
	
	eStates pState;
	deImage::Ref pImage;
	decObjectOrderedSet pListeners;
	bool pEnableDebug;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeGDPreviewCreator> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create preview creator. */
	igdeGDPreviewCreator(igdeEnvironment &environment, const decPoint &size);
	
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
	inline const deCanvasView::Ref &GetCanvas() const{ return pCanvas; }
	
	
	
	/** \brief Image or NULL if not created. */
	inline const deImage::Ref &GetImage() const{ return pImage; }
	
	/** \brief Set image or NULL if not created. */
	void SetImage(deImage *image);
	
	
	
	/** \brief Add listener. */
	void AddListener(igdeGDPreviewListener *listener);
	
	
	
	/** \brief Update preview with elapsed time to create a new frame. */
	void Update(float elapsed);
	
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
	void NotifyImageCreated(deImage *image);
	
	/** \brief Debug prefix. */
	virtual decString DebugPrefix() = 0;
	
	/** \brief Set up canvas to render. */
	virtual void PrepareCanvasForRender() = 0;
	
	/** \brief Wait for canvas to be ready for rendering. */
	virtual bool IsCanvasReadyForRender() = 0;
	
	/** \brief Update canvas to render animations. */
	virtual void UpdateCanvasForRender(float elapsed) = 0;
	
	/** \brief Debug log if enabled. */
	void DebugLog(const char *message);
	/*@}*/
	
	
	
private:
	void pUpdate();
};

#endif
