Perl-OIS version 0.04
=====================

This is a Perl binding for OIS, Object-Oriented Input System,
a crossplatform C++ input framework, found at
http://sourceforge.net/projects/wgois . I made it so that
Ogre (http://search.cpan.org/~slanning/Ogre/ ) can be useful,
but there might be uses outside of Ogre.

The wrapping is currently fairly complete, but I'm focusing on being
able to use it with Ogre. (OIS::Component and OIS::ForceEffect, and their
subclasses are basically the only parts not wrapped.)

There is no documentation yet, which is no doubt frustrating.
Then again, OIS itself isn't really very documented. :)
There are examples in the Ogre module, and you can look at
the (inadequate) tests under the t/ directory.


DEPENDENCIES

You should install the latest version of OIS. That's 1.0 as I'm writing this.
If you run Ubuntu, see below for installation instructions.

Makefile.PL uses pkg-config to get information about the libraries and header
files needed to build against OIS, so you should be able to do this:

  pkg-config --libs OIS
  pkg-config --cflags OIS
  pkg-config --modversion OIS

This latter should say at least 1.0.0.

The C++ compiler used by default is `g++`, but you can specify a different
C++ compiler by setting the CXX environmental variable. Anything more,
and you'll have to hack at Makefile.PL.


INSTALLATION

To install this module, do the usual:

   perl Makefile.PL
   make
   make test
   make install

You might have to edit Makefile.PL to get it to work for your system.
If so, please let me know.


INSTALLING OIS UNDER UBUNTU

To install OIS in Ubuntu,

  $ sudo apt-get install libois1 libois-dev

In `dpkg -l libois-dev`, I have version 0.99+1.0rc1-1,
though `pkg-config --modversion OIS` shows 1.0.0.


COPYRIGHT AND LICENCE

Please report any bugs/suggestions to <slanning@cpan.org>

Copyright 2007, Scott Lanning. All rights reserved.

This program is free software; you can redistribute it and/or modify
it under the same terms as Perl itself.

OIS itself is under the "zlib/libpng" license. See the ReadMe.txt file
in OIS's source distribution for more (and probably more accurate) information.
