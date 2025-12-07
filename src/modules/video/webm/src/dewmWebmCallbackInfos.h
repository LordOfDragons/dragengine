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
	dewmWebmCallbackInfos(deVideoWebm &module, dewmInfos &infos);
	
	/** Clean up callback. */
	virtual ~dewmWebmCallbackInfos();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Start parsing segment. */
	virtual webm::Status OnSegmentBegin(const webm::ElementMetadata &metadata, webm::Action *action);
	
	/** End parsing segment. */
	virtual webm::Status OnSegmentEnd(const webm::ElementMetadata &metadata);
	
	/** Segment information. */
	virtual webm::Status OnInfo(const webm::ElementMetadata &metadata, const webm::Info &info);
	
	/** Track entry. */
	virtual webm::Status OnTrackEntry(const webm::ElementMetadata &metadata,
		const webm::TrackEntry &track_entry);
	
	/** Cluster begin. */
	virtual webm::Status OnClusterBegin(const webm::ElementMetadata &metadata,
		const webm::Cluster &cluster, webm::Action *action);
	
	/** Blocks. */
	virtual webm::Status OnSimpleBlockBegin(const webm::ElementMetadata &metadata,
		const webm::SimpleBlock &simple_block, webm::Action *action);
	
	virtual webm::Status OnBlockBegin(const webm::ElementMetadata &metadata,
		const webm::Block &block, webm::Action *action);
	
	/** Frame. */
	virtual webm::Status OnFrame(const webm::FrameMetadata &metadata, webm::Reader *reader,
		std::uint64_t *bytes_remaining);
	/*@}*/
	
	
	
protected:
	webm::Status pOnTrackVideo(const webm::TrackEntry &track);
	webm::Status pOnTrackAudio(const webm::TrackEntry &track);
	void pProcessBlock(const webm::Block &block);
	webm::Status pProcessFirstFrameVideo(webm::Reader &reader, std::uint64_t &bytes_remaining);
	webm::Status pProcessFirstFrameAudio(webm::Reader &reader, std::uint64_t &bytes_remaining);
	
	decColorMatrix3 pCreateColorConversionMatrix() const;
};

#endif
