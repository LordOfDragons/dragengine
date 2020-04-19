/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLDELAYEDOPERATIONS_H_
#define _DEOGLDELAYEDOPERATIONS_H_

#include <dragengine/common/collection/decPointerOrderedSet.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/threading/deMutex.h>

class deoglRCamera;
class deoglRFont;
class deoglRImage;
class deoglRModel;
class deoglRenderThread;
class deoglRSkin;
class deoglShaderProgram;
class deoglSkinTexture;
class deoglDelayedFileWrite;
class deoglDelayedSaveImage;
class deoglDelayedDeletion;



/**
 * \brief Delayed operations.
 * 
 * Resources can be created at any time including asynchronously. OpenGL doesn't allow or
 * reacts badly to creating or manipulating OpenGL related objects outside a well defined
 * render loop. To prevent problems these operations have to be delayed until a time when
 * it is safe to carry them out. Such delayed operations are collected in a single instance
 * of this class and carried out either at the beginning or after the end of a frame update.
 * It is garanteed that all pending delayed operations are carried out during this time.
 */
class deoglDelayedOperations{
private:
	deoglRenderThread &pRenderThread;
	
	deMutex pMutexAsyncResInit;
	bool pHasAsyncResInitOperations;
	decPointerOrderedSet pAsyncResInitSkinList;
	decPointerOrderedSet pAsyncResInitFontList;
	
	deMutex pMutexInit;
	bool pHasInitOperations;
	decPointerOrderedSet pInitImageList;
	decPointerOrderedSet pInitSkinList;
	decPointerOrderedSet pInitModelList;
	
	deMutex pMutexFree;
	bool pHasFreeOperations;
	deoglDelayedDeletion *pRootDeletion;
	deoglDelayedDeletion *pTailDeletion;
	int pDeletionCount;
	
	deMutex pMutexSynchronize;
	bool pHasSynchronizeOperations;
	decPointerOrderedSet pFileWriteList;
	decPointerOrderedSet pSaveImageList;
	decObjectOrderedSet pCleanUpCameraList;
	
	deoglShaderProgram *pShaderGenConeMap;
	deoglShaderProgram *pShaderGenConeMapLayer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create delayed operations object. */
	deoglDelayedOperations( deoglRenderThread &renderThread );
	
	/** \brief Clean up delayed operations object. */
	~deoglDelayedOperations();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief Mutex. */
	inline deMutex &GetMutex(){ return pMutexInit; }
	/*@}*/
	
	
	
	/** \name Delayed main thread asynchronous resources initialization. */
	/*@{*/
	/** \brief Has asynchronous resource init operations (not thread-safe). */
	inline bool GetHasAsyncResInitOperations() const{ return pHasAsyncResInitOperations; }
	
	/**
	 * \brief Process asynchronous resource init operations (thread-safe).
	 * \details Called from main thread to finalize initialize asynchronously loaded resources.
	 *          This is required to deal with accessing render objects in a safe way.
	 *          Operates on the following lists:
	 *          - Render Skins (deoglRSkin)
	 *          - Render Fonts (deoglRFont)
	 */
	void ProcessAsyncResInitOperations();
	
	
	
	/** \brief Async res init skin list (not thread-safe). */
	inline const decPointerOrderedSet &GetAsyncResInitSkinList() const{ return pAsyncResInitSkinList; }
	
	/** \brief Add skin to async res initialize if not existing already (thread-safe). */
	void AddAsyncResInitSkin( deoglRSkin *skin );
	
	/** \brief Remove skin to async res initialize if existing (thread-safe). */
	void RemoveAsyncResInitSkin( deoglRSkin *skin );
	
	
	
	/** \brief Async res init font list (not thread-safe). */
	inline const decPointerOrderedSet &GetAsyncResInitFontList() const{ return pAsyncResInitFontList; }
	
	/** \brief Add font to async res initialize if not existing already (thread-safe). */
	void AddAsyncResInitFont( deoglRFont *font );
	
	/** \brief Remove font to async res initialize if existing (thread-safe). */
	void RemoveAsyncResInitFont( deoglRFont *font );
	/*@}*/
	
	
	
	/** \name Delayed render thread OpengGL initialization */
	/*@{*/
	/** \brief Has init operations (not thread-safe). */
	inline bool GetHasInitOperations() const{ return pHasInitOperations; }
	
	/**
	 * \brief Process init operations (thread-safe).
	 * \details Called from render thread to initialize OpenGL related objects.
	 *          Operates on the following lists:
	 *          - Render Images (deoglRImage)
	 *          - Render Models (deoglRModel)
	 *          - Render Skins (deoglRSkin)
	 */
	void ProcessInitOperations();
	
	
	
	/** \brief Init image list (not thread-safe). */
	inline const decPointerOrderedSet &GetInitImageList() const{ return pInitImageList; }
	
	/** \brief Add image to initialize if not existing already (thread-safe). */
	void AddInitImage( deoglRImage *image );
	
	/** \brief Remove image to initialize if existing (thread-safe). */
	void RemoveInitImage( deoglRImage *image );
	
	
	
	/** \brief Init skin list (not thread-safe). */
	inline const decPointerOrderedSet &GetInitSkinList() const{ return pInitSkinList; }
	
	/** \brief Add skin to initialize if not existing already (thread-safe). */
	void AddInitSkin( deoglRSkin *skin );
	
	/** \brief Remove skin to initialize if existing (thread-safe). */
	void RemoveInitSkin( deoglRSkin *skin );
	
	
	
	/** \brief Init model list (not thread-safe). */
	inline const decPointerOrderedSet &GetInitModelList() const{ return pInitModelList; }
	
	/** \brief Add model to initialize if not existing already (thread-safe). */
	void AddInitModel( deoglRModel *model );
	
	/** \brief Remove model to initialize if existing (thread-safe). */
	void RemoveInitModel( deoglRModel *model );
	/*@}*/
	
	
	
	/** \name Delayed render thread OpengGL deletion */
	/*@{*/
	/** \brief Has free operations (not thread-safe). */
	inline bool GetHasFreeOperations() const{ return pHasFreeOperations; }
	
	/**
	 * \brief Process free operations (thread-safe).
	 * 
	 * \param deleteAll Set to \em true to delete all pending tasks. If set to \em false only
	 *                  a maximum number of objects is deleted to avoid hickups if a large
	 *                  amount of delete task piled up.
	 */
	void ProcessFreeOperations( bool deleteAll );
	
	
	
	/** \brief Add deletion (thread-safe). */
	void AddDeletion( deoglDelayedDeletion *deletion );
	/*@}*/
	
	
	
	/** \name Delayed main thread synchronization. */
	/*@{*/
	/** \brief Has synchronize operations (not thread-safe). */
	inline bool GetHasSynchronizeOperations() const{ return pHasSynchronizeOperations; }
	
	/**
	 * \brief Process synchronize operations (thread-safe).
	 * \details Main thread call.
	 */
	void ProcessSynchronizeOperations();
	
	
	
	/** \brief File write list (not thread-safe). */
	inline const decPointerOrderedSet &GetFileWriteList() const{ return pFileWriteList; }
	
	/** \brief Add file write (thread-safe). */
	void AddFileWrite( deoglDelayedFileWrite *fileWrite );
	
	
	
	/** \brief Save image list (not thread-safe). */
	inline const decPointerOrderedSet &GetSaveImageList() const{ return pSaveImageList; }
	
	/** \brief Add save image (thread-safe). */
	void AddSaveImage( deoglDelayedSaveImage *saveImage );
	
	
	
	/** \brief Clean up camera list (not thread-safe). */
	inline const decObjectOrderedSet &GetCleanUpCameraList() const{ return pCleanUpCameraList; }
	
	/** \brief Add clean up camera (thread-safe). */
	void AddCleanUpCamera( deoglRCamera *camera );
	/*@}*/
	
private:
	void pCleanUp();
	
	void pProcessImage( deoglRImage &image );
	void pProcessSkin( deoglRSkin &skin );
	void pProcessModel( deoglRModel &model );
	void pGenerateConeMap( deoglRSkin &skin, const deoglSkinTexture &texture );
};

#endif
