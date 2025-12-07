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
#include <string.h>

#include "deglInstallDelga.h"
#include "deglWindowMain.h"
#include "../deglLauncher.h"
#include "../config/deglConfiguration.h"

#include <delauncher/engine/delEngineInstance.h>
#include <delauncher/game/patch/delPatch.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/logger/deLogger.h>



// Class deglInstallDelga
///////////////////////////

// Constructor, destructor
////////////////////////////

deglInstallDelga::deglInstallDelga(deglWindowMain &window) :
pWindow(window){
}

deglInstallDelga::~deglInstallDelga(){
}



// Management
///////////////

class cTaskInstallDelga : public FXRunnable {
private:
	deglLauncher &pLauncher;
	FXMessageChannel pMessageChannel;
	FXProgressDialog *pDialogProgress;
	bool pAbort;
	bool pFinished;
	const decString &pFilename;
	
public:
	cTaskInstallDelga (deglLauncher &launcher, FXProgressDialog *dialogProgress, const decString &filename) :
	pLauncher(launcher), pMessageChannel(FXApp::instance()), pDialogProgress(dialogProgress),
	pAbort(false), pFinished(false), pFilename(filename){
	};
	
	~cTaskInstallDelga(){
	};
	
	virtual FXint run(){
		char *buffer = nullptr;
		
		try{
			const deVFSDiskDirectory::Ref container(deVFSDiskDirectory::Ref::New(
				new deVFSDiskDirectory(decPath::CreatePathNative(pLauncher.GetPathGames()))));
			
			const decDiskFileReader::Ref reader(decDiskFileReader::Ref::New(
				new decDiskFileReader(pFilename)));
			
			decPath target(decPath::CreatePathUnix("/"));
			target.AddComponent(decPath::CreatePathNative(pFilename).GetLastComponent());
			
			decBaseFileWriter::Ref writer(decBaseFileWriter::Ref::New(
				container->OpenFileForWriting(target)));
			
			const int totalSize = reader->GetLength();
			const double percentageFactor = 100.0 / (double)totalSize;
			buffer = new char[8192];
			FXuint progressPercentage = 0;
			int bytesCopied = 0;
			
			while(bytesCopied < totalSize && ! pAbort){
				const FXuint percentage = (FXuint)(percentageFactor * bytesCopied);
				if(percentage != progressPercentage){
					progressPercentage = percentage;
					pMessageChannel.message(pDialogProgress,
						FXSEL(SEL_COMMAND, FXProgressDialog::ID_SETINTVALUE),
						&progressPercentage, sizeof(progressPercentage));
				}
				
				const int copyBytesCount = decMath::min(8192, totalSize - bytesCopied);
				reader->Read(buffer, copyBytesCount);
				writer->Write(buffer, copyBytesCount);
				
				bytesCopied += copyBytesCount;
			}
			
		}catch(const deException &e){
			pLauncher.GetLogger()->LogException("InstallDelga", e);
			
			if(buffer){
				delete [] buffer;
			}
			pDeleteTarget();
			
			pMessageChannel.message(pDialogProgress, FXSEL(SEL_COMMAND, FXProgressDialog::ID_CANCEL), nullptr);
			pFinished = true;
			return -1;
		}
		
		if(pAbort){
			pDeleteTarget();
			pFinished = true;
			return -1;
		}
		
		pMessageChannel.message(pDialogProgress, FXSEL(SEL_COMMAND, FXProgressDialog::ID_ACCEPT), nullptr);
		pFinished = true;
		return 0;
	}
	
	void WaitFinished(bool abort = false){
		if(pFinished){
			return;
		}
		
		pAbort = abort;
		while(! pFinished){
			FXThread::sleep(100000000); // 100ms
		}
	}
	
private:
	void pDeleteTarget(){
		try{
			deVFSDiskDirectory::Ref container(deVFSDiskDirectory::Ref::New(
				new deVFSDiskDirectory(decPath::CreatePathNative(pLauncher.GetPathGames()))));
			
			decPath target(decPath::CreatePathUnix("/"));
			target.AddComponent(decPath::CreatePathNative(pFilename).GetLastComponent());
			
			if(container->ExistsFile(target)){
				container->DeleteFile(target);
			}
			
		}catch(const deException &e){
			e.PrintError();
		}
	}
};

bool deglInstallDelga::Run(const char *forceFilename){
	// ask for file to install or use forced filename
	decString filename;
	
	if(forceFilename){
		filename = forceFilename;
		
	}else{
		FXFileDialog dialog(&pWindow, "Install DELGA");
		dialog.setPatternList("DELGA (*.delga)\nAll Files (*)");
		dialog.setCurrentPattern(0);
		if(! dialog.execute(PLACEMENT_OWNER)){
			return false;
		}
		
		filename = dialog.getFilename().text();
	}
	
	// load delga file
	deglLauncher &launcher = *pWindow.GetLauncher();
	delPatchList patches;
	delGameList games;
	
	try{
		const delEngineInstance::Ref instance(delEngineInstance::Ref::New(
			launcher.GetEngineInstanceFactory().CreateEngineInstance(
				launcher, launcher.GetEngine().GetLogFile())));
		instance->StartEngine();
		instance->LoadModules();
		
		launcher.GetGameManager().LoadGameFromDisk(instance, filename, games);
		launcher.GetPatchManager().LoadPatchFromDisk(instance, filename, patches);
		
	}catch(const deException &e){
		pWindow.DisplayException(e);
		return false;
	}
	
	// check if installed
	const int gameCount = games.GetCount();
	int i;
	for(i=0; i<gameCount; i++){
		if(launcher.GetGameManager().GetGames().HasWithID(games.GetAt(i)->GetIdentifier())){
			FXMessageBox::information(&pWindow, MBOX_OK, "Install DELGA",
				"Game '%s' is already installed", games.GetAt(i)->GetTitle().ToUTF8().GetString());
			return false;
		}
	}
	
	const int patchCount = patches.GetCount();
	for(i=0; i<patchCount; i++){
		if(launcher.GetPatchManager().GetPatches().HasWithID(patches.GetAt(i)->GetIdentifier())){
			FXMessageBox::information(&pWindow, MBOX_OK, "Install DELGA",
				"Patch '%s' is already installed", patches.GetAt(i)->GetName().ToUTF8().GetString());
			return false;
		}
	}
	
	// show what would be installed and ask to continue
	decString text("The following content will be installed:\n\n");
	for(i=0; i<games.GetCount(); i++){
		const delGame &game = *games.GetAt(i);
		text.AppendFormat("- Game '%s'\n", game.GetTitle().ToUTF8().GetString());
	}
	for(i=0; i<patches.GetCount(); i++){
		const delPatch &patch = *patches.GetAt(i);
		const delGame *game = launcher.GetGameManager().GetGames().GetWithID(patch.GetGameID());
		if(! game){
			game = games.GetWithID(patch.GetGameID());
		}
		text.AppendFormat("- Patch '%s' for game '%s'\n",
			patch.GetName().ToUTF8().GetString(),
			game ? game->GetTitle().ToUTF8().GetString() : "?");
	}
	
	text += "\nDo you want to continue?";
	if(FXMessageBox::question(&pWindow, MBOX_YES_NO, "Install DELGA", "%s", text.GetString()) != MBOX_CLICKED_YES){
		return false;
	}
	
	// install delga file
	{
	FXProgressDialog dialogProgress(&pWindow, "Install DELGA", "Installing", PROGRESSDIALOG_NORMAL);
	dialogProgress.setTotal(100);
	
	cTaskInstallDelga task(launcher, &dialogProgress, filename);
	FXWorker::execute(&task);
	if(! dialogProgress.execute(PLACEMENT_OWNER)){
		task.WaitFinished(true);
		return false;
	}
	task.WaitFinished();
	}
	
	FXMessageBox::information(&pWindow, MBOX_OK, "Install DELGA", "Installed successfully");
	return true;
}
