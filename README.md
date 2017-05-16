Author: nobodyless@gmail.com, GPL2 copyright!

To compile: gcc rpm_qa.c -o rpm_qa
To execute: /rpm_qa infile outfile [dnf] (dnf string optional)

With 3rd argument addition, it will add "dnf install " string
to the outfile.

Copyright _nobody_, GPL2. rpm -qa parser, which makes from
the command rpm -qa > input_file.txt simple bash shell script
in the case somebody needs to reinstall from schratch Fedora
distro, and import all the packages into new one.

Also could be used for other distros, after original C code
re-edit (changing char dnf[] string to match particular Linux
distro command, example Debian: dpkg (equivalent to RHL rpm)!

_nobody_
