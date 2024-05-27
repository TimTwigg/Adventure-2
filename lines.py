# updated 27 May 2024
# Counts the number of lines in code files

from pathlib import Path

breakdown = ["\n##############################\n\nBREAKDOWN\n##############################", "\nCode Files:"]
fileCount = 0

def countlines(f_list, include_path=False):
    global breakdown, fileCount
    count = 0
    for file in f_list :
        startcount = count
        with file.open("r") as f :
            for _ in f :
                count += 1
        if include_path:
            breakdown.append(f"  {str(file):25} | {count - startcount}")
        else:
            breakdown.append(f"  {file.name:25} | {count - startcount}")
        fileCount += 1
    return count

print("\nSUMMARY\n##############################")

p = Path()
count1 = countlines([Path("main.cpp")])
libP = p / "lib"
files = [i.resolve() for i in libP.iterdir() if i.name != "json.hpp"]
count1 += countlines(files)
print(f"\nCode Files: {count1} lines ({len(files)} files)")

breakdown.append("\nTest Files:")
testP = p / "tests"
files = [i.resolve() for i in testP.iterdir() if i.resolve().suffix == ".cpp"]
count2 = countlines(files)
print(f"Test Files: {count2} lines ({len(files)} files)")

data_dirs = ["data", "data/outputs", "infodata"]
count3 = 0
fcount = 0
for dir in data_dirs :
    breakdown.append(f"\nData Files - {dir}:")
    p2 = p / dir
    files = [i.resolve() for i in p2.iterdir() if i.is_file() and i.resolve().suffix != ".py"]
    fcount += len(files)
    count3 += countlines(files)
print(f"Data Files: {count3} lines ({fcount} files)")

breakdown.append("\nConfiguration Files:")
config_files = [i for i in Path().rglob("CMakeLists.txt") if len(i.parts) == 0 or i.parts[0] not in ["build"]] \
    + [Path("config.ini")]
count4 = countlines(config_files, True)
print(f"Configuration Files: {count4} lines ({len(config_files)} files)")

breakdown.append("\nPython Files:")
py_files = [i for i in Path().rglob("*.py") if len(i.parts) == 0 or i.parts[0] not in ["build"]]
count5 = countlines(py_files, True)
print(f"Python Files: {count5} lines ({len(py_files)} files)")

print(f"\nTotal: {count1 + count2 + count3 + count4 + count5} lines ({fileCount} files)")

for l in breakdown:
    print(l)