#!/usr/bin/perl

while(<>)
{
	chomp; #tar bort skräptecken i slutet (\n)
	$line = "";
	foreach(reverse(split(/\t/)))
	{
		$line = $line."$_\t";
	}
	$line =~ s/\t$//; #ta bort trailing \t

	print "$line\n";
}

