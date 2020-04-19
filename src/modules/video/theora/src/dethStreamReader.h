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
	dethStreamReader( dethOggReader *reader, int serial );
	/** Cleans up the ogg file. */
	~dethStreamReader();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the serial. */
	inline int GetSerial() const{ return pSerial; }
	/** Retrieves the stream state. */
	inline ogg_stream_state &GetStreamState(){ return pStreamState; }
	/** Retrieves the frame index for a granule position. */
	int GranuleToFrame( ogg_int64_t granule ) const;
	/** Retrieves the i-frame index for a granule position. */
	int GranuleToIFrame( ogg_int64_t granule ) const;
	/** Retrieves the p-frame index for a granule position. */
	int GranuleToPFrame( ogg_int64_t granule ) const;
	/** Convert a i/p-frame into a granule. */
	ogg_int64_t FrameToGranule( int iframe, int pframe ) const;
	/** Sets granule position. */
	void SetGranulePosition( ogg_int64_t granule );
	
	/** Add a page. */
	void AddPage( ogg_page &page );
	/** Add packet to decoder. */
	int AddPacketToDecoder( ogg_packet &packet, ogg_int64_t &granule );
	/** Reads a packet. */
	int ReadPacket( ogg_packet &packet );
	/** Peek a packet. */
	int PeekPacket( ogg_packet &packet );
	/** Submit a packet. */
	void SubmitPacket( ogg_packet &packet );
	/**
	 * Tries to read a full theora header. Stores information in the provided
	 * video info object. Returns true if the read packets are part of the
	 * header or false otherwise. Stops reading packets once the first data
	 * packet arrives. In this case the header finished flag is set to true
	 * in the video info object. Hence call ReadTheoraHeader as long as it
	 * returns true and the header finished flag is not set.
	 */
	bool ReadTheoraHeader( dethInfos &infos );
	
	/** Reset stream. */
	void Reset();
	/** Retrieve next decoded frame if existing. */
	bool GetDecodedFrame( th_ycbcr_buffer &buffer );
	/*@}*/
};

// end of include only once
#endif
