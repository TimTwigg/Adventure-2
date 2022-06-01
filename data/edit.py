# updated: 6 April 2022
# removes or adds a field to json entries in data files
# DEV USE ONLY. NOT USED IN GAME

import json

file = "animals"

with open(file + ".json", "r") as f :
    data = json.loads(f.read())
    f.close()

newData = {}
for k,v in data.items() :
    newD = {
        "health": v["health"],
        "damage": v["damage"],
        "xp": v["xp"],
        "value": v["value"],
        "drops": {
            v["drop"]: v["drop.count"],
            v["drop2"]: v["drop2.count"]
        },
        "interactions": {}
    }
    if v["special"] is not None and v["special"].startswith("resources"):
        newD["drops"][v["special"]] = v["special.count"]
    elif v["special"] is not None:
        newD["interactions"][v["special"]] = v["special.count"]

    newData[k] = newD

with open(file + "2.json", "w") as f :
    json.dump(newData, f, indent = 4)
    f.close()