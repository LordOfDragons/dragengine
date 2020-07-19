# 
# Example build configuration for BeOS platforms.
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
# - 'null': Use no ToolKit. Only special command lines supported.
# 
# igde_toolkit = 'null'


# System path
# 
# Value type: string with directory path
# 
# prefix = '/boot/system'

# System libraries
# 
# Value type: string with directory path
# 
# libdir = '${prefix}/lib'

# System includes
# 
# Value type: string with directory path
# 
# includedir = '${prefix}/develop/include'

# System shares
# 
# Value type: string with directory path
# 
# datadir = '${prefix}/data'

# System configuration
# 
# Value type: string with directory path
# 
# sysconfdir = '${prefix}/settings'

# System binaries
# 
# Value type: string with directory path
# 
# execdir = '${prefix}/bin'

# System var
# 
# Value type: string with directory path
# 
# sysvardir = '${prefix}/var'

# System cache
# 
# Value type: string with directory path
# 
# cachedir = '${prefix}/cache'

# System documentation
# 
# Value type: string with directory path
# 
# docdir = '${prefix}/documentation'

# Path to the Drag[en]gine binaries
# 
# Value type: string with directory path
# 
# path_de_bin = '${execdir}'

# Path to the Drag[en]gine libraries
# 
# Value type: string with directory path
# 
# path_de_lib = '${libdir}'

# Path to the Drag[en]gine headers
# 
# Value type: string with directory path
# 
# path_de_include = '${includedir}/dragengine'

# Path to the Drag[en]gine configuration
# 
# Value type: string with directory path
# 
# path_de_config = '${sysconfdir}/dragengine'

# Path to the Drag[en]gine data
# 
# Value type: string with directory path
# 
# path_de_data = '${libdir}/dragengine'

# Path to the Drag[en]gine shares
# 
# Value type: string with directory path
# 
# path_de_share = '${datadir}/dragengine'

# Path to the Drag[en]gine cache
# 
# Value type: string with directory path
# 
# path_de_cache = '${cachedir}/dragengine'

# Path to the IGDE binaries
# 
# Value type: string with directory path
# 
# path_igde_bin = '${execdir}'

# Path to the IGDE libraries
# 
# Value type: string with directory path
# 
# path_igde_lib = '${libdir}'

# Path to the IGDE headers
# 
# Value type: string with directory path
# 
# path_igde_include = '${includedir}/deigde'

# Path to the IGDE configuration
# 
# Value type: string with directory path
# 
# path_igde_config = '${sysconfdir}/deigde'

# Path to the IGDE data
# 
# Value type: string with directory path
# 
# path_igde_data = '${libdir}/deigde'

# Path to the IGDE shares
# 
# Value type: string with directory path
# 
# path_igde_share = '${datadir}/deigde'

# Path to the Launcher binaries
# 
# Value type: string with directory path
# 
# path_launcher_bin = '${execdir}'

# Path to the Launcher binary libraries
# 
# Value type: string with directory path
# 
# path_launcher_bin_lib = '${libdir}'

# Path to the Launcher configuration
# 
# Value type: string with directory path
# 
# path_launcher_config = '${sysconfdir}/delauncher'

# Path to the Launcher data
# 
# Value type: string with directory path
# 
# path_launcher_data = '${libdir}/delauncher'

# Path to the Launcher shares
# 
# Value type: string with directory path
# 
# path_launcher_share = '${datadir}/delauncher'

# Path to the Launcher games
# 
# Value type: string with directory path
# 
# path_launcher_games = '/boot/system/delauncher/games'
