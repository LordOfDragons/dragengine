/* 
 * Drag[en]gine Windows Launcher
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#pragma once

#include <memory>
#include <string>
#include "framework.h"

class WindowSplash;
class LauncherIni;

/** Launcher application. */
class Launcher{
private:
	const HINSTANCE pInstance;
	std::unique_ptr<WindowSplash> pWindowSplash;
	std::wstring pLauncherDirectory;
	std::unique_ptr<LauncherIni> pLauncherIni;

	static Launcher *pTheLauncher;

public:
	/** Create launcher. */
	Launcher(HINSTANCE hInstance);

	/** Clean up launcher. */
	~Launcher();

	/** Run. */
	int Run();

	/** App. */
	static inline Launcher &Get(){ return *pTheLauncher; };

	/** Application instance. */
	inline HINSTANCE GetInstance() const{return pInstance;}

	/** String to WString. */
	static std::wstring ToWString(const std::string &string);

	/** WString to String. */
	static std::string ToString(const std::wstring &string);

private:
	int pRunMessageLoop();
	void pExitApplication();
	void pLaunchDelga();
};
