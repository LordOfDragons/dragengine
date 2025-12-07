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

#ifndef _DEOGLDELAYEDOPERATIONS_H_
#define _DEOGLDELAYEDOPERATIONS_H_

#include "../deoglBasics.h"

#include <dragengine/common/collection/decPointerOrderedSet.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/threading/deMutex.h>

class deoglRCamera;
class deoglRFont;
class deoglRFontSize;
class deoglRImage;
class deoglRModel;
class deoglRenderThread;
class deoglRSkin;
class deoglSkinTexture;
class deoglDelayedFileWrite;
class deoglDelayedSaveImage;
class deoglShaderProgram;



/**
 * Delayed operations.
 * 
 * Resources can be created at any time including asynchronously. OpenGL doesn't allow or
 * reacts badly to creating or manipulating OpenGL related objects outside a well defined
 * render loop. To prevent problems these operations have to be delayed until a time when
 * it is safe to carry them out. Such delayed operations are collected in a single instance
 * of this class and carried out either at the beginning or after the end of a frame update.
 * It is garanteed that all pending delayed operations are carried out during this time.
 */
class deoglDelayedOperations{
public:
	enum eOpenGLObjectType{
		eoglotBuffer,
		eoglotBuffererPersistUnmap,
		eoglotTexture,
		eoglotVertexArray,
		eoglotFramebuffer,
		eoglotQuery,
		eoglotSampler,
		eoglotProgram,
		eoglotShader,
		eoglotSync
	};
	
	
	
private:
	struct sOpenGLObject{
		eOpenGLObjectType type;
		GLuint name;
		GLsync sync;
		
		inline void Set(eOpenGLObjectType ptype, GLuint pname){
			type = ptype;
			name = pname;
			sync = nullptr;
		}
		
		inline void Set(eOpenGLObjectType ptype, GLsync psync){
			type = ptype;
			name = 0;
			sync = psync;
		}
	};
	
	deoglRenderThread &pRenderThread;
	
	deMutex pMutexAsyncResInit;
	bool pHasAsyncResInitOperations;
	decPointerOrderedSet pAsyncResInitSkinList;
	decPointerOrderedSet pAsyncResInitFontList, pAsyncResInitFontSizeList;

	deMutex pMutexRecreateRes;
	decPointerOrderedSet pRecreateSkinList;
	
	deMutex pMutexInit;
	bool pHasInitOperations;
	decPointerOrderedSet pInitImageList;
	decPointerOrderedSet pInitSkinList;
	decPointerOrderedSet pInitModelList;
	
	deMutex pMutexOGLObjects;
	sOpenGLObject *pOGLObjects;
	int pOGLObjectCount;
	int pOGLObjectSize;
	
	deMutex pMutexCameras;
	decObjectOrderedSet pCleanUpCameraList;
	
	deMutex pMutexReleaseObjects;
	decObjectOrderedSet pReleaseObjects;
	
	deMutex pMutexSynchronize;
	bool pHasSynchronizeOperations;
	decPointerOrderedSet pFileWriteList;
	decPointerOrderedSet pSaveImageList;
	
	const deoglShaderProgram *pShaderGenConeMap;
	const deoglShaderProgram *pShaderGenConeMapLayer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create delayed operations object. */
	deoglDelayedOperations(deoglRenderThread &renderThread);
	
	/** Clean up delayed operations object. */
	~deoglDelayedOperations();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{return pRenderThread;}
	
	/** Mutex. */
	inline deMutex &GetMutex(){return pMutexInit;}
	/*@}*/
	
	
	
	/** \name Delayed main thread asynchronous resources initialization. */
	/*@{*/
	/** Has asynchronous resource init operations (not thread-safe). */
	inline bool GetHasAsyncResInitOperations() const{return pHasAsyncResInitOperations;}
	
	/**
	 * Process asynchronous resource init operations (thread-safe).
	 * \details Called from main thread to finalize initialize asynchronously loaded resources.
	 *          This is required to deal with accessing render objects in a safe way.
	 *          Operates on the following lists:
	 *          - Render Skins (deoglRSkin)
	 *          - Render Fonts (deoglRFont, deoglRFontSize)
	 */
	void ProcessAsyncResInitOperations();
	


	/**
	 * Process recreate resource operations (thread-safe).
	 * \details Called from main thread to recreate failed resources.
	 *          This is required to deal with accessing render objects in a safe way.
	 *          Operates on the following lists:
	 *          - Render Skins (deoglRSkin)
	 */
	void ProcessRecreateResOperations();
	
	
	
	/** Async res init skin list (not thread-safe). */
	inline const decPointerOrderedSet &GetAsyncResInitSkinList() const{return pAsyncResInitSkinList;}
	
	/** Add skin to async res initialize if not existing already (thread-safe). */
	void AddAsyncResInitSkin(deoglRSkin *skin);
	
	/** Remove skin to async res initialize if existing (thread-safe). */
	void RemoveAsyncResInitSkin(deoglRSkin *skin);
	
	
	
	/** Async res init font list (not thread-safe). */
	inline const decPointerOrderedSet &GetAsyncResInitFontList() const{return pAsyncResInitFontList;}
	
	/** Add font to async res initialize if not existing already (thread-safe). */
	void AddAsyncResInitFont(deoglRFont *font);
	
	/** Remove font to async res initialize if existing (thread-safe). */
	void RemoveAsyncResInitFont(deoglRFont *font);
	
	
	
	/** Async res init font size list (not thread-safe). */
	inline const decPointerOrderedSet &GetAsyncResInitFontSizeList() const{return pAsyncResInitFontSizeList;}
	
	/** Add font size to async res initialize if not existing already (thread-safe). */
	void AddAsyncResInitFontSize(deoglRFontSize *size);
	
	/** Remove font size to async res initialize if existing (thread-safe). */
	void RemoveAsyncResInitFontSize(deoglRFontSize *size);
	/*@}*/
	
	
	
	/** \name Delayed render thread OpengGL initialization */
	/*@{*/
	/** Has init operations (not thread-safe). */
	inline bool GetHasInitOperations() const{return pHasInitOperations;}
	
	/**
	 * Process init operations (thread-safe).
	 * \details Called from render thread to initialize OpenGL related objects.
	 *          Operates on the following lists:
	 *          - Render Images (deoglRImage)
	 *          - Render Models (deoglRModel)
	 *          - Render Skins (deoglRSkin)
	 */
	void ProcessInitOperations();
	
	
	
	/** Init image list (not thread-safe). */
	inline const decPointerOrderedSet &GetInitImageList() const{return pInitImageList;}
	
	/** Add image to initialize if not existing already (thread-safe). */
	void AddInitImage(deoglRImage *image);
	
	/** Remove image to initialize if existing (thread-safe). */
	void RemoveInitImage(deoglRImage *image);
	
	
	
	/** Init skin list (not thread-safe). */
	inline const decPointerOrderedSet &GetInitSkinList() const{return pInitSkinList;}
	
	/** Add skin to initialize if not existing already (thread-safe). */
	void AddInitSkin(deoglRSkin *skin);
	
	/** Remove skin to initialize if existing (thread-safe). */
	void RemoveInitSkin(deoglRSkin *skin);
	
	
	
	/** Init model list (not thread-safe). */
	inline const decPointerOrderedSet &GetInitModelList() const{return pInitModelList;}
	
	/** Add model to initialize if not existing already (thread-safe). */
	void AddInitModel(deoglRModel *model);
	
	/** Remove model to initialize if existing (thread-safe). */
	void RemoveInitModel(deoglRModel *model);
	/*@}*/
	
	
	
	/** Process free operations (thread-safe). */
	void ProcessFreeOperations(bool deleteAll = false);
	
	/** Add OpenGL object deletion if not 0 (thread-safe). */
	void DeleteOpenGLObject(eOpenGLObjectType type, GLuint name);
	void DeleteOpenGLObject(eOpenGLObjectType type, GLsync sync);
	inline void DeleteOpenGLBuffer(GLuint name){DeleteOpenGLObject(eoglotBuffer, name);}
	inline void DeleteOpenGLBufferPersistUnmap(GLuint name){DeleteOpenGLObject(eoglotBuffererPersistUnmap, name);}
	inline void DeleteOpenGLTexture(GLuint name){DeleteOpenGLObject(eoglotTexture, name);}
	inline void DeleteOpenGLVertexArray(GLuint name){DeleteOpenGLObject(eoglotVertexArray, name);}
	inline void DeleteOpenGLFramebuffer(GLuint name){DeleteOpenGLObject(eoglotFramebuffer, name);}
	inline void DeleteOpenGLQuery(GLuint name){DeleteOpenGLObject(eoglotQuery, name);}
	inline void DeleteOpenGLSampler(GLuint name){DeleteOpenGLObject(eoglotSampler, name);}
	inline void DeleteOpenGLProgram(GLuint name){DeleteOpenGLObject(eoglotProgram, name);}
	inline void DeleteOpenGLShader(GLuint name){DeleteOpenGLObject(eoglotShader, name);}
	inline void DeleteOpenGLSync(GLsync sync){DeleteOpenGLObject(eoglotSync, sync);}
	/*@}*/
	
	
	
	/** \name Delayed main thread synchronization. */
	/*@{*/
	/** Has synchronize operations (not thread-safe). */
	inline bool GetHasSynchronizeOperations() const{return pHasSynchronizeOperations;}
	
	/**
	 * Process synchronize operations (thread-safe).
	 * \details Main thread call.
	 */
	void ProcessSynchronizeOperations();
	
	
	
	/** File write list (not thread-safe). */
	inline const decPointerOrderedSet &GetFileWriteList() const{return pFileWriteList;}
	
	/** Add file write (thread-safe). */
	void AddFileWrite(deoglDelayedFileWrite *fileWrite);
	
	
	
	/** Save image list (not thread-safe). */
	inline const decPointerOrderedSet &GetSaveImageList() const{return pSaveImageList;}
	
	/** Add save image (thread-safe). */
	void AddSaveImage(deoglDelayedSaveImage *saveImage);
	
	
	
	/** Clean up camera list (not thread-safe). */
	inline const decObjectOrderedSet &GetCleanUpCameraList() const{return pCleanUpCameraList;}
	
	/** Add clean up camera (thread-safe). */
	void AddCleanUpCamera(deoglRCamera *camera);
	
	
	
	/** Release objects list (not thread-safe). */
	inline const decObjectOrderedSet &GetReleaseObjects() const{return pReleaseObjects;}
	
	/** Add release object (thread-safe). */
	void AddReleaseObject(deObject *object);
	
	
	
	/** Clear during cleaning up. */
	void Clear();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pProcessImage(deoglRImage &image);
	void pProcessSkin(deoglRSkin &skin);
	void pProcessModel(deoglRModel &model);
	//void pGenerateConeMap( deoglRSkin &skin, const deoglSkinTexture &texture );
	void pDeleteOpenGLObjects();
};

#endif
