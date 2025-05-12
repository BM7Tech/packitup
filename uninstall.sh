#!/bin/env bash

uninstall_packitup()
{
    ninja -C "$build_dir" uninstall
    gtk4-update-icon-cache -f -t "$prefix"/share/icons/hicolor
}

[[ $# -eq 0 ]] && echo "You must pass the build directory path E.g. ./uninstall.sh --build builddir" && exit 1

build_dir=""
prefix=""
refresh_icons=""

while [[ $# -gt 0 ]]; do
    case "$1" in
	-p | --prefix)
		prefix="$2"
		shift 2 ;;
	-b | --build)
		build_dir="$2"
		shift 2 ;;
	*)
	    echo "Invalid option $1: Usage: ./uninstall.sh --prefix <your prefix(optional)> --build <your build directory (mandatory)>" 
	    exit 1 ;;
    esac
done

[[ -z $build_dir ]] && echo "You must pass the build directory path E.g. ./uninstall.sh --build builddir" && exit 1

[[ -z $refresh_icons ]] && refresh_icons="$( readlink -f "$(command -v refresh-icons-cache.sh)")"

[[ -z $prefix ]] && prefix="$(dirname "$( dirname "$refresh_icons")")"

uninstall_packitup
