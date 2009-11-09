#!/usr/bin/perl
print ((/^[a-zA-Z0-9_!#$%*\/?|^{}`~&'+\-=]+(\.[a-zA-Z0-9_!#$%*\/?|^{}`~&'+\-=]+)*\@([a-zA-Z0-9]+\.)+(com|org|net|nu|se)$/)?"ja\n":"nej\n") while(<>);
