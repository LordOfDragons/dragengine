/* 
 * Drag[en]gine WebM Video Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEWMTRACKCALLBACK_H_
#define _DEWMTRACKCALLBACK_H_

#include <stdint.h>
#include <webm/callback.h>

class deVideoWebm;


/**
 * Base callback for reading a WebM track.
 */
class dewmTrackCallback : public webm::Callback{
private:
	deVideoWebm &pModule;
	bool pTrackOpen;
	std::uint64_t pTrackNumber;
	std::uint8_t *pBuffer;
	std::uint64_t pBufferSize;
	bool pStopParsing;
	bool pNeedMoreFrames;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create callback. */
	dewmTrackCallback( deVideoWebm &module );
	
	/** Clean up callback. */
	virtual ~dewmTrackCallback();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Module. */
	inline deVideoWebm &GetModule(){ return pModule; }
	
	/** End parsing segment. */
	virtual webm::Status OnSegmentEnd( const webm::ElementMetadata &metadata );
	
	/** Track entry. */
	virtual webm::Status OnTrackEntry( const webm::ElementMetadata &metadata,
		const webm::TrackEntry &track_entry );
	
	/** Blocks. */
	virtual webm::Status OnSimpleBlockBegin( const webm::ElementMetadata &metadata,
		const webm::SimpleBlock &simple_block, webm::Action *action );
	
	virtual webm::Status OnBlockGroupBegin( const webm::ElementMetadata &metadata,
		webm::Action *action );
	
	virtual webm::Status OnBlockGroupEnd( const webm::ElementMetadata &metadata,
		const webm::BlockGroup &block_group );
	
	virtual webm::Status OnBlockBegin( const webm::ElementMetadata &metadata,
		const webm::Block &block, webm::Action *action );
	
	/** Frame. */
	virtual webm::Status OnFrame( const webm::FrameMetadata &metadata, webm::Reader *reader,
		std::uint64_t *bytes_remaining );
	/*@}*/
	
	
	
protected:
	virtual bool pOpenTrack( const webm::TrackEntry &track ) = 0;
	
	virtual void pProcessFrame( webm::Reader &reader, std::uint64_t &bytes_remaining );
	virtual void pProcessAdditional( const std::vector<unsigned char> &data );
	
	inline const std::uint8_t *pGetBuffer() const{ return pBuffer; }
	
	void pReadFrameData( webm::Reader &reader, std::uint64_t &bytes_remaining );
	
	void SetNeedMoreFrames( bool needMoreFrames );
	
	virtual void pEndSegment();
	
	
	
private:
	webm::Status pProcessBlock( const webm::Block &block, webm::Action *action );
};

#endif
