#!/bin/awk -f

# input format from gnome-config-listener
# "<key>" "<value>" <type> default|set
# "/apps/gnome-terminal/profiles/Default/use_system_font" "1" bool default
# "/apps/gnome-terminal/profiles/Default/use_system_font" "0" bool set

# output for yaml format used by Chef recipe
# gconftool-2 -t <type> -s <key> 'value'
# gconftool-2 -t bool -s /apps/nautilus/desktop/home_icon_visible 'false'

BEGIN {
  FS = "\""

  print "  gnome-config-preferences"
}

{
  if ($8 != "default") {
    print "    - -", $2
    print "      -", $4
    print "      -", $6
  }
}

END {
  print ""
}
