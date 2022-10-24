# Adventure 2
 
Text Based Sandbox Adventure Game

## Code Design

All game source files are located in /lib.
Game data files are located in /data.
Information data files are located in /infodata.
Testing source files are located in /tests.
Error log files are created in /logs.
The code uses objects to represent everything in the game.

Player and Map are the core classes used to represent the player and their world.

User interaction is managed through Interface. Interface is an abstract base class which defines methods for user interaction.
WindowsInterface implements the Interface for Windows.

Thing is the root abstract base class.
Object and Entity are abstract classes inheriting from Thing.

Civilization inherits from Thing.

Animal and Enemy inherit from Entity.
Container, Machine, Resource, Tool, and Weapon inherit from Object.
CResource (or craftable resource) inherits from Resource.

The entry game menu is managed by Launcher.
The game is managed by GameEngine.

Colors, Formulae, RandomGenerator, ScreenInstructions, StringHelpers, and TextArt define helper functions and content.
FileReader manages all data and infodata file interactions.
StatDefaults defines numeric default values.