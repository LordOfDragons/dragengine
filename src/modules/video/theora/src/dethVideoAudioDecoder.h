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

#ifndef _DETHVIDEOAUDIODECODER_H_
#define _DETHVIDEOAUDIODECODER_H_

#include <vorbis/codec.h>

#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/systems/modules/video/deBaseVideoAudioDecoder.h>

class deVideoTheora;



/**
 * \brief Theora video decoder.
 */
class dethVideoAudioDecoder : public deBaseVideoAudioDecoder{
private:
	deVideoTheora &pModule;
	
	ogg_sync_state pSyncState;
	ogg_stream_state pStreamState;
	vorbis_comment pComment;
	vorbis_info pInfo;
	vorbis_dsp_state pDecoderContext;
	vorbis_block pDecoderBlock;
	int pSerial;
	bool pDecoderContextInited;
	bool pDecoderBlockInited;
	int pFileSize;
	int pFileRemaining;
	
	int pBytesPerSample;
	int pSampleCount;
	int pSampleRate;
	int pChannelCount;
	
	int pBufferSampleSize;
	
	int pCurSample;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create decoder. */
	dethVideoAudioDecoder(deVideoTheora &module, decBaseFileReader &reader);
	
	/** \brief Clean up decoder. */
	virtual ~dethVideoAudioDecoder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Bytes per sample or 0 if no audio. */
	inline int GetBytesPerSample() const{ return pBytesPerSample; }
	
	/** \brief Number of samples or 0 if no audio. */
	inline int GetSampleCount() const{ return pSampleCount; }
	
	/** \brief Number of channels or 0 if no audio. */
	inline int GetChannelCount() const{ return pChannelCount; }
	
	/** \brief Sample rate or 0 if no audio. */
	inline int GetSampleRate() const{ return pSampleRate; }
	
	
	
	/** \brief File position in samples from the beginning. */
	virtual int GetPosition();
	
	/** \brief Set file position in samples from the beginning. */
	virtual void SetPosition(int position);
	
	/**
	 * \brief Read chunk of sound data from current file position and advance.
	 * 
	 * Reads requested amount of samples into buffer. If successful the file position is
	 * advanced. Returns number of bytes written to the buffer. This can be less than the
	 * requested bytes required due to the number of requested samples because end of file
	 * has been reached. If reading fails an error is signaled using the engine error
	 * signaling and 0 returned.
	 */
	virtual int ReadSamples(void *buffer, int size);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pFindAudioStream();
	int pReadFromFile(char *buffer, int size);
	bool pReadPage(ogg_page &page);
	bool pReadVorbisHeader(vorbis_info &info, vorbis_comment &comment, bool &finished);
	void pRewind();
	void pSeek (int sample);
};

#endif
