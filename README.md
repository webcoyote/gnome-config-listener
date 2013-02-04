# Gnome-config-watcher

Run this program to monitor configuration changes made by programs which use the gnome GConf library.

# Why?

I use Chef to configure my computers. Instead of manually configuring all the settings which make each of my computers "perfect" I can monitor the changes on one computer and programmatically configure the others.

This program enables me to discover the settings I'm changing.

# Build

    ./configure
    make

# Run

    ./gnome-config-listener | tee -i config.log

Now make changes to gnome application configuration settings. When you're done, press Ctrl-C to halt the application. Note that I "tee" the output into "config.log" so I can see the results while I'm making changes.

You can also poke around using the gconf-editor tool and make changes there. On RedHat-based systems:

    sudo yum install -y gconf-editor


# Convert

You can use the format-* scripts to convert the output from your text file into a useful format for processing.

To process using bash:

    ./format-gconftool-2.sh config.log | bash

To convert into a format for soloist gem:

    ./format-yaml.sh config.log


# Author

Author:: Patrick Wyatt (pat@codeofhonor.com)

MIT license
