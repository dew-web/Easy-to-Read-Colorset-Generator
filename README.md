# Easy to Read Colorset Generator
Custom colorset generator for Rhythia <br>
This generates more easy-to-read colorset from colorset you use
# How does this work
Using the same color in the hold and stack sections to improve the visibility

 - Stack - Notes that come at the exactly same time (timeDiff == 0)
 - Hold - Notes that come at almost same position (timeDiff < 60ms and distance < 0.25 size of note)
 
 # How to use
 When you download .exe file follow these steps
 1.  Prepare these files below:

	 - .txt data of map (You can export .txt file in Rhythia)
	 - .txt file of colorset you want to use
 
 2. Run "ColorsetGen.exe" and choose .txt file of colorset first, and then choose .txt file of map
 3. Once you did that, (song_id)_colorset.txt will be generated so put it in colorsets folder and reload the game

# Known issue
high density stream is detected as hold (i'll fix this soon)

# For those want to change timeDiff, distance, etc
Change the value and compile with this code below:
```
g++ -o (name).exe main.cpp -lcomdlg32
```
 

 
