#
# Makefile to build: get_mac_add
#
# Program to automatically obtain the hardware address of a network device (MAC address)
#
# Copyright 2012 Fahlstrom Research LLC
#
# http://www.fahlstrom-research.com
#
# Author : Carl A. Fahlstrom
#
# This software is provided without warranty of any kind.
#
# This software is provided under the terms of the Creative Commons Attribution-NoDerivs 3.0 Unported
# this license is available from http://creativecommons.org/licenses/by-nd/3.0/legalcode
#
# The following terms are added to the CC-BY-ND license:
#
# Attribution is to include a message viable to end users of the program.
# Fahlstrom Research grants permission to make modifications only for personal use or within your organization.
# You may not distribute modified versions or derivative works.
# You may distribute the code unmodified.  Those you redistribute to are also bound by all terms of this license.
# You are not to remove, comment, modify, or cause not to be compile by other means code between:
# //=======Do not remove==========
#         code not to be removed
# //==============================
#
#/

# These are for Centos 6
# Autoconf configuration will be included in the future
# If these do not work make sure gtk-devel is install and use 'pkg-config --cflags gtk+-2.0' and put the output here.
GTK_INCLUDE = -I/usr/include/gtk-2.0 -I/usr/lib64/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng12

# These are for Centos 6
# Autoconf configuration will be include in the future
# If these do not work make sure gtk-devel is install and use 'pkg-config --libs gtk+-2.0' and put the output here.
GTK_LIBS = -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lgdk_pixbuf-2.0 -lpangocairo-1.0 -lcairo -lpango-1.0 -lfreetype -lfontconfig -lgobject-2.0 -lgmodule-2.0 -lglib-2.0

MYSQL_INCLUDE = -I/usr/include/mysql

MYSQL_LIBS = -L/usr/lib64/mysql -lmysqlclient

# Uncomment one of the following lines corresponding the the operating system
#
ARCH_DEF = LINUX
#ARCH_DEF = MACOSX
#ARCH_DEF = WIN

# To add a banner image to the GUI uncomment the following and put an image named banner.png in this directory.
#BANNER_DEF = -DADD_BANNER

# To change the main window title, uncomment the following line and change the 'const char* win_main_title = "";' line in get_mac_add.c to 
# have the title that you want between the quotations.
#
#TITLE_DEF = -DSET_TITLE

CC = gcc

get_mac_add: 
	$(CC) -o get_mac_add $(GTK_INCLUDE) $(MYSQL_INCLUDE) $(GTK_LIBS) $(MYSQL_LIBS) -D$(ARCH_DEF) $(BANNER_DEF) $(TITLE_DEF) get_mac_add.c

all: get_mac_add

clean:
	rm get_mac_add
