#!/usr/bin/env bash

if [ -z ${1} ]
then
    echo "Enter first argument which is author email";
    exit
fi

echo "Commits for user ${1} from branch $(git symbolic-ref --short -q HEAD) will put into ./git_tools/committer_${1}.txt" ;

git log --committer="${1}" --format=format:%H > ./git_tools/committer_${1}.txt