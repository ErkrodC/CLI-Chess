# Chess
CLI Chess Game

INSTALLATION
---
- For Windows:
	- Download ZIP file and extract.
	- Next you will need to install Cygwin: https://cygwin.com/install.html

	- When installing Cygwin, be sure you install the following packages:
		- gcc-core
		- libgcc1
		- make
	- After installing Cygwin and the necessary packages,
		- open a Cygwin terminal and navigate to inside the 'Chess-Master' folder.
		- From there run "make Chess", then run "./Chess"

- For Linux:
	- Download files
	- Run "make Chess", then run "./Chess"

How to play
---
- You can make a selection from the menus by typing the number that's associated with the option you want.
- Inputting moves follow a strict format, \<fileFrom>\<rankFrom> \<fileTo>\<rankTo>
	- An example of moving the e2 pawn as white up two ranks would be "e2 e4" without quotes.
