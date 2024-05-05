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

#ifndef _DETHINFOS_H_
#define _DETHINFOS_H_

#include <theora/codec.h>
#include <theora/theoradec.h>
#include <vorbis/codec.h>

#include <dragengine/systems/modules/video/deBaseVideoInfo.h>



/**
 * \brief Theora video information.
 */
class dethInfos : public deBaseVideoInfo{
private:
	th_info pInfo;
	th_comment pComment;
	th_setup_info *pSetupInfo;
	bool pHeaderFinished;
	
	vorbis_info pAudioInfo;
	vorbis_comment pAudioComment;
	bool pAudioHeaderFinished;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create information object. */
	dethInfos();
	
	/** \brief Clean up information object. */
	~dethInfos();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Info struct. */
	inline th_info &GetInfo(){ return pInfo; }
	
	/** \brief Comment struct. */
	inline th_comment &GetComment(){ return pComment; }
	
	/** \brief Setup pointer. */
	inline th_setup_info *GetSetupInfo() const{ return pSetupInfo; }
	
	/** \brief Set setup pointer. */
	void SetSetupInfo( th_setup_info *info );
	
	/** \brief Header finished reading. */
	inline bool GetHeaderFinished() const{ return pHeaderFinished; }
	
	/** \brief Set if header finished reading. */
	void SetHeaderFinished( bool headerFinished );
	
	
	
	/** \brief Audio info struct. */
	inline vorbis_info &GetAudioInfo(){ return pAudioInfo; }
	
	/** \brief Audio comment struct. */
	inline vorbis_comment &GetAudioComment(){ return pAudioComment; }
	
	/** \brief Audio header finished reading. */
	inline bool GetAudioHeaderFinished() const{ return pAudioHeaderFinished; }
	
	/** \brief Set if audio header finished reading. */
	void SetAudioHeaderFinished( bool headerFinished );
	/*@}*/
};

#endif
