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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "deoglCommandExecuter.h"
#include "../deGraphicOpenGl.h"
#include "../configuration/deoglConfiguration.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../extensions/deoglExtensions.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglFramebufferManager.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../skin/deoglSkinPropertyMap.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>



// Class deoglCommandExecuter
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCommandExecuter::deoglCommandExecuter(deGraphicOpenGl &ogl) :
pOgl(ogl){
}

deoglCommandExecuter::~deoglCommandExecuter(){
}



// Management
///////////////

void deoglCommandExecuter::ExecuteCommand(const decUnicodeArgumentList &command, decUnicodeString &answer){
	if(command.GetArgumentCount() == 0){
		answer.SetFromUTF8("No command provided.");
		
	}else{
		if(command.MatchesArgumentAt(0, "help")){
			pHelp(command, answer);
			
		}else if(command.MatchesArgumentAt(0, "extensions")){
			pExtensions(command, answer);
			
		}else if(command.MatchesArgumentAt(0, "renderWindow")){
			pRenderWindow(command, answer);
			
		}else if(command.MatchesArgumentAt(0, "visual")){
			pVisual(command, answer);
			
		}else if(command.MatchesArgumentAt(0, "fboInfos")){
			pFBOInfos(command, answer);
			
		}else if(command.MatchesArgumentAt(0, "fixNaN")){
			pFixNaN(command, answer);
			
		}else if(pOgl.HasRenderThread() && !pOgl.GetRenderThread().GetDebug().GetDeveloperMode().ExecuteCommand(command, answer)){
			answer.SetFromUTF8("Unknown command '");
			answer += *command.GetArgumentAt(0);
			answer.AppendFromUTF8("'.");
		}
	}
}



// Private functions
//////////////////////

void deoglCommandExecuter::pHelp(const decUnicodeArgumentList &command, decUnicodeString &answer){
	answer.SetFromUTF8("help => Displays this help screen.\n");
	answer.AppendFromUTF8("extensions => Lists status of extensions.\n");
	answer.AppendFromUTF8("renderWindow => Shows information about the render window.\n");
	answer.AppendFromUTF8("visual => Shows information about the visual.\n");
	answer.AppendFromUTF8("fboInfos => Shows FBO Information.");
}

void deoglCommandExecuter::pExtensions(const decUnicodeArgumentList &command, decUnicodeString &answer){
	const deoglExtensions &extensions = pOgl.GetRenderThread().GetExtensions();
	const decStringList &extensionList = extensions.GetStringListExtensions();
	const int extensionCount = extensionList.GetCount();
	decString text;
	int i;
	
	pAnswerStringValue(answer, "OpenGL Vendor", extensions.GetStringVendor().GetString());
	pAnswerStringValue(answer, "OpenGL Version", extensions.GetStringGLVersion().GetString());
	answer.AppendFromUTF8("Extensions:\n");
	for(i=0; i<extensionCount; i++){
		answer.AppendFromUTF8(extensionList.GetAt(i).GetString());
		answer.AppendCharacter('\n');
	}
}

void deoglCommandExecuter::pRenderWindow(const decUnicodeArgumentList &command, decUnicodeString &answer){
#if 0
	deoglRRenderWindow * const renderWindow = pOgl.GetActiveRRenderWindow();
	
	if(renderWindow){
		//answer.SetFromUTF8( "Active Render Window Information.\n" );
		pAnswerIntValue(answer, "Width", renderWindow->GetWidth());
		pAnswerIntValue(answer, "Height", renderWindow->GetHeight());
		pAnswerStringValue(answer, "Window Title", renderWindow->GetTitle());
		pAnswerIntValue(answer, "HostWindow", (int)(intptr_t)renderWindow->GetHostWindow());
		pAnswerIntValue(answer, "Window", (int)(intptr_t)renderWindow->GetWindow());
		//pAnswerBoolValue( answer, "Full Screen", renderWindow->GetFullScreen() );
		//pAnswerBoolValue( answer, "Notify script on resize", renderWindow->GetNotifyScriptOnResize() );
		
	}else{
		answer.SetFromUTF8("No render window is set");
	}
#endif
}

void deoglCommandExecuter::pVisual(const decUnicodeArgumentList &command, decUnicodeString &answer){
	answer.SetFromUTF8("Visual Information.\n");
	answer.AppendFromUTF8("< No information available >\n");
}

void deoglCommandExecuter::pFBOInfos(const decUnicodeArgumentList &command, decUnicodeString &answer){
	deoglRenderThread &renderThread = pOgl.GetRenderThread();
	
	renderThread.Freeze();
	
	try{
		deoglFramebufferManager &fboMgr = renderThread.GetFramebuffer().GetManager();
		deoglConfiguration &config = pOgl.GetConfiguration();
		int f, count = fboMgr.GetFBOCount();
		
		answer.SetFromUTF8("FBO Information (useOneFBO=");
		if(config.GetUseOneFBO()){
			answer.AppendFromUTF8("yes):\n");
			
		}else{
			answer.AppendFromUTF8("no):\n");
		}
		
		for(f=0; f<count; f++){
			const deoglFramebuffer &fbo = fboMgr.GetFBOAt(f);
			
			answer.AppendFromUTF8("- ");
			answer.AppendValue(fbo.GetUsageWidth());
			answer.AppendFromUTF8(" x ");
			answer.AppendValue(fbo.GetUsageHeight());
			answer.AppendFromUTF8(": usage=");
			answer.AppendValue(fbo.GetUsageCount());
			answer.AppendFromUTF8("\n");
		}
		
		renderThread.Unfreeze();
		
	}catch(...){
		renderThread.Unfreeze();
		throw;
	}
}

void deoglCommandExecuter::pFixNaN(const decUnicodeArgumentList &command, decUnicodeString &answer){
#if 0
	deCameraManager *camMgr = pOgl.GetGameEngine()->GetCameraManager();
	deoglPixelBuffer pbToneMapParams(deoglPixelBuffer::epfFloat4, 1, 1, 1);
	deoglPixelBuffer::sFloat4 *dataToneMapParams = pbToneMapParams.GetPointerFloat4();
	deCamera *camera = camMgr->GetRootCamera();
	deoglCamera *oglCamera;
	
	dataToneMapParams[0].r = 0.5f;
	dataToneMapParams[0].g = 0.0f;
	dataToneMapParams[0].b = 0.0f;
	dataToneMapParams[0].a = 0.0f;
	
	while(camera){
		oglCamera = (deoglCamera*)camera->GetPeerGraphic();
		oglCamera->GetToneMapParamsTexture()->SetPixels(pbToneMapParams);
		camera = (deCamera*)camera->GetLLManagerNext();
	}
#endif
}



void deoglCommandExecuter::pAnswerBoolValue(decUnicodeString &answer, const char *name, bool value){
	answer.AppendFromUTF8(name);
	if(value){
		answer.AppendFromUTF8(" = Yes\n");
	}else{
		answer.AppendFromUTF8(" = No\n");
	}	
}

void deoglCommandExecuter::pAnswerIntValue(decUnicodeString &answer, const char *name, int value){
	answer.AppendFromUTF8(name);
	answer.AppendFromUTF8(" = ");
	answer.AppendValue(value);
	answer.AppendFromUTF8("\n");
}

void deoglCommandExecuter::pAnswerStringValue(decUnicodeString &answer, const char *name, const char *value){
	answer.AppendFromUTF8(name);
	answer.AppendFromUTF8(" = ");
	if(value){
		answer.AppendFromUTF8(value);
	}else{
		answer.AppendFromUTF8("< null >");
	}
	answer.AppendFromUTF8("\n");
}
