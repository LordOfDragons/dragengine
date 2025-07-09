import { PanelLogs } from './panelLogs.mjs'
import { PanelSettings } from './panelSettings.mjs'
import { PanelCanvas } from './panelCanvas.mjs'
import './jszip.min.js'

/**
 * Web launcher class.
 */
export class WebLauncher {
	/**
	 * Send log event.
	 * 
	 * Event details contains:
	 * - severity {PanelLogs.Severity}: Log severity.
	 * - source {String}: Log source.
	 * - message {String}: Log message.
	 */
	static EventLog = 'delauncher:log'
	
	/**
	 * Web launcher has finished initialize event.
	 * 
	 * Event details contains:
	 * - error {Any}: Error or null if success.
	 */
	static EventInitialized = 'delauncher:initialized'
	
	/**
	 * Game running state changed.
	 * 
	 * Event details contains:
	 * - running {Boolean}: Game is running.
	 */
	static EventGameRunningChanged = 'delauncher:gameRunningChanged'
	
	/**
	 * Game can not run due to problems.
	 * 
	 * Event details contains:
	 * - gameProblems {String[]}: Game problems or undefined.
	 * - profileName {String}: Profile name or undefined.
	 * - profileProblems {String[]}: Profile problems or undefined.
	 * - patchProblems {String[]}: Patch problems or undefined.
	 */
	static EventRunProblems = 'delauncher:gameProblems'
	
	
	/**
	 * Create web launcher
	 */
	constructor() {
		this.gameRunning = false
		this.launcher = undefined
		this._initPanels()
		
		window.addEventListener("load", this._onPageLoaded.bind(this));
		window.addEventListener("resize", this._onPageResize.bind(this));
		window.addEventListener(WebLauncher.EventLog, this._onLogMessage.bind(this));
		
		this._initFileSystem()
	}
	
	logInfo(message) {
		this.panelLogs.addLogs(PanelLogs.Severity.Info, 'Launcher', message)
	}
	
	logWarn(message) {
		this.panelLogs.addLogs(PanelLogs.Severity.Warning, 'Launcher', message)
	}
	
	logError(message) {
		this.panelLogs.addLogs(PanelLogs.Severity.Error, 'Launcher', message)
	}
	
	/**
	 * Create launcher and initialize it. Throws exception if launcher exists already.
	 */
	initLauncher() {
		if (this.launcher !== undefined) {
			throw 'Launcher exists already'
		}
		
		this.logInfo('Create launcher')
		
		try {
			this.launcher = new Module.dewlLauncher()
			this.logInfo('Initinalize launcher')
			
			/*
			this._launcher.AddArgument("this_is_a_test")
			this._launcher.AddArgument("something_else")
			
			var args_in = this._launcher.GetArgumentList()
			var args = new Array(args_in.size()).fill(0).map((_,i) => args_in.get(i))
			this.logInfo("Arguments: " + args)
			*/
			
			this.launcher.init()
			
		} catch (e) {
			this.disposeLauncher()
			throw e
		}
		this.logInfo('Launcher initialized')
	}
	
	/**
	 * Run game.
	 */
	runGame(pathDelga, options) {
		if (this.launcher === undefined) {
			throw 'Launcher or initialized'
		}
		if (this.gameRunning) {
			throw 'Game is already running'
		}
		
		this.logInfo(`Run game: ${pathDelga}`)
		this.gameRunning = true
		this._dispatchEvent(WebLauncher.EventGameRunningChanged, {running: true})
		
		try {
			this._runGame(pathDelga, options ?? {})
		
		} catch(e) {
			this.logError(`${e}: ${e.stack}`)
			this.gameRunning = false
			this._dispatchEvent(WebLauncher.EventGameRunningChanged, {running: false})
		}
		
		/*
		this.logInfo('Run game stopped')
		this.gameRunning = false
		this._dispatchEvent(WebLauncher.EventGameRunningChanged, {running: false})
		*/
	}
	
	/**
	 * Shutdown launcher and dispose it if existing.
	 */
	disposeLauncher() {
		if (this.launcher === undefined) {
			return
		}
		
		this.logInfo('Dispose of launcher')
		try {
			this.launcher.cleanUp()
			
		} finally {
			this.launcher.delete()
			this.launcher = undefined
		}
		this.logInfo('Launcher disposed')
	}
	
	/**
	 * Create zip archive of all log files.
	 */
	archiveLogs(callback) {
		const basePath = '/dragengine/userConfig/delauncher/logs'
		const basePathLen = basePath.length
		
		const zip = new JSZip()
		
		function _recursive(curPath) {
			for (const name of FS.readdir(curPath)) {
				if (name === '.' || name === '..'){
					continue
				}
				
				const filename = `${curPath}/${name}`
				const {mode, timestamp} = FS.lookupPath(filename).node
				if (FS.isFile(mode)) {
					zip.file(filename.substring(basePathLen), FS.readFile(filename))
				} else if (FS.isDir(mode)) {
					_recursive(filename);
				}
			}
		}
		_recursive(basePath)
		
		zip.generateAsync({
			type: 'blob',
			compression: 'DEFLATE',
			compressionOptions: {level: 9},
			streamFiles: true
		}).then(callback)
	}
	
	_readAllLogFiles(path) {
		const files = [];
		
		function _recursive(curPath) {
			for (const name of FS.readdir(curPath)) {
				if (name === '.' || name === '..'){
					continue
				}
				
				const childPath = `${curPath}/${name}`
				const {mode, timestamp} = FS.lookupPath(childPath).node
				if (FS.isFile(mode)) {
					files.push({path: childPath, timestamp})
				} else if (FS.isDir(mode)) {
					_recursive(childPath);
				}
			}
		}
		
		_recursive(path);
		return files;
	}
	
	_onPageLoaded(_event) {
		this.panelCanvas.resizeCanvas()
	}
	
	_onPageResize(_event) {
		this.panelCanvas.resizeCanvas()
	}
	
	_onLogMessage(_event) {
		this.panelLogs.addLogs(_event.detail.severity, _event.detail.source, _event.detail.message)
	}
	
	_initPanels() {
		this.panelCanvas = new PanelCanvas('panel-canvas')
		
		this.panelSettings = new PanelSettings('panel-settings')
		this.panelSettings.showPanel()
		
		this.panelLogs = new PanelLogs('panel-logs')
		//this.panelLogs.showPanel()
	}
	
	_initFileSystem() {
		FS.mkdir('/idbfs');
		FS.mount(FS.filesystems.IDBFS, {autoPersist: true}, '/idbfs');
		FS.syncfs(true, this._initFileSystem2.bind(this));
	}
	
	_initFileSystem2(err) {
		if (err) {
			this._dispatchEvent(WebLauncher.EventInitialized, {error: err})
			return
		}
		
		if (!FS.analyzePath('/idbfs/config').exists) {
			FS.mkdir('/idbfs/config');
		}
		if (!FS.analyzePath('/idbfs/cache').exists) {
			FS.mkdir('/idbfs/cache');
		}
		if (!FS.analyzePath('/idbfs/capture').exists) {
			FS.mkdir('/idbfs/capture');
		}
		
		FS.mkdir('/dragengine');
		FS.symlink('/idbfs/config', '/dragengine/userConfig');
		FS.symlink('/idbfs/cache', '/dragengine/userCache');
		FS.symlink('/idbfs/capture', '/dragengine/userCapture');
		
		if (!FS.analyzePath('/dragengine/userCache/delgas').exists) {
			FS.mkdir('/dragengine/userCache/delgas');
		}
		
		FS.mkdir('/dragengine/localDelgas');
		
		this._dispatchEvent(WebLauncher.EventInitialized, {})
	}
	
	_dispatchEvent(name, data) {
		window.dispatchEvent(new CustomEvent(name, {detail: data}))
	}
	
	_runGame(pathDelga, options) {
		this.launcher.delgaPath = pathDelga
		if (options.profileName) {
			this.launcher.profileName = options.profileName
		}
		
		this.launcher.prepare()
		this.launcher.locateGame()
		
		var vector = this.launcher.getGameProblems()
		var problems = new Array(vector.size()).fill(0).map((_,i) => args_in.get(i))
		if (problems.length > 0) {
			this._dispatchEvent(WebLauncher.EventRunProblems, {gameProblems: problems})
			throw 'Game can not run: Game problems'
		}
		
		this.launcher.locateProfile()
		vector = this.launcher.getProfileProblems()
		var problems = new Array(vector.size()).fill(0).map((_,i) => args_in.get(i))
		if (problems.length > 0) {
			this._dispatchEvent(WebLauncher.EventRunProblems, {
				profileName: this.launcher.getLocatedProfileName(),
				profileProblems: problems
			})
			throw 'Game can not run: Profile problems'
		}
		
		this.launcher.locatePatches()
		vector = this.launcher.getPatchProblems()
		var problems = new Array(vector.size()).fill(0).map((_,i) => args_in.get(i))
		if (problems.length > 0) {
			this._dispatchEvent(WebLauncher.EventRunProblems, {
				profileName: this.launcher.getLocatedProfileName(),
				patchProblems: problems
			})
			throw 'Game can not run: Patch problems'
		}
		
		this.launcher.prepareRunParameters()
		this.launcher.startGame()
	}
}

window.webLauncher = new WebLauncher()
