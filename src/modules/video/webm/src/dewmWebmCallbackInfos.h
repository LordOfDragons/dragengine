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

#ifndef _DEWMWEBMCALLBACKINFOS_H_
#define _DEWMWEBMCALLBACKINFOS_H_

#include <stdint.h>
#include <webm/callback.h>

#include <dragengine/common/math/decMath.h>

class dewmInfos;
class deVideoWebm;


/**
 * Callback finding WebM information only. Updates a dewmInfos with the found information.
 */
class dewmWebmCallbackInfos : public webm::Callback{
private:
	enum eState{
		esNone,
		esBlockVideo,
		esBlockAudio
	};
	
	deVideoWebm &pModule;
	dewmInfos &pInfos;
	
	eState pState;
	bool pFirstVideoFrame;
	bool pFirstAudioFrame;
	
	std::uint64_t pTimecodeScale;
	double pTimecodeFactor;
	std::uint64_t pFrameCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create callback. */
	dewmWebmCallbackInfos( deVideoWebm &module, dewmInfos &infos );
	
	/** Clean up callback. */
	virtual ~dewmWebmCallbackInfos();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Start parsing segment. */
	virtual webm::Status OnSegmentBegin( const webm::ElementMetadata &metadata, webm::Action *action );
	
	/** End parsing segment. */
	virtual webm::Status OnSegmentEnd( const webm::ElementMetadata &metadata );
	
	/** Segment information. */
	virtual webm::Status OnInfo( const webm::ElementMetadata &metadata, const webm::Info &info );
	
	/** Track entry. */
	virtual webm::Status OnTrackEntry( const webm::ElementMetadata &metadata,
		const webm::TrackEntry &track_entry );
	
	/** Cluster begin. */
	virtual webm::Status OnClusterBegin( const webm::ElementMetadata &metadata,
		const webm::Cluster &cluster, webm::Action *action );
	
	/** Blocks. */
	virtual webm::Status OnSimpleBlockBegin( const webm::ElementMetadata &metadata,
		const webm::SimpleBlock &simple_block, webm::Action *action );
	
	virtual webm::Status OnBlockBegin( const webm::ElementMetadata &metadata,
		const webm::Block &block, webm::Action *action );
	
	/** Frame. */
	virtual webm::Status OnFrame( const webm::FrameMetadata &metadata, webm::Reader *reader,
		std::uint64_t *bytes_remaining );
	/*@}*/
	
	
	
protected:
	webm::Status pOnTrackVideo( const webm::TrackEntry &track );
	webm::Status pOnTrackAudio( const webm::TrackEntry &track );
	void pProcessBlock( const webm::Block &block );
	webm::Status pProcessFirstFrameVideo( webm::Reader &reader, std::uint64_t &bytes_remaining );
	webm::Status pProcessFirstFrameAudio( webm::Reader &reader, std::uint64_t &bytes_remaining );
	
	decColorMatrix3 pCreateColorConversionMatrix() const;
};

#endif
