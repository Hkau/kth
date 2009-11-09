#!/usr/bin/perl
s/([^"]+)"(?=(([^"]*"[^"]*")*[^"]*$))/print "$1\n"/gee while(<>);
print "\n";
