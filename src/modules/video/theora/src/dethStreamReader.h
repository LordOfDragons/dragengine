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

// include only once
#ifndef _DETHSTREAMREADER_H_
#define _DETHSTREAMREADER_H_

// includes
#include <ogg/ogg.h>
#include <theora/theoradec.h>

// predefinitions
class dethOggReader;
class dethInfos;



/**
 * @brief Ogg Stream Reader.
 */
class dethStreamReader{
private:
	dethOggReader *pReader;
	int pSerial;
	ogg_stream_state pStreamState;
	th_dec_ctx *pDecoderContext;
	int pVersion;
	int pFrameOffset;
	int pGranuleShift;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new ogg file. */
	dethStreamReader(dethOggReader *reader, int serial);
	/** Cleans up the ogg file. */
	~dethStreamReader();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the serial. */
	inline int GetSerial() const{return pSerial;}
	/** Retrieves the stream state. */
	inline ogg_stream_state &GetStreamState(){return pStreamState;}
	/** Retrieves the frame index for a granule position. */
	int GranuleToFrame(ogg_int64_t granule) const;
	/** Retrieves the i-frame index for a granule position. */
	int GranuleToIFrame(ogg_int64_t granule) const;
	/** Retrieves the p-frame index for a granule position. */
	int GranuleToPFrame(ogg_int64_t granule) const;
	/** Convert a i/p-frame into a granule. */
	ogg_int64_t FrameToGranule(int iframe, int pframe) const;
	/** Sets granule position. */
	void SetGranulePosition(ogg_int64_t granule);
	
	/** Add a page. */
	void AddPage(ogg_page &page);
	/** Add packet to decoder. */
	int AddPacketToDecoder(ogg_packet &packet, ogg_int64_t &granule);
	/** Reads a packet. */
	int ReadPacket(ogg_packet &packet);
	/** Peek a packet. */
	int PeekPacket(ogg_packet &packet);
	/** Submit a packet. */
	void SubmitPacket(ogg_packet &packet);
	/**
	 * Tries to read a full theora header. Stores information in the provided
	 * video info object. Returns true if the read packets are part of the
	 * header or false otherwise. Stops reading packets once the first data
	 * packet arrives. In this case the header finished flag is set to true
	 * in the video info object. Hence call ReadTheoraHeader as long as it
	 * returns true and the header finished flag is not set.
	 */
	bool ReadTheoraHeader(dethInfos &infos);
	
	/** Reset stream. */
	void Reset();
	/** Retrieve next decoded frame if existing. */
	bool GetDecodedFrame(th_ycbcr_buffer &buffer);
	/*@}*/
};

// end of include only once
#endif
