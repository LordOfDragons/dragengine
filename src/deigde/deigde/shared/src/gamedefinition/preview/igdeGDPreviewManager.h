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

#ifndef _IGDEGDPREVIEWMANAGER_H_
#define _IGDEGDPREVIEWMANAGER_H_

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/image/deImage.h>

class igdeEnvironment;
class igdeGDPreviewListener;
class igdeGameDefinition;
class igdeGDClass;
class igdeGDSkin;
class igdeGDSky;

class deEngine;
class deImage;
class decPath;



/**
 * \brief Manages previews for game definition elements.
 * 
 * Creating preview versions of images is done asynchronous. Once
 * created it is stored on disc for later retrieval. Calling the
 * GetImage method returns the image if already preloaded. If the
 * image does not yet exist NULL is returned. In this case the
 * caller has to set the create image placeholder image and call
 * CreateImage method using a listener. Once the image is loaded
 * and ready the listener is called with the created image. The
 * preview manager holds a reference to the listener and frees it
 * once the listener is called.
 */
class DE_DLL_EXPORT igdeGDPreviewManager {
private:
	igdeEnvironment &pEnvironment;
	
	decString pPathCache;
	decString pDirObjectClass;
	decString pDirSkin;
	decString pDirSky;
	
	deImage::Ref pImageCreating;
	deImage::Ref pImageFailed;
	int pImageSize;
	
	decObjectList pCreatorsObjectClass;
	decObjectList pCreatorsSkin;
	decObjectList pCreatorsSky;
	bool pHasCreators;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create preview manager. */
	igdeGDPreviewManager(igdeEnvironment &environment);
	
	/** \brief Clean up preview manager. */
	~igdeGDPreviewManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Path to cache directory. */
	inline const decString &GetPathCache() const{ return pPathCache; }
	
	/** \brief Set path to cache directory. */
	void SetPathCache(const char *path);
	
	/** \brief Directory name for object classes. */
	inline const decString &GetDirectoryObjectClass() const{ return pDirObjectClass; }
	
	/** \brief Set directory name for object classes. */
	void SetDirectoryObjectClass(const char *directory);
	
	/** \brief Directory name for skins. */
	inline const decString &GetDirectorySkin() const{ return pDirSkin; }
	
	/** \brief Set directory name for skins. */
	void SetDirectorySkin(const char *directory);
	
	/** \brief Directory name for skies. */
	inline const decString &GetDirectorySky() const{ return pDirSky; }
	
	/** \brief Set directory name for skies. */
	void SetDirectorySky(const char *directory);
	
	
	
	/** \brief Image for creating preview in progress. */
	inline deImage *GetImageCreating() const{ return pImageCreating; }
	
	
	
	/** \brief Engine has been started. */
	void OnAfterEngineStart();
	
	/** \brief Engine is about to be shut down. */
	void OnBeforeEngineStop();
	
	/**
	 * \brief Update preview creation is pending.
	 * 
	 * Call every frame update best in OnFrameUpdate.
	 */
	void Update();
	
	/** \brief Abort all creators for example due to engine being shut down. */
	void AbortAllCreators();
	
	
	
	/**
	 * \brief Get preview image for object class loading it from cache if required.
	 * \returns NULL if preview image needs to be created.
	 */
	deImage *GetPreviewObjectClass(igdeGDClass *gdclass) const;
	
	/** \brief Create preview image for object class. */
	void CreatePreviewObjectClass(igdeGDClass *gdclass, igdeGDPreviewListener *listener);
	
	/** \brief Clear preview image for object class. */
	void ClearPreviewObjectClass(igdeGDClass *gdclass);
	
	
	
	/**
	 * \brief Get preview image for skin loading it from cache if required.
	 * \returns NULL if preview image needs to be created.
	 */
	deImage *GetPreviewSkin(igdeGDSkin *gdskin) const;
	
	/** \brief Create preview image for skin. */
	void CreatePreviewSkin(igdeGDSkin *gdskin, igdeGDPreviewListener *listener);
	
	/** \brief Clear preview image for skin. */
	void ClearPreviewSkin(igdeGDSkin *gdskin);
	
	
	
	/**
	 * \brief Get preview image for sky loading it from cache if required.
	 * \returns NULL if preview image needs to be created.
	 */
	deImage *GetPreviewSky(igdeGDSky *gdsky) const;
	
	/** \brief Create preview image for sky. */
	void CreatePreviewSky(igdeGDSky *gdsky, igdeGDPreviewListener *listener);
	
	/** \brief Clear preview image for sky. */
	void ClearPreviewSky(igdeGDSky *gdsky);
	/*@}*/
	
	
	
private:
	void pLoadFromFile(deImage::Ref &image, const decString &typedir, const decString &filename) const;
	void pSaveToFile(deImage *image, const decString &typedir, const decString &filename) const;
	void pDeleteFile(const decString &typedir, const decString &filename) const;
};

#endif
