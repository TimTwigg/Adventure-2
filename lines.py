from pathlib import Path

def countlines(p_list) :
    count = 0
    for py in p_list :
        with py.open("r") as f :
            for _ in f :
                count += 1
    return count

p = Path()
libP = p / "lib"
files = [i.resolve() for i in libP.iterdir() if i.name != "json.hpp"]
count1 = countlines(files)
print("\nFiles: {} lines".format(count1))

data_dirs = ["data", "infodata"]
count2 = 0
for dir in data_dirs :
    p2 = p / dir
    files = [i.resolve() for i in p2.iterdir()]
    count2 += countlines(files)
print("Data Files: {} lines".format(count2))

print("Total: {} lines".format(count1 + count2))
