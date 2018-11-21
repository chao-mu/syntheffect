#!/bin/bash

function clone_or_update () {
    if [ ! -d "$2" ]; then
        git clone https://github.com/$1/$2
    fi 
    
    cd $2
    git checkout $3
    git pull origin $3
    cd -
}

# https://stackoverflow.com/questions/59895/getting-the-source-directory-of-a-bash-script-from-within
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"

cd $DIR/../../../addons

clone_or_update armadillu ofxTimeMeasurements master
clone_or_update roymacdonald ofxSoundObjects master
clone_or_update npisanti ofxAudioFile master
clone_or_update kylemcdonald ofxFft master
