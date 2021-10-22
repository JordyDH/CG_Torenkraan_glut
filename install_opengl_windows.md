# Install OpenGL on windows with WSL (by christophe)

## Activate and install WSL
Search for "Turn on Windows features or off" or "Windows-onderdelen in- of uitschakelen".  
Scroll down to Windows-subsystem for Linux and enable it.  
Restart your system or all the following steps will fail.  
Open the microsoft store and search for a linux-distro, my recomandation is Ubuntu.  

## Installing a xserver
to get a graphical output on windows you would need to install a xserver.  
One example is mobaXterm, this is a xserver for windows.  
https://mobaxterm.mobatek.net/download-home-edition.html  

Open a terminal and boot-up the wsl kernel.  
To use the xserver the the next enviroment variable.  
```bash
export DISPLAY=localhost:0
```
## Installing openGL
Before we can compile our openGL code we need to install openGL.  
```bash
sudo apt-get update
sudo apt-get install freeglut3-dev
```
## Accessing windows files in WSL
```bash
cd /
cd /mnt
```
You can find your partitions in the /mnt folder.

## Contribution
Instructions provided by Christophe (Delta-473).
