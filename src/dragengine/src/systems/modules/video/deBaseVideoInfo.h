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

#ifndef _DEBASEVIDEOINFOS_H_
#define _DEBASEVIDEOINFOS_H_

#include "../deBaseModule.h"
#include "../../../common/math/decMath.h"


/**
 * \brief Video information.
 */
class DE_DLL_EXPORT deBaseVideoInfo{
private:
	int pWidth;
	int pHeight;
	int pComponentCount;
	int pBitCount;
	int pFrameCount;
	float pFrameRate;
	decColorMatrix3 pColorConversionMatrix;
	
	int pBytesPerSample;
	int pSampleCount;
	int pSampleRate;
	int pChannelCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create info. */
	deBaseVideoInfo();
	
	/** \brief Clean up info. */
	~deBaseVideoInfo();
	/*@}*/
	
	
	
	/** \name Video Information */
	/*@{*/
	/** \brief Width in pixels. */
	inline int GetWidth() const{return pWidth;}
	
	/** \brief Set width in pixels. */
	void SetWidth(int width);
	
	/** \brief Height in pixels. */
	inline int GetHeight() const{return pHeight;}
	
	/** \brief Set height in pixels. */
	void SetHeight(int height);
	
	/** \brief Component count. */
	inline int GetComponentCount() const{return pComponentCount;}
	
	/** \brief Set component count. */
	void SetComponentCount(int componentCount);
	
	/** \brief Bits per pixel. */
	inline int GetBitCount() const{return pBitCount;}
	
	/** \brief Set bits per pixel. */
	void SetBitCount(int bitCount);
	
	/** \brief Number of frames. */
	inline int GetFrameCount() const{return pFrameCount;}
	
	/** \brief Set number of frames. */
	void SetFrameCount(int frameCount);
	
	/** \brief Frame rate. */
	inline float GetFrameRate() const{return pFrameRate;}
	
	/** \brief Set frame rate. */
	void SetFrameRate(float frameRate);
	
	/** \brief Color conversion matrix. */
	inline const decColorMatrix3 &GetColorConversionMatrix() const{return pColorConversionMatrix;}
	
	/** \brief Set color conversion matrix. */
	void SetColorConversionMatrix(const decColorMatrix3 &matrix);
	/*@}*/
	
	
	
	/** \name Audio Information */
	/*@{*/
	/** \brief Bytes per sample or 0 if no audio. */
	inline int GetBytesPerSample() const{return pBytesPerSample;}
	
	/** \brief Set bytes per sample or 0 if no audio. */
	void SetBytesPerSample(int bytesPerSample);
	
	/** \brief Number of samples or 0 if no audio. */
	inline int GetSampleCount() const{return pSampleCount;}
	
	/** \brief Set number of samples or 0 if no audio. */
	void SetSampleCount(int sampleCount);
	
	/** \brief Number of channels or 0 if no audio. */
	inline int GetChannelCount() const{return pChannelCount;}
	
	/** \brief Set number of channels or 0 if no audio. */
	void SetChannelCount(int channelCount);
	
	/** \brief Sample rate or 0 if no audio. */
	inline int GetSampleRate() const{return pSampleRate;}
	
	/** \brief Set sample rate or 0 if no audio. */
	void SetSampleRate(int sampleRate);
	/*@}*/
};

#endif
