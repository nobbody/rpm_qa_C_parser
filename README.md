rpm -qa "C" parser. Author: nobodyless@gmail.com, GPL3 copyright!

Steps TO DO to make the parser operational:
[1] git clone https://github.com/nobbody/rpm_qa_C_parser.git (to pull the package into your local directory);
[2] To compile it: gcc rpm_qa.c -o rpm_qa
[3] To create input file: rpm -qa > packages-list.txt
[4] To execute: ./rpm_qa packages-list.txt package_names.txt (in this case you'll have just packages' names);
[5] To execute: ./rpm_qa packages-list.txt script.sh dnf (in this case you'll have "dnf install " string in front of the each package's name).

The new, simplified version, called rpm_qa_v1.c was added, as successor to rpm_qa.c

Output file as script.sh from step [5] is also presented in this GitHub repo, as output of the parser.

The same could be accomplish with the rpm -qa command, modified as on(e)line script to achieve the same/similar purpose:
rpm -qa --qf "%{name}.%{arch}\n" > packages-list.txt
rpm -qa --qf "%{name}\n" > packages-list.txt
rpm -qa --qf "dnf install %{name}\n" > packages-list.txt

Also could be used for other distros, after original C code re-edit (changing char dnf[] string to match particular Linux distro command, example Debian: dpkg (equivalent to RHL rpm)!

_nobody_
