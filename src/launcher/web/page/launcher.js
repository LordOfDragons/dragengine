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
		this._initPanels()
		
		window.addEventListener("load", this._onPageLoaded.bind(this));
		window.addEventListener("resize", this._onPageResize.bind(this));
		window.addEventListener(WebLauncher.EventLog, this._onLogMessage.bind(this));
		
		// this.launcherWorker = new Worker(new URL('launcherWorker.js', import.meta.url));
		// this.launcherWorker.onmessage = this._onWorkerMessage.bind(this)
	}
	
	/**
	 * Webassembly runtime finished loading.
	 */
	onRuntimeInitialized() {
		this.logInfo('Runtime initialized')
		this.launcher = new Module.dewlLauncher()
		this.launcher.canvasId = `#${this.panelCanvas.panelId()}`
	}
	
	_onWorkerMessage(event) {
		const data = event.data
		const action = data['action']
		
		if (action == WebLauncher.EventLog) {
			this.panelLogs.addLogs(data['severity'], data['source'], data['message'])
			
		} else if (action == WebLauncher.EventInitialized) {
			if (data['error']) {
				
			} else {
				
			}
		}
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
			this.gameRunning = false
			this._dispatchEvent(WebLauncher.EventGameRunningChanged, {running: false})
			throw e
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
				const stat = FS.stat(filename);
				if ((stat.mode & 0o100000) == 0o100000) {
					zip.file(filename.substring(basePathLen), FS.readFile(filename))
				} else if ((stat.mode & 0o40000) == 0o40000) {
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
		
		this._fsMkdirAbsent('/idbfs/config');
		this._fsMkdirAbsent('/idbfs/cache');
		this._fsMkdirAbsent('/idbfs/capture');
		
		this._fsMkdirAbsent('/dragengine');
		FS.symlink('/idbfs/config', '/dragengine/userConfig');
		FS.symlink('/idbfs/cache', '/dragengine/userCache');
		FS.symlink('/idbfs/capture', '/dragengine/userCapture');
		
		this._fsMkdirAbsent('/dragengine/userCache/delgas');
		this._fsMkdirAbsent('/dragengine/localDelgas');
		
		this._dispatchEvent(WebLauncher.EventInitialized, {})
	}
	
	_fsMkdirAbsent(path) {
		if (!FS.analyzePath(path).exists) {
			FS.mkdir(path);
		}
	}
	
	_dispatchEvent(name, data) {
		window.dispatchEvent(new CustomEvent(name, {detail: data}))
	}
	
	_runGame(pathDelga, options) {
		this.launcher.delgaPath = pathDelga
		if (options.profileName) {
			this.launcher.profileName = options.profileName
		}
		
		this.launcher.removeAllModuleParameters()
		this.launcher.addModuleParameter('OpenGL', 'logLevel', 'debug')
		this.launcher.addModuleParameter('OpenGL', 'debugContext', '0')
		this.launcher.addModuleParameter('OpenGL', 'debugNoMessages', '0')
		
		this.launcher.runGame()
		
		/*
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
		*/
	}
}

window.webLauncher = new WebLauncher()
