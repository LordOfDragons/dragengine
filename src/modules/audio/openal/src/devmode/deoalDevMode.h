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
