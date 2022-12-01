## Intro

Xbooxp is a multiboot transfer software originally designed with winXP/2000 in mind, but has since been updated to work in Linux/BSD/Solaris thanks to Jacques Pelletier.

Xboo cables are cheap and easy to build. Plus you can send multiboot images of up to 256kB, and upload speed is fast. This program can send a 256kB multiboot image in about 8.1 - 8.5 seconds in my AMD 2100++ CPU. With the delay setting, its a little slower. Please refer to the [xboo cable documentation](https://github.com/vishnu350/xbooxp/blob/master/xboo_cable_info.txt).

This program was created as a contribution towards a community that has helped me a lot, with all the guides, emulators, and stuff. This is an old project from 2004. I uploaded this to Github and Google Code to get the code out there for hackers to play with it. I no longer support this project.


## Building from source

For Linux, just clone the directory and type **make**. The xbooxp binary will be built.
For Windows, the original code was built using Borland C/C++ compilers, and is no longer supported. However, since the code is very portable, it may still build on modern systems.


## Usage

Here is what you should see upon running the xbooxp command:<br>
`./xbooxp yourfile.mb -argument`

The arguments:

			-h		use this to upload multiboot file without a valid
					header. This does not affect or modify the file.

			-d[value]	To increase the delay value, use this. Example,
						if i want to use delay=1, then type -d1.
						Use this only if you have a lot of problems.

			-p[port]	Set the port address here. If your port address
						is 0x278, then type -p278. If no value specified
						then 0x378 will be used as default.

			-t		Set for testing purposes. This will allow you to 
					send files without actually starting them. The
					file will be sent, but wont start. Only for
					testing purposes.


To see the help screen, just type xbooxp, and you will see the above message (or similar).

So, if your parellel port address is 0x278, you want delay=2, and you want to upload a multiboot file named matrix.mb without a valid header, do:<br>
`./xbooxp matrix.mb -278 -h -d2`


## FAQ

1. How to know your parallel port address in Windows?
 - Right click on my computer and chose properties. Then click on the Hardware tab, and choose Device Manager.
 - Look for Ports(Com & LTP), and expand this branch. Then right click on printer port and choose properties.
 - Go to the Resources Tab, and look at the first IO-Range value, if 378-3FF that means you port is 0x378. This is the most common value.

2. How do i build a xboo cable?
 - To build one, check the [xboo cable documentation](https://github.com/vishnu350/xbooxp/blob/master/xboo_cable_info.txt). You only need to connect three wires, four IF you want to use high speed BURST BOOT. You can still use BURST BOOT with only three wires, only transfer is a little slower.
 - Check out the PERN project tutorial. It has a good coverage on the topic of building the xboo cable.

3. What is BURST BOOT?
 - BURST BOOT is a system designed by Martin Korth that allows you to send a multiboot image to replace the old image that you have transfered previously. So you dont need to RESET (turn off/on) the GBA, thus saving you time and hassle. To use this system:
  - Put a burst boot code(like a backdoor, you know) in your code.
  - Send your multiboot image.
  - Once you are done checking it out, send your multiboot image again. Simple as that.


## License

xbooxp is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

xbooxp is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with xbooxp.  If not, see <http://www.gnu.org/licenses/>.

For Linux, xbooxp works out of the box with the usual permission tweaks.
For Windows, xbooxp makes use of io.dll, which is free, and available at http://www.geekhideout.com/iodll.shtml. However, this is now no longer supported.


## Credits

Thanks to:
- Jacques Pelletier, for updating this program to make it work in Linux.
- Alexander aka arundel, for all his help and support. His gbacopy.asm was of GREAT help, but took a while to understand. His beta testing was also a great help. Check out his gbacopy for Linux.
- Matt, for his C code for CRC routines.
- Martin Korth's extremely informative GBAtek document.
- Fred's great io.dll from geekhideout.com.
