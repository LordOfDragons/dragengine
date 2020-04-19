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
 * \brief OpenAL developer mode.
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
	bool pShowAudioModels;
	int pCaptureSpeakerDirectClosest;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create developer mode. */
	deoalDevMode( deAudioOpenAL &audioThread );
	
	/** \brief Clean up developer mode. */
	~deoalDevMode();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Developer mode is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Log calculation of environment probe calculation. */
	inline bool GetLogCalcEnvProbe() const{ return pLogCalcEnvProbe; }
	
	/** \brief Show module information. */
	inline bool GetShowModuleInfo() const{ return pShowModuleInfo; }
	
	/** \brief Show speaker environment overlay information. */
	inline bool GetShowSpeakerEnvInfo() const{ return pShowSpeakerEnvInfo; }
	
	/** \brief Show only speaker environment overlay information at position. */
	inline bool GetShowSpeakerEnvInfoAt() const{ return pShowSpeakerEnvInfoAt; }
	
	/** \brief Show environment overlay information position. */
	inline const decDVector &GetShowSpeakerEnvInfoAtPosition() const{ return pShowSpeakerEnvInfoAtPosition; }
	
	/** \brief Show closest speaker environment overlay information. */
	inline int GetShowSpeakerEnvInfoClosest() const{ return pShowSpeakerEnvInfoClosest; }
	
	/** \brief Show active microphone information. */
	inline bool GetShowActiveMicInfo() const{ return pShowActiveMicInfo; }
	
	/** \brief Request capturing microphone sound rays once. */
	inline bool GetCaptureMicRays() const{ return pCaptureMicRays; }
	
	/** \brief Clear request capturing microphone sound rays once. */
	void ClearCaptureMicRays();
	
	/** \brief Capturing microphone sound rays using XRay . */
	inline bool GetCaptureMicRaysXRay() const{ return pCaptureMicRaysXRay; }
	
	/** \brief Show audio models . */
	inline bool GetShowAudioModels() const{ return pShowAudioModels; }
	
	/** \brief Capture speaker closest direct. */
	inline int GetCaptureSpeakerClosestDirect() const{ return pCaptureSpeakerDirectClosest; }
	
	/** \brief Clear capture speaker closest direct. */
	void ClearCaptureSpeakerClosestDirect();
	
	
	
	/** \brief Run command. */
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
	void pCmdCaptureSpeakerDirectClosest( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pActiveWorldNotifyDevModeChanged();
};

#endif
