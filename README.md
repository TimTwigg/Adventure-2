# Adventure 2
 
Text Based Sandbox Adventure Game. I began this project to practice C++ design principles, as well as get experience working on a larger project than those in university courses.

## To Play

<u>Note:</u> The game relies on an Interface class to interact with the user. Currently, this is implemented for Windows only. The game is basically playable, but is still in development.

Download the repo files. <br/>
Run `build.bat` to build the executable. <br/>
Run `run.bat` to run the game.

## Code Design

All game source files are located in /lib. <br/>
Game data files are located in /data. <br/>
Information data files are located in /infodata. <br/>
Testing source files are located in /tests. <br/>
Error log files are created in /logs. <br/>
The code uses objects to represent everything in the game. <br/>

Player and Map are the core classes used to represent the player and their world.

User interaction is managed through Interface. Interface is an abstract base class which defines methods for user interaction. <br/>
WindowsInterface implements the Interface for Windows.

Thing is the root abstract base class. <br/>
Object and Entity are abstract classes inheriting from Thing.

Civilization inherits from Thing.

Animal and Enemy inherit from Entity. <br/>
Container, Machine, Resource, Tool, and Weapon inherit from Object. <br/>
CResource (or craftable resource) inherits from Resource.

The entry game menu is managed by Launcher. <br/>
The game is managed by GameEngine.

Colors, Formulae, RandomGenerator, ScreenInstructions, StringHelpers, and TextArt define helper functions and content. <br/>
FileReader manages all data and infodata file interactions. <br/>
StatDefaults defines numeric default values.