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

#include <algorithm>

#include "projRemoteClient.h"
#include "projRemoteClientLogger.h"
#include "projRemoteClientListener.h"
#include "projRemoteClientTaskProcessor.h"
#include "../projProject.h"
#include "../profile/projProfile.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/engine/igdeEngineController.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/threading/deMutexGuard.h>


// Class projRemoteClient
///////////////////////////

// Constructor, destructor
////////////////////////////

projRemoteClient::projRemoteClient(projProject &project, derlServer &server,
	const derlRemoteClientConnection::Ref &connection) :
derlRemoteClient(server, connection),
pProject(project)
{
	// store path to log file to use
	decString filename;
	filename.Format("testRun-%s.log", GetName().c_str());
	
	const int last = filename.GetLength() - 4;
	int i;
	for(i=8; i<last; i++){
		if(!isalnum(filename[i])){
			filename[i] = '_';
		}
	}
	
	decPath path;
	path.SetFromNative(project.GetEnvironment()->GetGameProject()->GetDirectoryPath());
	path.AddUnixPath(filename);
	
	pPathLogFile = path.GetPathNative();
	
	decDiskFileWriter::Ref::New(pPathLogFile, false); // clear file
	
	pLogFileReader = decDiskFileReader::Ref::New(pPathLogFile);
	
	SetLogger(std::make_shared<projRemoteClientLogger>("RemoteClient", pPathLogFile));
}

projRemoteClient::~projRemoteClient(){
	pListeners.RemoveAll();
}


// Management
///////////////

projRemoteClient::Ref projRemoteClient::GetRefInServer() const{
	const derlRemoteClient::List &clients = GetServer().GetClients();
	const derlRemoteClient::List::const_iterator iter = std::find_if(
		clients.cbegin(), clients.cend(), [&](const derlRemoteClient::Ref &each){
			return each.get() == this;
		});
	return iter != clients.cend() ? std::static_pointer_cast<projRemoteClient>(*iter) : nullptr;
}

decString projRemoteClient::ReadNextLogData(){
	const int position = pLogFileReader->GetPosition();
	pLogFileReader->SetPositionEnd(0);
	const int end = pLogFileReader->GetPosition();
	
	decString content;
	
	if(end > position){
		content.Set(' ', end - position);
		pLogFileReader->SetPosition(position);
		pLogFileReader->Read(content.GetMutableString(), end - position);
	}
	
	return content;
}

decString projRemoteClient::GetLastLogContent(){
	try{
		const decDiskFileReader::Ref reader(decDiskFileReader::Ref::New(pPathLogFile));
		
		const int length = reader->GetLength();
		if(length == 0){
			return decString();
		}
		
		decString content;
		content.Set(' ', length);
		reader->Read(content.GetMutableString(), length);
		return content;
		
	}catch(const deException &){
		// ignore errors. treat this as if file could not be found
		return decString();
	}
}

derlTaskProcessorRemoteClient::Ref projRemoteClient::CreateTaskProcessor(){
	return std::make_shared<projRemoteClientTaskProcessor>(*this);
}


void projRemoteClient::StartApplication(){
	projProfile * const profile = pProject.GetActiveProfile();
	DEASSERT_NOTNULL(profile)
	
	derlRunParameters params;
	params.SetProfileName(pActiveLaunchProfile.GetString());
	
	pBuildGameXml(params, *profile);
	
	try{
		derlRemoteClient::StartApplication(params);
		
	}catch(const std::exception &e){
		DETHROW_INFO(deeInvalidAction, e.what());
	}
}


bool projRemoteClient::IsRunning(){
	return GetRunStatus() != RunStatus::stopped;
}

void projRemoteClient::StopApplication(){
	try{
		derlRemoteClient::StopApplication();
		
	}catch(const std::exception &e){
		DETHROW_INFO(deeInvalidAction, e.what());
	}
}

void projRemoteClient::KillApplication(){
	try{
		derlRemoteClient::KillApplication();
		
	}catch(const std::exception &e){
		DETHROW_INFO(deeInvalidAction, e.what());
	}
}

void projRemoteClient::ProcessReceivedSystemProperties(){
	ListSysProps props;
	{
	const std::lock_guard<std::mutex> guard(GetMutex());
	props = pReceivedSysProps;
	pReceivedSysProps.clear();
	}
	
	for(const sSystemProperty &each : props){
		if(each.property == derlProtocol::SystemPropertyNames::profileNames){
			decStringSet profiles;
			std::stringstream ss(each.value);
			std::string s;
			while(std::getline(ss, s, '\n')){
				profiles.Add(s.c_str());
			}
			SetLaunchProfiles(profiles);
			
		}else if(each.property == derlProtocol::SystemPropertyNames::defaultProfile){
			SetDefaultLaunchProfile(each.value.c_str());
			
			if(pActiveLaunchProfile.IsEmpty()){
				SetActiveLaunchProfile(pDefaultLaunchProfile);
			}
		}
	}
}

void projRemoteClient::DropProfileData(){
	SetFileLayoutServer(nullptr);
}

void projRemoteClient::OnConnectionEstablished(){
	const projRemoteClient::Ref ref(GetRefInServer());
	if(ref){
		pProject.NotifyRemoteClientConnected(ref);
	}
}

void projRemoteClient::OnConnectionClosed(){
	NotifyClientDisconnected();
}

void projRemoteClient::OnSynchronizeBegin(){
	std::stringstream ss;
	ss << "Begin synchronization of client '" << GetName() << "'.";
	GetLogger()->Log(denLogger::LogSeverity::info, ss.str());
}

void projRemoteClient::OnSynchronizeUpdate(){
}

void projRemoteClient::OnSynchronizeFinished(){
	std::stringstream ss;
	ss << "Finished synchronization of client '" << GetName() << "'.";
	GetLogger()->Log(denLogger::LogSeverity::info, ss.str());
}

void projRemoteClient::OnRunStatusChanged(){
	std::stringstream ss;
	ss << "Run status changed of client '" << GetName() << "' to ";
	
	switch(GetRunStatus()){
	case RunStatus::running:
		ss << "'Running'";
		break;
		
	case RunStatus::stopped:
		ss << "'Stopped'";
		break;
	}
	
	ss << ".";
	
	GetLogger()->Log(denLogger::LogSeverity::info, ss.str());
}

void projRemoteClient::OnSystemProperty(const std::string &property, const std::string &value){
	const std::lock_guard<std::mutex> guard(GetMutex());
	pReceivedSysProps.push_back({property, value});
}


decStringSet projRemoteClient::GetLaunchProfiles(){
	const std::lock_guard<std::mutex> guard(GetMutex());
	return pLaunchProfiles;
}

void projRemoteClient::SetLaunchProfiles(const decStringSet &profiles){
	bool profilesChanged = false;
	bool activeChanged = false;
	
	{
	const std::lock_guard<std::mutex> guard(GetMutex());
	if(profiles == pLaunchProfiles){
		return;
	}
	
	pLaunchProfiles = profiles;
	profilesChanged = true;
	
	if(!pActiveLaunchProfile.IsEmpty() && !pLaunchProfiles.Has(pActiveLaunchProfile)){
		pActiveLaunchProfile = "";
		activeChanged = true;
	}
	}
	
	if(profilesChanged){
		NotifyLaunchProfilesChanged();
	}
	if(activeChanged){
		NotifyActiveLaunchProfileChanged();
	}
}

decString projRemoteClient::GetActiveLaunchProfile(){
	const std::lock_guard<std::mutex> guard(GetMutex());
	return pActiveLaunchProfile;
}

void projRemoteClient::SetActiveLaunchProfile(const decString &profile){
	{
	const std::lock_guard<std::mutex> guard(GetMutex());
	if(profile == pActiveLaunchProfile){
		return;
	}
	pActiveLaunchProfile = profile;
	}
	
	NotifyActiveLaunchProfileChanged();
}

decString projRemoteClient::GetDefaultLaunchProfile(){
	const std::lock_guard<std::mutex> guard(GetMutex());
	return pDefaultLaunchProfile;
}

void projRemoteClient::SetDefaultLaunchProfile(const decString &profile){
	{
	const std::lock_guard<std::mutex> guard(GetMutex());
	if(profile == pDefaultLaunchProfile){
		return;
	}
	pDefaultLaunchProfile = profile;
	}
	
	NotifyActiveLaunchProfileChanged();
}


// Listeners
//////////////

void projRemoteClient::AddListener(projRemoteClientListener *listener){
	pListeners.Add(listener);
}

void projRemoteClient::RemoveListener(projRemoteClientListener *listener){
	pListeners.Remove(listener);
}


void projRemoteClient::NotifyClientChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		static_cast<projRemoteClientListener*>(pListeners.GetAt(i))->ClientChanged(this);
	}
}

void projRemoteClient::NotifyClientDisconnected(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		static_cast<projRemoteClientListener*>(pListeners.GetAt(i))->ClientDisconnected(this);
	}
}

void projRemoteClient::NotifyLaunchProfilesChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		static_cast<projRemoteClientListener*>(pListeners.GetAt(i))->LaunchProfilesChanged(this);
	}
}

void projRemoteClient::NotifyActiveLaunchProfileChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((projRemoteClientListener*)pListeners.GetAt(i))->ActiveLaunchProfileChanged(this);
	}
}


// Private Functions
//////////////////////

void projRemoteClient::pBuildGameXml(derlRunParameters &params, const projProfile &profile){
	const decMemoryFile::Ref file(decMemoryFile::Ref::New("run.degame"));
	
	{
	decXmlWriter xmlWriter(decMemoryFileWriter::Ref::New(file, false));
	pBuildGameXml(xmlWriter, profile);
	}
	
	std::string gameConfig;
	gameConfig.assign(file->GetPointer(), file->GetLength());
	params.SetGameConfig(gameConfig);
}

void projRemoteClient::pBuildGameXml(decXmlWriter &writer, const projProfile &profile){
	writer.WriteXMLDeclaration();
	
	writer.WriteOpeningTag("degame");
	
	writer.WriteDataTagString("identifier", profile.GetIdentifier().ToHexString(false));
	writer.WriteDataTagString("title", profile.GetTitle());
	writer.WriteDataTagString("description", profile.GetGameDescription());
	writer.WriteDataTagString("creator", profile.GetCreator());
	writer.WriteDataTagString("homepage", profile.GetWebsite());
	
	igdeEnvironment &env = *pProject.GetEnvironment();
	const decStringSet &iconPathList = profile.GetIcons();
	const int iconPathCount = iconPathList.GetCount();
	if(iconPathCount > 0){
		deImageManager &imageManager = *env.GetEngineController()->GetEngine()->GetImageManager();
		int i;
		for(i=0; i<iconPathCount; i++){
			const deImage::Ref icon(imageManager.LoadImage(iconPathList.GetAt(i), "/"));
			writer.WriteOpeningTagStart("icon");
			writer.WriteAttributeInt("size", icon->GetWidth());
			writer.WriteOpeningTagEnd(false, false);
			writer.WriteTextString(iconPathList.GetAt(i));
			writer.WriteClosingTag("icon", false);
		}
	}
	
	writer.WriteDataTagString("scriptDirectory", profile.GetScriptDirectory());
	writer.WriteDataTagString("gameObject", profile.GetGameObject());
	writer.WriteDataTagString("pathConfig", profile.GetPathConfig());
	writer.WriteDataTagString("pathCapture", profile.GetPathCapture());
	
	writer.WriteOpeningTagStart("scriptModule");
	if(!env.GetGameProject()->GetScriptModuleVersion().IsEmpty()){
		writer.WriteAttributeString("version", env.GetGameProject()->GetScriptModuleVersion());
	}
	writer.WriteOpeningTagEnd(false, false);
	writer.WriteTextString(env.GetGameProject()->GetScriptModule());
	writer.WriteClosingTag("scriptModule", false);
	
	const decPoint &windowSize = profile.GetWindowSize();
	if(windowSize != decPoint()){
		writer.WriteOpeningTagStart("windowSize");
		writer.WriteAttributeInt("x", windowSize.x);
		writer.WriteAttributeInt("y", windowSize.y);
		writer.WriteOpeningTagEnd(true);
	}
	
	writer.WriteClosingTag("degame");
}
