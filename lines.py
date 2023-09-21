# updated 21 September 2023
# Counts the number of lines in code files

from pathlib import Path

breakdown = ["\nCode Files:"]
fileCount = 0

def countlines(f_list):
    global breakdown, fileCount
    count = 0
    for file in f_list :
        startcount = count
        with file.open("r") as f :
            for _ in f :
                count += 1
        breakdown.append(f"  {file.name:25} | {count - startcount}")
        fileCount += 1
    return count

p = Path()
count1 = countlines([Path("main.cpp")])
libP = p / "lib"
files = [i.resolve() for i in libP.iterdir() if i.name != "json.hpp"]
count1 += countlines(files)
print(f"\nCode Files: {count1} lines ({len(files)} files)")

breakdown.append("\nTest Files:")
testP = p / "tests"
files = [i.resolve() for i in testP.iterdir()]
count2 = countlines(files)
print(f"Test Files: {count2} lines ({len(files)} files)")

data_dirs = ["data", "infodata"]
count3 = 0
fcount = 0
for dir in data_dirs :
    breakdown.append(f"\nData Files - {dir}:")
    p2 = p / dir
    files = [i.resolve() for i in p2.iterdir()]
    fcount += len(files)
    count3 += countlines(files)
print(f"Data Files: {count3} lines ({fcount} files)")

print(f"Total: {count1 + count2 + count3} lines ({fileCount} files)")

for l in breakdown:
    print(l)