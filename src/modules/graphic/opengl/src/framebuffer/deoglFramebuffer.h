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

#ifndef _DEOGLFRAMEBUFFER_H_
#define _DEOGLFRAMEBUFFER_H_

#include "../deoglBasics.h"
#include <dragengine/deObject.h>

class deoglArrayTexture;
class deoglCubeMap;
class deoglRenderThread;
class deoglTexture;

class deErrorTracePoint;

#define FBO_MAX_ATTACHMENT_COUNT 8



/**
 * Framebuffer Object.
 */
class deoglFramebuffer : public deObject{
public:
	typedef deTObjectReference<deoglFramebuffer> Ref;
	
	
private:
	enum eAttachementTypes{
		eatNone,
		eatTexture,
		eatCubeMap,
		eatCubeMapPosX,
		eatCubeMapNegX,
		eatCubeMapPosY,
		eatCubeMapNegY,
		eatCubeMapPosZ,
		eatCubeMapNegZ,
		eatArrayTexture,
		eatArrayTextureLayer
	};
	
	struct sAttachement{
		GLuint image;
		int type;
		int level;
		int layer;
		
		sAttachement() : image( 0 ), type( eatNone ), level( 0 ), layer( 0 ){}
		
		inline void Reset(){
			image = 0;
			type = eatNone;
			level = 0;
			layer = 0;
		}
		
		inline void Set( GLuint aimage, int atype ){
			image = aimage;
			type = atype;
			level = 0;
			layer = 0;
		}
		inline void Set( GLuint aimage, int atype, int alevel ){
			image = aimage;
			type = atype;
			level = alevel;
			layer = 0;
		}
		inline void Set( GLuint aimage, int atype, int alevel, int alayer ){
			image = aimage;
			type = atype;
			layer = alayer;
			level = alevel;
		}
		
		inline bool Matches( GLuint aimage, int atype, int alevel, int alayer ) const{
			return image == aimage && type == atype && level == alevel && layer == alayer;
		}
		
		inline bool DoesNotMatch( GLuint aimage, int atype ) const{
			return image != aimage || type != atype || level != 0 || layer != 0;
		}
		inline bool DoesNotMatch( GLuint aimage, int atype, int alevel ) const{
			return image != aimage || type != atype || level != alevel || layer != 0;
		}
		inline bool DoesNotMatch( GLuint aimage, int atype, int alevel, int alayer ) const{
			return image != aimage || type != atype || level != alevel || layer != alayer;
		}
	};
	
	deoglRenderThread &pRenderThread;
	GLuint pFBO;
	bool pPrimary;
	sAttachement pAttColor[ FBO_MAX_ATTACHMENT_COUNT ];
	sAttachement pAttDepth;
	sAttachement pAttStencil;
	int pUsageWidth;
	int pUsageHeight;
	int pUsageCount;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new framebuffer object either as primary or offscreen. */
	deoglFramebuffer(deoglRenderThread &renderThread, bool primary);
	
protected:
	/** Cleans up the framebuffer object. */
	~deoglFramebuffer() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Determines if the framebuffer is a primary framebuffer. */
	inline bool GetIsPrimary() const{ return pPrimary; }
	/** Activates the framebuffer if not active yet. */
	void SetAsCurrent() const;
	/** Verifies the framebuffer for completeness. */
	void Verify();
	
	/** Retrieves the FBO. */
	inline GLuint GetFBO() const{ return pFBO; }
	
	/** Retrieves the usage width. */
	inline int GetUsageWidth() const{ return pUsageWidth; }
	/** Retrieves the usage height. */
	inline int GetUsageHeight() const{ return pUsageHeight; }
	/** Sets the usage resolution. */
	void SetUsageResolution( int width, int height );
	/** Retrieves the usage count. */
	inline int GetUsageCount() const{ return pUsageCount; }
	/** Increases the usage count by one. */
	void IncreaseUsageCount();
	/** Decreases the usage count by one. */
	void DecreaseUsageCount();
	
	/** Determines if a color target is attached. */
	bool IsColorAttached( int index ) const;
	/** Determines if a color target is not attached. */
	bool IsColorDetached( int index ) const;
	/** Attach a color texture. */
	void AttachColorTexture( int index, deoglTexture *texture );
	/** Attach a color texture level. */
	void AttachColorTextureLevel( int index, deoglTexture *texture, int level );
	/** Attach a color cube map. */
	void AttachColorCubeMap( int index, deoglCubeMap *texture );
	/** Attach a color cube map level. */
	void AttachColorCubeMapLevel( int index, deoglCubeMap *texture, int level );
	/** Attach a color cube map face. */
	void AttachColorCubeMapFace( int index, deoglCubeMap *texture, int face );
	/** Attach a color cube map face level. */
	void AttachColorCubeMapFaceLevel( int index, deoglCubeMap *texture, int face, int level );
	/** Attach a color array texture. */
	void AttachColorArrayTexture( int index, deoglArrayTexture *texture );
	/** Attach a color array texture level. */
	void AttachColorArrayTextureLevel( int index, deoglArrayTexture *texture, int level );
	/** Attach a color array texture layer. */
	void AttachColorArrayTextureLayer( int index, deoglArrayTexture *texture, int layer );
	/** Attach a color array texture layer level. */
	void AttachColorArrayTextureLayerLevel( int index, deoglArrayTexture *texture, int layer, int level );
	
	/** Attach color texture. */
	void AttachColorTextureLevel( int index, GLuint texture, int level );
	
	/** Detach a color image if attached. */
	void DetachColorImage( int index );
	/** Detaches all color images in this and all higher attachment points if they are attached. */
	void DetachColorImages( int startIndex );
	
	/** Invalidate color attachment. */
	void InvalidateColor( int index );
	
	
	
	/** Determines if a depth target is attached. */
	bool IsDepthAttached() const;
	/** Determines if no depth target is attached. */
	bool IsDepthDetached() const;
	/** Attach a depth texture. */
	void AttachDepthTexture( deoglTexture *texture );
	/** Attach a depth texture level. */
	void AttachDepthTextureLevel( deoglTexture *texture, int level );
	/** Attach a depth cube map. */
	void AttachDepthCubeMap( deoglCubeMap *texture );
	/** Attach a depth cube map level. */
	void AttachDepthCubeMapLevel( deoglCubeMap *texture, int level );
	/** Attach a depth cube map face. */
	void AttachDepthCubeMapFace( deoglCubeMap *texture, int face );
	/** Attach a depth cube map face level. */
	void AttachDepthCubeMapFaceLevel( deoglCubeMap *texture, int face, int level );
	/** Attach a depth array texture. */
	void AttachDepthArrayTexture( deoglArrayTexture *texture );
	/** Attach a depth array texture level. */
	void AttachDepthArrayTextureLevel( deoglArrayTexture *texture, int level );
	/** Attach a depth array texture layer. */
	void AttachDepthArrayTextureLayer( deoglArrayTexture *texture, int layer );
	/** Attach a depth array texture layer level. */
	void AttachDepthArrayTextureLayerLevel( deoglArrayTexture *texture, int layer, int level );
	/** Detach depth image if attached. */
	void DetachDepthImage();
	
	/** Invalidate depth attachment. */
	void InvalidateDepth();
	
	
	
	/** Determines if a stencil target is attached. */
	bool IsStencilAttached() const;
	/** Determines if no stencil target is attached. */
	bool IsStencilDetached() const;
	/** Attach a stencil texture. */
	void AttachStencilTexture( deoglTexture *texture );
	/** Attach a stencil texture level. */
	void AttachStencilTextureLevel( deoglTexture *texture, int level );
	/** Attach a stencil texture. */
	void AttachStencilArrayTexture( deoglArrayTexture *texture );
	/** Attach a stencil texture level. */
	void AttachStencilArrayTextureLevel( deoglArrayTexture *texture, int level );
	/** Attach a stencil array texture layer. */
	void AttachStencilArrayTextureLayer( deoglArrayTexture *texture, int layer );
	/** Attach a stencil array texture layer level. */
	void AttachStencilArrayTextureLayerLevel( deoglArrayTexture *texture, int layer, int level );
	/** Detach stencil image if attached. */
	void DetachStencilImage();
	
	/** Invalidate stencil attachment. */
	void InvalidateStencil();
	
	/** Invalidate depth and stencil attachments. */
	void InvalidateDepthStencil();
	
	
	
	/** Detach all images if they are attached. */
	void DetachAllImages();
	
	/** Invalidate all images that are attached. */
	void InvalidateAllImages();
	
	
	
	/** Set up read/write buffers matching framebuffer. */
	void UpdateReadWriteBuffers();
	
	/** Adds a configuration trace point with the configuration of this framebuffer object. */
	void AddConfigToTrace( deErrorTracePoint &tracePoint );
	
	/** Debug print framebuffer configuration. */
	void DebugPrint( const char *prefix );
	
	/** Set debug object label. */
	void SetDebugObjectLabel( const char *name );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
