# TODO
- Add enemies to map biome generation
- peaceful setting
- Write GameEngine
- Test GameEngine
- look around? (list all directions)
- Binoculars allow to look two blocks across instead of one
- Replace Liquid class with enum class in Container.hpp
- Container enable set amount
- Debug civ setTrades - testing shows selling trades sometimes gets 5 instead of 6 on a town
    - problem is trades are set by randomly choosing a name in the category, trying 10 times to find one
    - which has not been already selected. Might need to change the category as well
- Create UML class diagrams
- finish Civilization::setTrades() - add animals, potions, machines etc
- Add player-built buildings/stuff
- chopping trees removes them if not in forest?
- crafting things like ammo crafts more than 1
- Add machine use commands (smoke uses smoker already, need to add command for furnace)
- finish object_info.json - fill in blanks marked by "@@@"
- Add more detailed descriptions to commandUses.json
- integrate Factory
    - GameEngine::craft, ??
    - Player
    - Map

# IDEAS
- Refresh ht and hp completely at towns? For a price?
- For buildings - have preset options that require set amounts of resources? Similar to Hearthfire?
- Be able to place a chest and store resources in a certain location, later to return.
- Visual representation of map? Include user-placed things and show whether settlements have been visited/raided/etc.
- Ability to name sections of a map? Search for named section and get direction towards it?
- Enemies scaling with player?
- Civilization resets trades after a while?
- Civilizations and entities heal over time?
- Log user commands?
- Able to sleep anywhere or must it be safe? What about bed? Do you heal overnight?
- Create "Make Item" function that creates the shared ptr. Maybe in Map? Then Player::addItem would take that object as param

# MAINLOOP QUESTION
Game Engine in C++ but prompt querying in python so I can use prompt_toolkit to get auto-completions and so on? If so, call python from C++ mainloop, or call C++ functions from python mainloop?
Could create a c++ wrapper on c++ GameEngine taking in int argument for function number, and then use python mainloop to call it.

# ACTION QUESTIONS
## Sleep
- "sleep until morning" or "sleep 8 hours?"
    - if i implemented an exhaustion system then sleeping would be more meaningful
- are mobs more common or more aggressive during the night?

# TEST
- check and rework damage formulae