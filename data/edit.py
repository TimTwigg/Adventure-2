# updated: 6 April 2022
# removes or adds a field to json entries in data files
# DEV USE ONLY. NOT USED IN GAME

import json

with open("resources.json", "r") as f :
    data = json.loads(f.read())
    f.close()

for k in sorted(data.keys()) :
    data[k]["general1"] = 0

with open("resources2.json", "w") as f :
    json.dump(data, f, indent = 4)
    f.close()
