# Easy to Read Colorset Generator
Custom colorset generator for Rhythia
This generates more easy-to-read colorset from colorset you use
# How does this work
Using the same color in the hold and stack sections helps improve the visibility

 - Stack - Notes that come at the exactly same time (timeDiff == 0)
 - Hold - Notes that come at almost same position (timeDiff < 60ms and distance < 0.25 size of note)
 
 # How to use
 When you download .exe file follow these steps
 1.  Prepare these files below:

	 - .txt data of map (You can export .txt file in Rhythia)
	 - .txt file of colorset you want to use
 
 2. Rename the colorset to "colorset.txt"
 3. Run "color.exe" and choose .txt file of map
 4. This program overwrite .txt data of map so open it and check if the file contains colorset format data
 5. There is 2 color codes at the first line so delete the first one
	 

> When you open it, it looks like this:
> #FF00FF#50F340
> #A90028
> **︙**
> 
> In this case, delete #FF00FF
6. Save the file and put it in colorset and reload the game

# Known issue
idk why but this code generates 2 color codes at the first line

 
 

 
