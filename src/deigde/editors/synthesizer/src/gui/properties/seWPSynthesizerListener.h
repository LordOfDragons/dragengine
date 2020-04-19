/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#ifndef _SEWPSYNTHESIZERLISTENER_H_
#define _SEWPSYNTHESIZERLISTENER_H_

#include "../../synthesizer/seSynthesizerNotifier.h"

class seWPSynthesizer;



/**
 * \brief Synthesizer properties panel listener.
 */
class seWPSynthesizerListener : public seSynthesizerNotifier{
private:
	seWPSynthesizer &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWPSynthesizerListener( seWPSynthesizer &panel );
	
	/** \brief Clean up listener. */
	virtual ~seWPSynthesizerListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Synthesizer changed. */
	virtual void SynthesizerChanged( seSynthesizer *synthesizer );
	
	/** \brief Playback properties changed. */
	virtual void PlaybackChanged( seSynthesizer *synthesizer );
	/*@}*/
};

#endif
