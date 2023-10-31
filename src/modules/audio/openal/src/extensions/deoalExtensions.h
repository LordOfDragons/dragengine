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

#ifndef _DEOALEXTENSIONS_H_
#define _DEOALEXTENSIONS_H_

#include <dragengine/common/string/decStringList.h>

class deoalAudioThread;



/**
 * OpenAL Extensions.
 */
class deoalExtensions{
public:
	/** Extensions. */
	enum eExtensions{
		ext_ALC_EXT_EFX,
		ext_ALC_SOFT_HRTF,
	};
	
	static const int EXT_COUNT = ext_ALC_SOFT_HRTF + 1;
	
	
	
private:
	deoalAudioThread &pAudioThread;
	
	int pVersionMajor;
	int pVersionMinor;
	decStringList pStrListExtensions;
	
	bool pHasExtension[ EXT_COUNT ];
	bool pDisableExtension[ EXT_COUNT ];
	
	bool pHasRequiredFunctions;
	bool pHasEFX;
	bool pHasHRTF;
	
	int pEfxVersionMajor;
	int pEfxVersionMinor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create extensions. */
	deoalExtensions( deoalAudioThread &audioThread );
	
	/** Clean up extensions. */
	~deoalExtensions();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Scan device extensions. */
	void ScanDeviceExtensions();
	
	/** Scan context extensions. */
	void ScanContextExtensions();
	
	/** Print summary of findings. */
	void PrintSummary();
	
	/** Returns true if all required extensions are present. */
	bool VerifyPresence();
	
	
	
	/** Major version. */
	inline int GetVersionMajor() const{ return pVersionMajor; }
	
	/** Minor version. */
	inline int GetVersionMinor() const{ return pVersionMinor; }
	
	/** List of extension strings supported by hardware. */
	inline const decStringList &GetStringListExtensions() const{ return pStrListExtensions; }
	
	
	
	/** Extensions is supported. */
	bool GetHasExtension( eExtensions extension ) const;
	
	/** Name of extension. */
	const char *GetExtensionName( eExtensions extension ) const;
	
	/** Disable extension. */
	void DisableExtension( eExtensions extension );
	
	
	
	/** EFX major version. */
	inline int GetEfxVersionMajor() const{ return pEfxVersionMajor; }
	
	/** EFX minor version. */
	inline int GetEfxVersionMinor() const{ return pEfxVersionMinor; }
	
	
	
	/** EFX is supported. */
	inline bool GetHasEFX() const{ return pHasEFX; }
	
	/** HRTF is supported. */
	inline bool GetHasHRTF() const{ return pHasHRTF; }
	/*@}*/
	
	
	
private:
	void pScanVersion();
	void pDisableExtensions();
	
	void pFetchRequiredFunctions();
	void pFetchOptionalFunctions();
	
	void pGetRequiredFunction( void **funcPointer, const char *funcName );
	void pGetOptionalFunction( void **funcPointer, const char *funcName, int extensionIndex );
};

#endif
