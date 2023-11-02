#
# Copy the right things into the right places as part of install. This
# will be run from the current directory
#

RSRC	= .
SYSDIR	= /usr/local/atari/System
DISK	= $(SYSDIR)/../Disk
DESKTOP	= $(DISK)/Applications/desktop.app

CP		= /bin/cp
RM		= /bin/rm -f
LN 		= /bin/ln -s
MKDIR	= /bin/mkdir -p

install:
	@ $(CP) $(RSRC)/system.ttf $(SYSDIR)/Fonts/system.ttf
	@ $(CP) $(RSRC)/cicons.rsc $(DESKTOP)/icons.rsc
	@ $(CP) $(RSRC)/{gem,nvdi}.pal $(SYSDIR)/Palettes
	@ (cd $(SYSDIR)/Palettes && $(RM) system.pal && $(LN) gem.pal system.pal)
	@ $(MKDIR) $(DESKTOP)
	@ $(CP) $(RSRC)/newdesk.inf $(DESKTOP)
	@ echo "System installed"
	