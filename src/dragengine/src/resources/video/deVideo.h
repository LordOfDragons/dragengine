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

#ifndef _DEVIDEO_H_
#define _DEVIDEO_H_

#include "../deFileResource.h"
#include "../../common/math/decMath.h"

class deVideoManager;
class deBaseGraphicVideo;


/**
 * \brief Video class.
 *
 * Stores a video. Videos are sequences of images used to play short videos once
 * or in a loop. Videos can be of streaming or non-streaming type. For
 * non-streaming videos the data is loaded into a memory buffer shared. This is
 * the best choice for small videos which do not require a lot of memory or are
 * used in many places. For streaming videos no memory buffer is held. Streaming
 * videos use a Video Decoder object to retrieve the image data. This is the best
 * choice for large videos or videos which are used only a few times. The format
 * of the video data can be RGB or YUV. The same data structures as in the image
 * resource can be found. For the YUV format the mapping r=y, g=u and b=v is used.
 */
class DE_DLL_EXPORT deVideo : public deFileResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deVideo> Ref;
	
	
private:
	int pWidth;
	int pHeight;
	int pComponentCount;
	int pBitCount;
	int pFrameCount;
	float pFrameRate;
	float pPlayTime;
	decColorMatrix3 pColorConversionMatrix;
	
	int pBytesPerSample;
	int pSampleCount;
	int pSampleRate;
	int pChannelCount;
	
	deBaseGraphicVideo *pPeerGraphic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create video. */
	deVideo(deVideoManager *manager, deVirtualFileSystem *vfs, const char *filename,
		TIME_SYSTEM modificationTime, int width, int height, int componentCount,
		int bitCount, float frameRate, int frameCount,
		const decColorMatrix3 &colorConversionMatrix, int bytesPerSample,
		int sampleCount, int sampleRate, int channelCount);
	
	/**
	 * \brief Create video for internal loading.
	 * \warning For use by deResourceLoader only.
	 */
	deVideo(deVideoManager *manager, deVirtualFileSystem *vfs, const char *filename,
		TIME_SYSTEM modificationTime);
	
protected:
	/**
	 * \brief Clean up video object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deVideo() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Width in pixels. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Height in pixels. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Component count. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** \brief Number of frames. */
	inline int GetFrameCount() const{ return pFrameCount; }
	
	/** \brief Frame rate. */
	inline float GetFrameRate() const{ return pFrameRate; }
	
	/** \brief Bits per pixel. */
	inline int GetBitCount() const{ return pBitCount; }
	
	/** \brief Play time in seconds. */
	inline float GetPlayTime() const{ return pPlayTime; }
	
	/** \brief Color conversion matrix. */
	inline const decColorMatrix3 &GetColorConversionMatrix() const{ return pColorConversionMatrix; }
	
	/** \brief Bytes per sample or 0 if no audio. */
	inline int GetBytesPerSample() const{ return pBytesPerSample; }
	
	/** \brief Number of samples or 0 if no audio. */
	inline int GetSampleCount() const{ return pSampleCount; }
	
	/** \brief Number of channels or 0 if no audio. */
	inline int GetChannelCount() const{ return pChannelCount; }
	
	/** \brief Sample rate or 0 if no audio. */
	inline int GetSampleRate() const{ return pSampleRate; }
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer. */
	inline deBaseGraphicVideo *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer. */
	void SetPeerGraphic(deBaseGraphicVideo *peer);
	/*@}*/
	
	
	
	/** \name Special */
	/*@{*/
	/**
	 * \brief Finalize construction after asynchronous loading.
	 * \warning For use by deResourceLoader only.
	 */
	void FinalizeConstruction(int width, int height, int componentCount, int bitCount,
		float frameRate, int frameCount, const decColorMatrix3 &colorConversionMatrix,
		int bytesPerSample, int sampleCount, int sampleRate, int channelCount);
	/*@}*/
};

#endif
