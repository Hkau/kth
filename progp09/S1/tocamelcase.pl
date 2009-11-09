#!/usr/bin/perl

while(<>)
{
	s/([a-z]+)_|(?<=[_])([a-z]+)/\u\1\2/g;
	print;
}
