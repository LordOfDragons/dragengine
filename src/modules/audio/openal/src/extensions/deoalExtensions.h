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
		ext_ALC_EXT_debug
	};
	
	static const int EXT_COUNT = ext_ALC_EXT_debug + 1;
	
	
	
private:
	deoalAudioThread &pAudioThread;
	
	int pVersionMajor;
	int pVersionMinor;
	decStringList pStrListExtensions;
	
	bool pHasExtension[EXT_COUNT];
	bool pDisableExtension[EXT_COUNT];
	
	bool pHasRequiredFunctions;
	bool pHasEFX;
	bool pHasHRTF;
	
	int pEfxVersionMajor;
	int pEfxVersionMinor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create extensions. */
	deoalExtensions(deoalAudioThread &audioThread);
	
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
	inline int GetVersionMajor() const{return pVersionMajor;}
	
	/** Minor version. */
	inline int GetVersionMinor() const{return pVersionMinor;}
	
	/** List of extension strings supported by hardware. */
	inline const decStringList &GetStringListExtensions() const{return pStrListExtensions;}
	
	
	
	/** Extensions is supported. */
	bool GetHasExtension(eExtensions extension) const;
	
	/** Name of extension. */
	const char *GetExtensionName(eExtensions extension) const;
	
	/** Disable extension. */
	void DisableExtension(eExtensions extension);
	
	
	
	/** EFX major version. */
	inline int GetEfxVersionMajor() const{return pEfxVersionMajor;}
	
	/** EFX minor version. */
	inline int GetEfxVersionMinor() const{return pEfxVersionMinor;}
	
	
	
	/** EFX is supported. */
	inline bool GetHasEFX() const{return pHasEFX;}
	
	/** HRTF is supported. */
	inline bool GetHasHRTF() const{return pHasHRTF;}
	/*@}*/
	
	
	
private:
	void pScanVersion();
	void pDisableExtensions();
	
	void pFetchRequiredFunctions();
	void pFetchOptionalFunctions();
	
	void pGetRequiredFunction(void **funcPointer, const char *funcName);
	void pGetOptionalFunction(void **funcPointer, const char *funcName, int extensionIndex);
	void pGetOptionalFunction(void **funcPointer, const char *funcName,
		const char *funcName2, int extensionIndex);
};

#endif
