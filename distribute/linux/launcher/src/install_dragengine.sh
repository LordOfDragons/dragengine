#!/bin/bash

function abort_installation()
{
	echo ""
	echo -e "\e[93m\e[1mAbort installation. Application will not be able to run.\e[39m\e[0m"
	echo ""
	echo "You can manually install the game engine before running the application again."
	echo "Please see https://dragondreams.ch/?page_id=152#downloads-dragengine for installation options."
	echo ""
	echo -e "\e[93m\e[1m"
	read -p "Press any key to close the installer " ans
	echo -e "\e[39m\e[0m"
	exit 1
}

function add_ppa()
{
	echo ""
	echo -e "\e[93m\e[1m>>> Adding DragonDreams PPA Repository...\e[39m\e[0m"
	echo ""
	
	if ! hash add-apt-repository 2> /dev/null
	then
		sudo apt install -y software-properties-common
	fi
	
	if ! hash add-apt-repository 2> /dev/null
	then
		echo ""
		echo -e "\e[93m\e[1mCould not find nor install add-apt-repository.\e[39m\e[0m"
		echo ""
		abort_installation
	fi
	
	sudo add-apt-repository -y -u ppa:rpluess/dragondreams
}

function install_package()
{
	echo ""
	echo -e "\e[93m\e[1m>>> Install dragengine package...\e[39m\e[0m"
	echo ""
	
	sudo apt-get install -y dragengine
}

function installation_finished_successfully()
{
	echo ""
	echo -e "\e[93m\e[1m>>> Finished Successfully\e[39m\e[0m"
	echo ""
	echo -e "\e[93m\e[1m"
	read -p "Press any key to close the installer " ans
	echo -e "\e[39m\e[0m"
}

function install_dragengine()
{
	add_ppa
	install_package
	installation_finished_successfully
}

echo -e "\e[93m\e[1mTo run this application the \e[32m\e[1mDrag[en]gine Game Engine\e[93m\e[1m is required.\e[39m\e[0m"
echo ""
echo "To continue the Drag[en]gine Linux Packages have to be installed."
echo "Future updates are automatically installed when you update your system."
echo ""
echo -e "\e[93m\e[1mThe following changes are required to continue:\e[39m\e[0m"
echo "- Add DragonDreams PPA Repository providing the required packages."
echo "- Install dragengine package."
echo ""
echo "(Additional information can be found at https://dragondreams.ch/?page_id=567)"
echo ""
echo -e "\e[91m\e[1m"
read -p "Do you want to apply these changes to your system? [y/n] " ans
echo -e "\e[39m\e[0m"
if [[ "${ans:0:1}" == "y" || "${ans:0:1}" == "Y" ]]
then
	echo "To apply the changes the installer requires super user permissions."
	echo "Please enter the super user password if asked."
	echo ""
	echo -e "\e[91m\e[1m"
	read -p "Do you want to continue? [y/n] " ans
	echo -e "\e[39m\e[0m"
	if [[ "${ans:0:1}" == "y" || "${ans:0:1}" == "Y" ]]
	then
		install_dragengine
		exit 0
	fi
	exit 0
fi
abort_installation

# end of script
