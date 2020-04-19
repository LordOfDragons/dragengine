/* 
 * Drag[en]gine Theora Video Module
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
	dethVideoAudioDecoder( deVideoTheora &module, decBaseFileReader &reader );
	
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
	virtual void SetPosition( int position );
	
	/**
	 * \brief Read chunk of sound data from current file position and advance.
	 * 
	 * Reads requested amount of samples into buffer. If successful the file position is
	 * advanced. Returns number of bytes written to the buffer. This can be less than the
	 * requested bytes required due to the number of requested samples because end of file
	 * has been reached. If reading fails an error is signaled using the engine error
	 * signaling and 0 returned.
	 */
	virtual int ReadSamples( void *buffer, int size );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pFindAudioStream();
	int pReadFromFile( char *buffer, int size );
	bool pReadPage( ogg_page &page );
	bool pReadVorbisHeader( vorbis_info &info, vorbis_comment &comment, bool &finished );
	void pRewind();
	void pSeek ( int sample );
};

#endif
