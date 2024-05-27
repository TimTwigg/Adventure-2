# updated 27 May 2024
# validate data and infodata files

from pathlib import Path
import json
import unittest

DATASOURCE = Path("data/")
INFODATASOURCE = Path("infodata/")

class ValidateData(unittest.TestCase):
    def __init__(self, methodName: str = "runTest") -> None:
        super().__init__(methodName)
        
        self.animals: dict[str,dict] = json.loads(DATASOURCE.joinpath("animals.json").read_text())
        self.biomes: dict[str,dict] = json.loads(DATASOURCE.joinpath("biomes.json").read_text())
        self.civTypes: dict[str,dict] = json.loads(DATASOURCE.joinpath("civ-types.json").read_text())
        self.containers: dict[str,dict] = json.loads(DATASOURCE.joinpath("containers.json").read_text())
        self.craftableResources: dict[str,dict] = json.loads(DATASOURCE.joinpath("craftableResources.json").read_text())
        self.enemies: dict[str,dict] = json.loads(DATASOURCE.joinpath("enemies.json").read_text())
        self.liquids: dict[str,dict] = json.loads(DATASOURCE.joinpath("liquids.json").read_text())
        del self.liquids["empty"]
        self.machines: dict[str,dict] = json.loads(DATASOURCE.joinpath("machines.json").read_text())
        self.resources: dict[str,dict] = json.loads(DATASOURCE.joinpath("resources.json").read_text())
        self.skillsets: dict[str,dict] = json.loads(DATASOURCE.joinpath("skillsets.json").read_text())
        self.storages: dict[str,dict] = json.loads(DATASOURCE.joinpath("storages.json").read_text())
        self.tools: dict[str,dict] = json.loads(DATASOURCE.joinpath("tools.json").read_text())
        self.weapons: dict[str,dict] = json.loads(DATASOURCE.joinpath("weapons.json").read_text())
        
        self.smokerOutput: dict[str,str] = json.loads(DATASOURCE.joinpath("outputs/smoker.json").read_text())
        self.furnaceOutput: dict[str,str] = json.loads(DATASOURCE.joinpath("outputs/furnace.json").read_text())
        
        self.commands: list[str] = [c for c in INFODATASOURCE.joinpath("commands.txt").read_text().split("\n") if c != "END"]
        self.commandUses: dict[str,list[str]] = json.loads(INFODATASOURCE.joinpath("commandUses.json").read_text())
        self.objectInfo: dict[str,list[str]] = json.loads(INFODATASOURCE.joinpath("object_info.json").read_text())
        
        self.files = [f.stem for f in DATASOURCE.glob("*.json")] + ["wealth"]
    
    def test_biomes(self):
        keys = ["trees", "stone", "metal", "gold", "animals", "river", "people", "civ-types", "enemies"]
        for biome in self.biomes.values():
            self.assertEqual(len(biome), len(keys))
            self.assertEqual(list(biome.keys()), keys)
            
            self.assertIsInstance(biome["trees"], bool)
            self.assertIsInstance(biome["stone"], bool)
            self.assertIsInstance(biome["metal"], bool)
            self.assertIsInstance(biome["gold"], bool)
            self.assertIsInstance(biome["animals"], list)
            self.assertIsInstance(biome["river"], float)
            self.assertIsInstance(biome["people"], float)
            self.assertIsInstance(biome["civ-types"], list)
            self.assertIsInstance(biome["enemies"], list)
            
            for animal in biome["animals"]:
                self.assertIn(animal, self.animals.keys())
            
            for civType in biome["civ-types"]:
                self.assertIn(civType, self.civTypes.keys())
            
            for enemy in biome["enemies"]:
                self.assertIn(enemy, self.enemies.keys())
                
    def test_civTypes(self):
        keys = ["buying", "selling", "count", "items", "loot", "xp", "defense", "counter-attack"]
        for civType in self.civTypes.values():
            self.assertEqual(len(civType), len(keys))
            self.assertEqual(list(civType.keys()), keys)
            
            self.assertIsInstance(civType["buying"], list)
            self.assertIsInstance(civType["selling"], list)
            self.assertIsInstance(civType["count"], int)
            self.assertIsInstance(civType["items"], list)
            self.assertIsInstance(civType["loot"], int)
            self.assertIsInstance(civType["xp"], int)
            self.assertIsInstance(civType["defense"], int)
            self.assertIsInstance(civType["counter-attack"], int)
            
            for item in civType["buying"]:
                self.assertIn(item, self.files)
            
            for item in civType["selling"]:
                self.assertIn(item, self.files)
            
            for item in civType["items"]:
                self.assertIn(item, self.files)
    
    def test_containers(self):
        keys = ["amount", "value", "recipe", "weight"]
        for container in self.containers.values():
            self.assertEqual(len(container), len(keys))
            self.assertEqual(list(container.keys()), keys)
            
            self.assertIsInstance(container["amount"], int)
            self.assertIsInstance(container["value"], int)
            self.assertIsInstance(container["recipe"], dict)
            self.assertIsInstance(container["weight"], int)
            
            for item,val in container["recipe"].items():
                self.assertIn(item, self.resources)
                self.assertIsInstance(val, int)
                
    def test_craftableResources(self):
        keys = ["category", "value", "weight", "recipe", "output"]
        for craftableResource in self.craftableResources.values():
            self.assertEqual(len(craftableResource), len(keys))
            self.assertEqual(list(craftableResource.keys()), keys)
            
            self.assertIsInstance(craftableResource["category"], str)
            self.assertIsInstance(craftableResource["value"], int)
            self.assertIsInstance(craftableResource["recipe"], dict)
            self.assertIsInstance(craftableResource["weight"], int)
            self.assertIsInstance(craftableResource["output"], int)
            self.assertGreaterEqual(craftableResource["output"], 1)
            
            for item,val in craftableResource["recipe"].items():
                self.assertIn(item, self.resources)
                self.assertIsInstance(val, int)
                
            self.assertIn(craftableResource["category"], ["OBJECT", "AMMO"])
    
    def test_enemies(self):
        keys = ["health", "damage", "drops", "xp", "minimumLevel", "minimumDanger"]
        allDropTypes = self.weapons.keys() | self.tools.keys() | self.resources.keys() | self.craftableResources.keys() | {"wealth"}
        for enemy in self.enemies.values():
            self.assertEqual(len(enemy), len(keys))
            self.assertEqual(list(enemy.keys()), keys)
            
            self.assertIsInstance(enemy["health"], int)
            self.assertIsInstance(enemy["damage"], int)
            self.assertIsInstance(enemy["drops"], dict)
            self.assertIsInstance(enemy["xp"], int)
            self.assertIsInstance(enemy["minimumLevel"], int)
            self.assertIsInstance(enemy["minimumDanger"], str)
            self.assertGreaterEqual(enemy["minimumLevel"], 1)
            self.assertIn(enemy["minimumDanger"], ["Peaceful", "Normal", "Hostile"])
            
            for item,val in enemy["drops"].items():
                self.assertIn(item, allDropTypes)
                self.assertIsInstance(val, int)
                
    def test_liquids(self):
        keys = ["hunger", "thirst", "value", "hp", "weight"]
        for liquid in self.liquids.values():
            self.assertEqual(len(liquid), len(keys))
            self.assertEqual(list(liquid.keys()), keys)
            
            self.assertIsInstance(liquid["hunger"], int)
            self.assertIsInstance(liquid["thirst"], int)
            self.assertIsInstance(liquid["value"], int)
            self.assertIsInstance(liquid["hp"], int)
            self.assertIsInstance(liquid["weight"], int)
            
    def test_machines(self):
        keys = ["requires", "value", "weight", "recipe"]
        for machine in self.machines.values():
            self.assertEqual(len(machine), len(keys))
            self.assertEqual(list(machine.keys()), keys)
            
            self.assertIsInstance(machine["requires"], list)
            self.assertIsInstance(machine["value"], int)
            self.assertIsInstance(machine["weight"], int)
            self.assertIsInstance(machine["recipe"], dict)
            
            for item in machine["requires"]:
                self.assertIn(item, self.resources.keys())
            
            for item,val in machine["recipe"].items():
                self.assertIn(item, self.resources)
                self.assertIsInstance(val, int)
    
    def test_resources(self):
        keys = ["category", "value", "weight", "general1"]
        for resource in self.resources.values():
            self.assertEqual(len(resource), len(keys))
            self.assertEqual(list(resource.keys()), keys)
            
            self.assertIsInstance(resource["category"], str)
            self.assertIsInstance(resource["value"], int)
            self.assertIsInstance(resource["weight"], int)
            self.assertIsInstance(resource["general1"], int)
            
            self.assertIn(resource["category"], ["FOOD", "RAW-FOOD", "OBJECT", "AMMO", "MATERIAL"])
            
    def test_skillsets(self):
        keys = ["health", "damage", "fist_damage", "speed", "consumption_ratio", "chopping_ratio", "mining_ratio", "hunger_ratio", "carry_ratio"]
        for skillset in self.skillsets.values():
            self.assertEqual(len(skillset), len(keys))
            self.assertEqual(list(skillset.keys()), keys)
            
            self.assertIsInstance(skillset["health"], float)
            self.assertIsInstance(skillset["damage"], float)
            self.assertIsInstance(skillset["fist_damage"], float)
            self.assertIsInstance(skillset["speed"], float)
            self.assertIsInstance(skillset["consumption_ratio"], float)
            self.assertIsInstance(skillset["chopping_ratio"], float)
            self.assertIsInstance(skillset["mining_ratio"], float)
            self.assertIsInstance(skillset["hunger_ratio"], float)
            self.assertIsInstance(skillset["carry_ratio"], float)
    
    def test_storages(self):
        keys = ["capacity", "carry", "value", "weight", "recipe"]
        for storage in self.storages.values():
            self.assertEqual(len(storage), len(keys))
            self.assertEqual(list(storage.keys()), keys)
            
            self.assertIsInstance(storage["capacity"], int)
            self.assertIsInstance(storage["carry"], bool)
            self.assertIsInstance(storage["value"], int)
            self.assertIsInstance(storage["weight"], int)
            self.assertIsInstance(storage["recipe"], dict)
            
            for item,val in storage["recipe"].items():
                self.assertIn(item, self.resources)
                self.assertIsInstance(val, int)
                
    def test_tools(self):
        keys = ["hitpoints", "value", "recipe", "weight"]
        for tool in self.tools.values():
            self.assertEqual(len(tool), len(keys))
            self.assertEqual(list(tool.keys()), keys)
            
            self.assertIsInstance(tool["hitpoints"], int)
            self.assertIsInstance(tool["value"], int)
            self.assertIsInstance(tool["recipe"], dict)
            self.assertIsInstance(tool["weight"], int)
            
            for item,val in tool["recipe"].items():
                self.assertIn(item, self.resources)
                self.assertIsInstance(val, int)
                
    def test_weapons(self):
        keys = ["type", "ammo", "m-damage", "r-damage", "hitpoints", "value", "recipe", "weight"]
        for weapon in self.weapons.values():
            self.assertEqual(len(weapon), len(keys))
            self.assertEqual(list(weapon.keys()), keys)
            
            self.assertIsInstance(weapon["type"], str)
            self.assertIsInstance(weapon["ammo"], str)
            self.assertIsInstance(weapon["m-damage"], float)
            self.assertIsInstance(weapon["r-damage"], float)
            self.assertIsInstance(weapon["hitpoints"], int)
            self.assertIsInstance(weapon["value"], int)
            self.assertIsInstance(weapon["recipe"], dict)
            self.assertIsInstance(weapon["weight"], int)
            self.assertIn(weapon["type"], ["melee", "ranged"])
            
            self.assertIn(weapon["ammo"], list(self.craftableResources.keys()) + ["none"])
            if weapon["type"] == "ranged":
                self.assertEqual(self.craftableResources[weapon["ammo"]]["category"], "AMMO", item)
            else:
                self.assertEqual(weapon["ammo"], "none")
            
            for item,val in weapon["recipe"].items():
                self.assertIn(item, self.resources)
                self.assertIsInstance(val, int)
    
    def test_outputs(self):        
        for inp, outp in self.smokerOutput.items():
            self.assertIn(inp, self.resources.keys())
            self.assertIn(outp, self.resources.keys())
        
        for inp, outp in self.furnaceOutput.items():
            self.assertIn(inp, self.resources.keys())
            self.assertIn(outp, self.resources.keys())
    
    def test_commandUses(self):
        for command in self.commands:
            command = command.split()[0]
            self.assertIn(command, self.commandUses.keys())
            self.assertGreaterEqual(len(self.commandUses[command]), 2)
            self.assertTrue(self.commandUses[command][1].startswith("Description:"), command)
    
    def test_objectInfo(self):
        allObjects = self.containers.keys() | self.craftableResources.keys() | self.liquids.keys() | self.machines.keys() | self.resources.keys() | self.storages.keys() | self.tools.keys() | self.weapons.keys()
        for obj in allObjects:
            self.assertIn(obj, self.objectInfo.keys())
        
        categories = ["CONTAINER", "AMMO", "LIQUID", "MACHINE", "FOOD", "RAW-FOOD", "STORAGE", "TOOL", "WEAPON", "OBJECT", "MATERIAL"]
        for obj, info in self.objectInfo.items():
            label, category = info[0].split(":")
            category = category.strip()
            self.assertEqual(label, "Category", obj)
            self.assertIn(category, categories, obj)
            
            if category in ["RAW-FOOD", "OBJECT", "AMMO", "MATERIAL"]:
                self.assertEqual(len(info), 4, obj)
                self.assertTrue(info[1].startswith("Value:"), obj)
                self.assertTrue(info[2].startswith("Weight:"), obj)
                self.assertTrue(info[3].startswith("Acquire:"), obj)
            
            elif category == "FOOD":
                self.assertEqual(len(info), 5, obj)
                self.assertTrue(info[1].startswith("Value:"), obj)
                self.assertTrue(info[2].startswith("Weight:"), obj)
                self.assertTrue(info[3].startswith("Nutritional Value:"), obj)
                self.assertTrue(info[4].startswith("Acquire:"), obj)
                
            elif category == "CONTAINER":
                self.assertEqual(len(info), 5, obj)
                self.assertTrue(info[1].startswith("Liquid Count:"), obj)
                self.assertTrue(info[2].startswith("Value:"), obj)
                self.assertTrue(info[3].startswith("Weight:"), obj)
                self.assertTrue(info[4].startswith("Acquire:"), obj)
            
            elif category == "MACHINE":
                self.assertEqual(len(info), 5, obj)
                self.assertTrue(info[1].startswith("Value:"), obj)
                self.assertTrue(info[2].startswith("Weight:"), obj)
                self.assertTrue(info[3].startswith("Acquire:"), obj)
                self.assertTrue(info[4].startswith("Fuel:"), obj)
                
            elif category == "LIQUID":
                self.assertEqual(len(info), 7, obj)
                self.assertTrue(info[1].startswith("Value:"), obj)
                self.assertTrue(info[2].startswith("Weight:"), obj)
                self.assertTrue(info[3].startswith("Acquire:"), obj)
                self.assertTrue(info[4].startswith("Hunger:"), obj)
                self.assertTrue(info[5].startswith("Thirst:"), obj)
                self.assertTrue(info[6].startswith("HP:"), obj)
            
            elif category == "TOOL":
                self.assertEqual(len(info), 5, obj)
                self.assertTrue(info[1].startswith("Value:"), obj)
                self.assertTrue(info[2].startswith("Weight:"), obj)
                self.assertTrue(info[3].startswith("Acquire:"), obj)
                self.assertTrue(info[4].startswith("Uses:"), obj)
                
            elif category == "WEAPON":
                self.assertEqual(len(info), 8, obj)
                self.assertTrue(info[1].startswith("Value:"), obj)
                self.assertTrue(info[2].startswith("Weight:"), obj)
                self.assertTrue(info[3].startswith("Acquire:"), obj)
                self.assertTrue(info[4].startswith("Uses:"), obj)
                self.assertTrue(info[5].startswith("Melee damage ratio:"), obj)
                self.assertTrue(info[6].startswith("Ranged damage ratio:"), obj)
                self.assertTrue(info[7].startswith("Ammo type:"), obj)
            
            elif category == "STORAGE":
                self.assertEqual(len(info), 6, obj)
                self.assertTrue(info[1].startswith("Value:"), obj)
                self.assertTrue(info[2].startswith("Weight:"), obj)
                self.assertTrue(info[3].startswith("Acquire:"), obj)
                self.assertTrue(info[4].startswith("Carryable:"), obj)
                self.assertTrue(info[5].startswith("Capacity:"), obj)

if __name__ == "__main__":
    unittest.main()