# 
# Example build configuration for Windows platforms.
# 
# To use rename to "custom.py" and uncomment the desired parameters.
# 
# Most parameters in here are tri-state and accept the values 'auto', 'yes' and 'no'.
# Since this is a python file after all you can also use the values True and False
# instead of 'yes' and 'no' as well as all kinds of code you can use in python for
# example to adjust configuration on external parameters.
# 
# See dist.custom.py for platform independent options to add here.
#


# ToolKit used to build IGDE. 
# 
# Value enumeration: one from:
# - 'fox': Use FOX ToolKit
# - 'null': Use no ToolKit. Only special command lines supported.
# 
# igde_toolkit = 'fox'


#
# Special path indicators can be used. Only one special path indicator can be used
# and it has to be the first component in the path. The following special path
# indicators are supported:
#
# @ProgramFiles
#   The Program Files folder. A typical path is C:\Program Files.
#
# @System
#   The Windows System folder. A typical path is C:\Windows\System32.
#
# @RoamingAppData
#   The file system directory that serves as a common repository for
#   application-specific data. A typical path is C:\Documents and
#   Settings\username\Application Data.
#
# @ProgramData
#   The file system directory that contains application data for all users.
#   A typical path is C:\Documents and Settings\All Users\Application Data.
#   This folder is used for application data that is not user specific. For
#   example, an application can store a spell-check dictionary, a database
#   of clip art, or a log file in the $ProgramData folder. This information
#   will not roam and is available to anyone using the computer.
#
# @PublicDocuments
#   The file system directory that contains documents that are common to all
#   users. A typical path is C:\Documents and Settings\All Users\Documents.
#
# @LocalAppData
#   The file system directory that serves as a data repository for local
#   (nonroaming) applications. A typical path is C:\Documents and
#   Settings\username\Local Settings\Application Data.
#
# @Documents
#   The virtual folder that represents the My Documents desktop item. A typical
#   path is C:\Documents and Settings\username\My Documents.</td>
#
# @Windows
#   The Windows directory or $System. This corresponds to the %windir% or
#   %SYSTEMROOT% environment variables. A typical path is C:\Windows.
# 


# Window program files directory
# 
# Value type: string with directory path
# 
# programfiles = '@ProgramFiles'

# Window system root directory
# 
# Value type: string with directory path
# 
# systemroot = '@System'

# Path to Drag[en]gine SDK directory
# 
# Value type: string with directory path
# 
# path_de_sdk = '${path_de}/SDK'

# Path to Drag[en]gine SDK libraries
# 
# Value type: string with directory path
# 
# path_de_sdk_lib = '${path_de_sdk}/lib'

# Path to Drag[en]gine SDK includes
# 
# Value type: string with directory path
# 
# path_de_sdk_inc = '${path_de_sdk}/include'

# Path to the Drag[en]gine Installation
# 
# Value type: string with directory path
# 
# path_de = '${programfiles}/Dragengine'

# Path to the Drag[en]gine binaries
# 
# Value type: string with directory path
# 
# path_de_bin = '${path_de}/Bin'

# Path to the Drag[en]gine libraries
# 
# Value type: string with directory path
# 
# path_de_lib = '${path_de_sdk_lib}'

# Path to the Drag[en]gine headers
# 
# Value type: string with directory path
# 
# path_de_include = '${path_de_sdk_inc}/dragengine'

# Path to the Drag[en]gine configuration
# 
# Value type: string with directory path
# 
# path_de_config = '${path_de}/Config'

# Path to the Drag[en]gine data
# 
# Value type: string with directory path
# 
# path_de_data = '${path_de}/Data'

# Path to the Drag[en]gine shares
# 
# Value type: string with directory path
# 
# path_de_share = '${path_de}/Share'

# Path to the Drag[en]gine cache
# 
# Value type: string with directory path
# 
# path_de_cache = '@LocalAppData/Dragengine/Cache'

# Path to Drag[en]gine IGDE SDK directory
# 
# Value type: string with directory path
# 
# path_igde_sdk = '${path_igde}/SDK'

# Path to Drag[en]gine IGDE SDK libraries
# 
# Value type: string with directory path
# 
# path_igde_sdk_lib = '${path_igde_sdk}/lib'

# Path to Drag[en]gine IGDE SDK includes
# 
# Value type: string with directory path
# 
# path_igde_sdk_inc = '${path_igde_sdk}/include'

# Path to the IGDE Installation
# 
# Value type: string with directory path
# 
# path_igde = '${programfiles}/DEIGDE'

# Path to the IGDE binaries
# 
# Value type: string with directory path
# 
# path_igde_bin = '${path_igde}/Bin'

# Path to the IGDE libraries
# 
# Value type: string with directory path
# 
# path_igde_lib = '${path_igde_sdk_lib}'

# Path to the IGDE headers
# 
# Value type: string with directory path
# 
# path_igde_include = '${path_igde_sdk_inc}/deigde'

# Path to the IGDE configuration
# 
# Value type: string with directory path
# 
# path_igde_config = '${path_igde}/Config'

# Path to the IGDE data
# 
# Value type: string with directory path
# 
# path_igde_data = '${path_igde}/Data'

# Path to the IGDE shares
# 
# Value type: string with directory path
# 
# path_igde_share = '${path_igde}/Share'

# Path to the Launcher Installation
# 
# Value type: string with directory path
# 
# path_launcher = '${path_de}/Launchers'

# Path to the Launcher binaries
# 
# Value type: string with directory path
# 
# path_launcher_bin = '${path_launcher}/Bin'

# Path to the Launcher binary libraries
# 
# Value type: string with directory path
# 
# path_launcher_bin_lib = '${path_launcher_bin}'

# Path to the Launcher configuration
# 
# Value type: string with directory path
# 
# path_launcher_config = '@RoamingAppData/DELaunchers/Config'

# Path to the Launcher data
# 
# Value type: string with directory path
# 
# path_launcher_data = '${path_launcher}/Data'

# Path to the Launcher shares
# 
# Value type: string with directory path
# 
# path_launcher_share = '${path_launcher}/Share'

# Path to the Launcher games
# 
# Value type: string with directory path
# 
# path_launcher_games = '${path_launcher}/Games'
