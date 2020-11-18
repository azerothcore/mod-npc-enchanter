#!/usr/bin/env bash

MOD_NPCENCHANTER_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/" && pwd )"

source "$MOD_NPCENCHANTER_ROOT/conf/conf.sh.dist"

if [ -f "$MOD_NPCENCHANTER_ROOT/conf/conf.sh" ]; then
    source "$MOD_NPCENCHANTER_ROOT/conf/conf.sh"
fi 