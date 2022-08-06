# updated 6 August 2022
# Counts the number of lines in code files

from pathlib import Path

breakdown = ["\nCode Files:"]

def countlines(f_list) :
    count = 0
    for file in f_list :
        startcount = count
        with file.open("r") as f :
            for _ in f :
                count += 1
        breakdown.append(f"  {file.name:25} | {count - startcount}")
    return count

p = Path()
libP = p / "lib"
files = [i.resolve() for i in libP.iterdir() if i.name != "json.hpp"]
count1 = countlines(files)
print("\nCode Files: {} lines".format(count1))

breakdown.append("\nTest Files:")
testP = p / "tests"
files = [i.resolve() for i in testP.iterdir()]
count2 = countlines(files)
print(f"Test Files: {count2}")

data_dirs = ["data", "infodata"]
count3 = 0
for dir in data_dirs :
    breakdown.append(f"\nData Files - {dir}:")
    p2 = p / dir
    files = [i.resolve() for i in p2.iterdir()]
    count3 += countlines(files)
print("Data Files: {} lines".format(count3))

print("Total: {} lines".format(count1 + count2 + count3))

for l in breakdown:
    print(l)