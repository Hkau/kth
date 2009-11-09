#!/usr/bin/perl

while(<>)
{
	s/\b([A-Z][a-z]+)(?=[A-Z])/\l\1/g;
	s/\B([A-Z][a-z]+)/_\l\1/g;
	print;
}
