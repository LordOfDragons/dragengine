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

#ifndef _PROJREMOTECLIENT_H_
#define _PROJREMOTECLIENT_H_

#include <unordered_set>

#include <deremotelauncher/derlRemoteClient.h>

#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>

class projProject;
class projProfile;
class projRemoteClientListener;

class decXmlWriter;


/**
 * \brief Remote connection client.
 */
class projRemoteClient : public derlRemoteClient{
public:
	typedef std::shared_ptr<projRemoteClient> Ref;
	typedef std::unordered_set<Ref> Set;
	
	struct sSystemProperty{
		std::string property, value;
	};
	
	
private:
	typedef std::vector<sSystemProperty> ListSysProps;
	
	projProject &pProject;
	
	decString pPathLogFile;
	decBaseFileReader::Ref pLogFileReader;
	
	decStringSet pLaunchProfiles;
	decString pActiveLaunchProfile;
	decString pDefaultLaunchProfile;
	
	decObjectSet pListeners;
	
	ListSysProps pReceivedSysProps;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create remote server. */
	projRemoteClient(projProject &project, derlServer &server,
		const derlRemoteClientConnection::Ref &connection);
	
	/** \brief Clean up remote server. */
	~projRemoteClient() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Project. */
	inline projProject &GetProject() const{ return pProject; }
	
	/** \brief Reference in parent server or nullptr if not found. */
	Ref GetRefInServer() const;
	
	
	/** \brief Path to log file. */
	inline const decString &GetPathLogFile() const{ return pPathLogFile; }
	
	/** \brief Read next log data if available or empty string otherwise. */
	decString ReadNextLogData();
	
	/** \brief Last log file content if present. */
	decString GetLastLogContent();
	
	
	/** \brief Create task processors. */
	derlTaskProcessorRemoteClient::Ref CreateTaskProcessor() override;
	
	
	/** \brief Is running. */
	bool IsRunning();
	
	/** \brief Start application. */
	void StartApplication();
	
	/** \brief Stop application.
	 */
	void StopApplication();
	
	/** \brief Kill application. */
	void KillApplication();
	
	/** \brief Process received system properties. */
	void ProcessReceivedSystemProperties();
	
	/** \brief Connection established. */
	void OnConnectionEstablished() override;
	
	/** \brief Connection closed either by calling Disconnect() or by server. */
	void OnConnectionClosed() override;
	
	/** \brief Begin synchronize. */
	void OnSynchronizeBegin() override;
	
	/** \brief Synchronize update. */
	void OnSynchronizeUpdate() override;
	
	/** \brief Synchronize finished. */
	void OnSynchronizeFinished() override;
	
	/** \brief Run status changed. */
	void OnRunStatusChanged() override;
	
	/** \brief System property received from client. */
	void OnSystemProperty(const std::string &property, const std::string &value) override;
	
	
	/**
	 * \brief Launch profiles.
	 * \note Locks mutex while accessing client data.
	 */
	decStringSet GetLaunchProfiles();
	
	/**
	 * \brief Set launch profiles.
	 * \note Locks mutex while accessing client data.
	 */
	void SetLaunchProfiles(const decStringSet &profiles);
	
	/**
	 * \brief Active launch profile.
	 * \note Locks mutex while accessing client data.
	 */
	decString GetActiveLaunchProfile();
	
	/**
	 * \brief Set active launch profile.
	 * \note Locks mutex while accessing client data.
	 */
	void SetActiveLaunchProfile(const decString &profile);
	
	/**
	 * \brief Default launch profile.
	 * \note Locks mutex while accessing client data.
	 */
	decString GetDefaultLaunchProfile();
	
	/**
	 * \brief Set default launch profile.
	 * \note Locks mutex while accessing client data.
	 */
	void SetDefaultLaunchProfile(const decString &profile);
	/*@}*/
	
	
	
	/** \name Listeners */
	/*@{*/
	/** \brief Add listener. */
	void AddListener(projRemoteClientListener *listener);
	
	/** \brief Remove listener. */
	void RemoveListener(projRemoteClientListener *listener);
	
	
	/** \brief Notify listeners client changed. */
	void NotifyClientChanged();
	
	/** \brief Notify listeners client disconnected. */
	void NotifyClientDisconnected();
	
	/** \brief Notify listeners launch profiles changed. */
	void NotifyLaunchProfilesChanged();
	
	/** \brief Notify listeners active launch profile changed. */
	void NotifyActiveLaunchProfileChanged();
	/*@}*/
	
	
	
private:
	void pBuildGameXml(derlRunParameters &params, const projProfile &profile);
	void pBuildGameXml(decXmlWriter &writer, const projProfile &profile);
};

#endif
