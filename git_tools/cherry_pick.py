import os

file_path = "./git_tools/cherry_pick.txt"

while True:
    file = open(file_path, "rt")

    lines = file.read().split("\n")
    commit = lines.pop()
    if not commit:
        break

    file.close()
    file = open(file_path, "w")
    file.write("\n".join(lines))

    print("Cherry picking: " + commit)
    res = os.system("git cherry-pick %s" % commit)
    if res != 0:
        break
