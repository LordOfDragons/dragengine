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

#ifndef _PROJTESTRUNNER_H_
#define _PROJTESTRUNNER_H_

#include <stdio.h>

#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#endif
#include <dragengine/common/string/decString.h>
#include <dragengine/common/file/decBaseFileReader.h>

#include "profile/projTRProfileList.h"

class projWindowMain;
class projProfile;



/**
 * \brief Test runner.
 */
class projTestRunner{
private:
	projWindowMain &pWindowMain;
	
	projTRProfileList pLauncherProfiles;
	decString pDefaultLauncherProfileName;
	
	projProfile *pProfile;
	projTRProfile *pLauncherProfile;
	
	decString pPathLogFile;
	decBaseFileReader::Ref pLogFileReader;
	
	#ifdef OS_W32
	HANDLE pPipeIn;
	HANDLE pPipeOut;
	HANDLE pProcessHandle;
	HANDLE pThreadHandle;
	DWORD pProcessID;
	DWORD pThreadID;
	
	#else
	int pPipeIn;
	int pPipeOut;
	pid_t pProcessID;
	#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create test runner. */
	projTestRunner(projWindowMain &windowMain);
	
	/** \brief Clean up test runner. */
	~projTestRunner();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline projWindowMain &GetWindowMain() const{return pWindowMain;}
	
	/** \brief Profile. */
	inline const projProfile *GetProfile() const{return pProfile;}
	
	/** \brief Load engine configuration. */
	void LoadEngineConfiguration();
	
	/** \brief Launcher profiles. */
	inline projTRProfileList &GetLauncherProfiles(){return pLauncherProfiles;}
	inline const projTRProfileList &GetLauncherProfiles() const{return pLauncherProfiles;}
	
	/** \brief Default launcher profile name. */
	inline const decString &GetDefaultLauncherProfileName() const{return pDefaultLauncherProfileName;}
	
	/** \brief Set fefault launcher profile name. */
	void SetDefaultLauncherProfileName(const char *name);
	
	/** \brief Launcher profile. */
	inline const projTRProfile *GetLauncherProfile() const{return pLauncherProfile;}
	
	/** \brief Is running. */
	bool IsRunning();
	
	/** \brief Start. */
	void Start(projProfile *profile, projTRProfile *launcherProfile);
	
	/** \brief Stop. */
	void Stop();
	
	/** \brief Kill. */
	void Kill();
	
	
	
	/** \brief Write unsigned char to pipe. */
	void WriteUCharToPipe(int value);
	
	/** \brief Write unsigned short to pipe. */
	void WriteUShortToPipe(int value);
	
	/** \brief Write float to pipe. */
	void WriteFloatToPipe(float value);
	
	/** \brief Write string to pipe using unsigned short as length. */
	void WriteString16ToPipe(const char *string);
	
	/** \brief Write data to pipe. */
	void WriteToPipe(const void *data, int length);
	
	/** \brief Read unsigned char from pipe. */
	int ReadUCharFromPipe();
	
	/** \brief Read unsigned short from pipe. */
	int ReadUShortFromPipe();
	
	/** \brief Read float from pipe. */
	float ReadFloatFromPipe();
	
	/** \brief Read string from pipe using unsigned short as length. */
	void ReadString16FromPipe(decString &string);
	
	/** \brief Read data from pipe. */
	void ReadFromPipe(void *data, int length);
	
	
	
	/** \brief Send quit request. */
	void SendQuit();
	
	/** \brief Read next log data if available or empty string otherwise. */
	decString ReadNextLogData();
	
	/** \brief Last log file content if present. */
	decString GetLastLogContent();
	/*@}*/
	
	
	
private:
	void pInitLogFile();
	void pSendLaunchParameters();
};

#endif
