import os

file_path = './git_tools/cherry_pick.txt'
file = open(file_path, 'rt')

lines = file.read().split('\n')
commit = lines.pop()

file.close()
file = open(file_path, 'w')
file.write('\n'.join(lines))

print('Cherry picking: ' + commit)
os.system('git cherry-pick %s' % commit)