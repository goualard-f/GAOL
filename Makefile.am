#-*-Makefile-*-----------------------------------------------------------------
#  gaol -- Just Another Interval Library
#------------------------------------------------------------------------------
# This file is part of the gaol distribution. Gaol was primarily 
# developed at the Swiss Federal Institute of Technology, Lausanne, 
# Switzerland, and is now developed at the Institut de Recherche 
# en Informatique de Nantes, France.
#
# Copyright (c) 2001 Swiss Federal Institute of Technology, Switzerland
# Copyright (c) 2002 Institut de Recherche en Informatique de Nantes, France
#------------------------------------------------------------------------------
# gaol is a software distributed WITHOUT ANY WARRANTY. Read the associated 
# COPYING file for information.
#------------------------------------------------------------------------------
# Note: To be used with GNU Make                                              
# If this file has the name "Makefile.in" then it is a template for a         
# Makefile; to generate the actual Makefile, run "./configure" from the base  
# directory of gaol.                                                          
#------------------------------------------------------------------------------
# CVS: $Id: Makefile.am,v 1.12 2007/09/13 07:51:26 goualard Exp $
# Last modified: Wed Sep 27 10:55:49 2006 on pc-goualard.lina.sciences.univ-nantes.prive
# By: Frederic Goualard <Frederic.Goualard@lina.univ-nantes.fr>
#------------------------------------------------------------------------------


EXTRA_DIST = BUGS
SUBDIRS = gaol check examples
DIST_SUBDIRS = gaol check doc examples
ACLOCAL_AMFLAGS = -I m4

# Requires automake 1.4
AUTOMAKE_OPTIONS = 1.4

# Rule to build tar-gzipped distribution package
$(PACKAGE)-$(VERSION).tar.gz: dist


# Rule to build RPM distribution package
rpm: $(PACKAGE)-$(VERSION).tar.gz
	rpmbuild -ta --clean $(PACKAGE)-$(VERSION).tar.gz

