/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEWLLAUNCHERBINDINGS_H_
#define _DEWLLAUNCHERBINDINGS_H_

#include "dewlLauncher.h"

EMSCRIPTEN_BINDINGS(dragengine_web_launcher) {
	es::class_<dewlLauncher>("dewlLauncher")
		.constructor<>()
		.property("delgaPath", &dewlLauncher::GetDelgaPath, &dewlLauncher::SetDelgaPath)
		.property("profileName", &dewlLauncher::GetProfileName, &dewlLauncher::SetProfileName)
		.property("canvasId", &dewlLauncher::GetCanvasId, &dewlLauncher::SetCanvasId)
		.function("removeAllModuleParameters", &dewlLauncher::RemoveAllModuleParameters)
		.function("addModuleParameter", &dewlLauncher::AddModuleParameter)
		.function("runGame", &dewlLauncher::RunGame)
		.function("getGameProblems", &dewlLauncher::GetGameProblems)
		.function("getLocatedProfileName", &dewlLauncher::GetLocatedProfileName)
		.function("getProfileProblems", &dewlLauncher::GetProfileProblems)
		.function("getPatchProblems", &dewlLauncher::GetPatchProblems)
		;
	
	es::register_vector<std::string>("vector<string>");
	es::register_optional<std::string>();
}

#endif
