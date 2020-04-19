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
