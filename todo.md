# TODO
- Replace Liquid class with enum class in Container.hpp?
- Debug civ setTrades - testing shows selling trades sometimes gets 5 instead of 6 on a town
    - problem is trades are set by randomly choosing a name in the category, trying 10 times to find one which has not been already selected. Might need to change the category as well
- finish Civilization::setTrades() - add animals, potions, machines etc
- Add player-built buildings/stuff
- chopping trees removes them if not in forest?
- crafting things like ammo crafts more than 1
- finish object_info.json - fill in blanks marked by "@@@"
- finish detailed descriptions in commandUses.json (marked by "Not Implemented")
- get attacked (bottom of GameEngine mainloop)
- raiding a civ makes it hostile?
- balance hunger and thirst. needs to be semi-realistic but not be too nitty-gritty to deal with for player
- low hunger and thirst has consequences
- make name matches more malleable by checking both singular and plural?
- how does a player heal?
- only able to put stuff into static storages? or backpack as well?
- review Civ loot selection
- Actually use mining/chopping/etc ratios
- Write tests
    - formulaTests
    - configTests
- infodata validation program
    - make sure that all objects have a description, etc
- revise windowsInterface askSelect to mirror prompt_toolkit behavior after selection is made
- revise location description indefinite article logic

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
- Trading with civs: when items are all bought, replenish/replace?
- item to increase certain stats, eg carry capacity
- time
    - growth
    - change
    - events
- automation with npc builders, miners, workers, etc?

# MAINLOOP QUESTION
Game Engine in C++ but prompt querying in python so I can use prompt_toolkit to get auto-completions and so on? If so, call python from C++ mainloop, or call C++ functions from python mainloop?
Could create a c++ wrapper on c++ GameEngine taking in int argument for function number, and then use python mainloop to call it. Or I create my own C++ auto-complete?

# ACTION QUESTIONS
## Sleep
- "sleep until morning" or "sleep 8 hours?"
    - if i implemented an exhaustion system then sleeping would be more meaningful
- are mobs more common or more aggressive during the night?

# TEST
- check and rework damage formulae
- test raiding high-level civs
    - manually set damage to super high to insta raid
