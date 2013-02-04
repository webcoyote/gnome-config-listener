#!/bin/awk -f

# input format from gnome-config-listener
# "<key>" "<value>" <type> default|set
# "/apps/gnome-terminal/profiles/Default/use_system_font" "1" bool default
# "/apps/gnome-terminal/profiles/Default/use_system_font" "0" bool set

# output for gconftool-2
# gconftool-2 -t <type> -s <key> 'value'
# gconftool-2 -t bool -s /apps/nautilus/desktop/home_icon_visible 'false'

BEGIN {
  FS = "\""
}

{
  if ($8 != "default")
    print "gconftool-2 -t", $6, "-s", $2, "'" $4 "'"
}