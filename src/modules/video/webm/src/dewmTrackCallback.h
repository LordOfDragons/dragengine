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
