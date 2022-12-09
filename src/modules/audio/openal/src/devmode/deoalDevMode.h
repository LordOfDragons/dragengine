/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALDEVMODE_H_
#define _DEOALDEVMODE_H_

#include <dragengine/common/math/decMath.h>

class deAudioOpenAL;
class decUnicodeArgumentList;
class decUnicodeString;



/**
 * OpenAL developer mode.
 */
class deoalDevMode{
private:
	deAudioOpenAL &pOal;
	bool pEnabled;
	
	bool pLogCalcEnvProbe;
	bool pShowModuleInfo;
	bool pShowSpeakerEnvInfo;
	bool pShowSpeakerEnvInfoAt;
	decDVector pShowSpeakerEnvInfoAtPosition;
	int pShowSpeakerEnvInfoClosest;
	bool pShowActiveMicInfo;
	bool pCaptureMicRays;
	bool pCaptureMicRaysXRay;
	bool pCaptureMicRaysVolume;
	bool pShowAudioModels;
	int pCaptureSpeakerDirectClosest;
	int pVisualizeAudibleSpeakers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create developer mode. */
	deoalDevMode( deAudioOpenAL &audioThread );
	
	/** Clean up developer mode. */
	~deoalDevMode();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Developer mode is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** Log calculation of environment probe calculation. */
	inline bool GetLogCalcEnvProbe() const{ return pLogCalcEnvProbe; }
	
	/** Show module information. */
	inline bool GetShowModuleInfo() const{ return pShowModuleInfo; }
	
	/** Show speaker environment overlay information. */
	inline bool GetShowSpeakerEnvInfo() const{ return pShowSpeakerEnvInfo; }
	
	/** Show only speaker environment overlay information at position. */
	inline bool GetShowSpeakerEnvInfoAt() const{ return pShowSpeakerEnvInfoAt; }
	
	/** Show environment overlay information position. */
	inline const decDVector &GetShowSpeakerEnvInfoAtPosition() const{ return pShowSpeakerEnvInfoAtPosition; }
	
	/** Show closest speaker environment overlay information. */
	inline int GetShowSpeakerEnvInfoClosest() const{ return pShowSpeakerEnvInfoClosest; }
	
	/** Show active microphone information. */
	inline bool GetShowActiveMicInfo() const{ return pShowActiveMicInfo; }
	
	/** Request capturing microphone sound rays once. */
	inline bool GetCaptureMicRays() const{ return pCaptureMicRays; }
	
	/** Clear request capturing microphone sound rays once. */
	void ClearCaptureMicRays();
	
	/** Capturing microphone sound rays using XRay . */
	inline bool GetCaptureMicRaysXRay() const{ return pCaptureMicRaysXRay; }
	
	/** Capturing microphone sound rays using Volume. */
	inline bool GetCaptureMicRaysVolume() const{ return pCaptureMicRaysVolume; }
	
	/** Show audio models . */
	inline bool GetShowAudioModels() const{ return pShowAudioModels; }
	
	/** Capture speaker closest direct. */
	inline int GetCaptureSpeakerClosestDirect() const{ return pCaptureSpeakerDirectClosest; }
	
	inline int GetVisualizeAudibleSpeakers() const{ return pVisualizeAudibleSpeakers; }
	
	/** Clear capture speaker closest direct. */
	void ClearCaptureSpeakerClosestDirect();
	
	
	
	/** Run command. */
	bool ExecuteCommand( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/*@}*/
	
	
	
private:
	void pCmdHelp( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdEnable( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pCmdLogCalcEnvProbe( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowModuleInfo( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowSpeakerEnvInfo( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowSpeakerEnvInfoAt( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowSpeakerEnvInfoClosest( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowActiveMicInfo( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdCaptureMicRays( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowAudioModels( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdVisualizeAudibleSpeakers( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdCaptureSpeakerDirectClosest( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pActiveWorldNotifyDevModeChanged();
};

#endif
