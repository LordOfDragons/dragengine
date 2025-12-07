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

#ifndef _DEOGLIMAGESTAGEMANAGER_H_
#define _DEOGLIMAGESTAGEMANAGER_H_

#include "../deoglBasics.h"
#include "../skin/channel/deoglSkinChannel.h"

#define OGL_MAX_IMAGE_STAGES 16

class deoglArrayTexture;
class deoglCubeMap;
class deoglRenderThread;
class deoglRSkin;
class deoglTexture;


/**
 * Image stage manager.
 * 
 * Allows to enable and disable stages and bind different kinds of images.
 * Keeps track of bound images avoiding not require state changes.
 */
class deoglImageStageManager{
public:
	/** Access. */
	enum eAccess{
		eaRead,
		eaWrite,
		eaReadWrite
	};
	
private:
	struct sStage{
		GLuint texture;
		GLenum type;
		GLint level;
		GLint layer;
		eAccess access;
		bool layered;
		GLenum format;
		sStage();
	};
	
private:
	deoglRenderThread &pRenderThread;
	sStage pStages[OGL_MAX_IMAGE_STAGES];
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create image stage manager. */
	deoglImageStageManager(deoglRenderThread &renderThread);
	
	/** Clean up image stage manager. */
	~deoglImageStageManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of enabled stages. */
	int GetEnabledStageCount() const;
	
	/** Type of imagebound to stage. */
	GLenum GetStageType(int stage) const;
	
	/** Enable stage and bind image. */
	void Enable(int stage, const deoglTexture &texture, int level, eAccess access);
	
	/** Enable stage and bind cube map. */
	void Enable(int stage, const deoglCubeMap &cubemap, int level, eAccess access);
	
	/** Enable stage and bind cube map face. */
	void Enable(int stage, const deoglCubeMap &cubemap, int level, int face, eAccess access);
	
	/** Enable stage and bind array texture. */
	void Enable(int stage, const deoglArrayTexture &texture, int level, eAccess access);
	
	/** Enable stage and bind array texture layer. */
	void Enable(int stage, const deoglArrayTexture &texture, int level, int layer, eAccess access);
	
	/** Enable stage and bind texture buffer object. */
	void EnableTBO(int stage, GLuint tbo, eAccess access, GLenum format);
	
	/** Enable stage and bind texture channel from a skin. */
	void EnableSkin(int stage, const deoglRSkin &skin, int texture,
		deoglSkinChannel::eChannelTypes channel, deoglTexture *defaultTexture,
		eAccess access);
	
	/** Enable render doc debug texture if debug mode contains flag. */
	void EnableRenderDocDebug(int stage, int flag);
	
	/** Disable stage. */
	void DisableStage(int stage);
	
	/**
	 * Disable all stages above stage. If you have for example 4 enabled stages and you want
	 * to disable stages 2 and 3 then you have to call this function with the stage of the
	 * last stage to remain enabled thus 1 in this case.
	 */
	void DisableStagesAbove(int stage);
	
	/** Disable all stages. */
	void DisableAllStages();
	
	
	
	/** Activate stage and bind texture if not bound already. */
	void BindImage(int stage, GLuint texture, GLenum type, int level, int layer,
		eAccess access, bool layered, GLenum format);
	
	
	
	/** Dump the current state to the logger. */
	void LogCurrentState() const;
	/*@}*/
};

#endif
