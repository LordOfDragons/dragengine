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

#ifndef _DEBASEAUDIOMICROPHONE_H_
#define _DEBASEAUDIOMICROPHONE_H_

#include "../../../dragengine_export.h"

class deSpeaker;



/**
 * \brief Audio Module Microphone Peer.
 */
class DE_DLL_EXPORT deBaseAudioMicrophone{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseAudioMicrophone();
	
	/** \brief Clean up peer. */
	virtual ~deBaseAudioMicrophone();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Type changed. */
	virtual void TypeChanged();
	
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Velocity changed. */
	virtual void VelocityChanged();
	
	/** \brief Muted changed. */
	virtual void MutedChanged();
	
	/** \brief Volume changed. */
	virtual void VolumeChanged();
	
	/** \brief Speaker gain changed. */
	virtual void SpeakerGainChanged();
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	
	/** \brief Speaker has been added. */
	virtual void SpeakerAdded( deSpeaker *speaker );
	
	/** \brief Speaker has been removed. */
	virtual void SpeakerRemoved( deSpeaker *speaker );
	
	/** \brief All speakers have been removed. */
	virtual void AllSpeakersRemoved();
	/*@}*/
};

#endif
