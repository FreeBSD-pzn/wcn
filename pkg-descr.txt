A word count new (wcn) utility, like a word count (wc) utility UNIX.
----
A wc command appeared in Version 1 AT&T UNIX.
--------------------------------------------------------------------
My version of WORD COUNT (wc) utility is named WORD COUNT NEW (wcn).

Why did I write wcn?
1) I often use a wc and sometimes I need information:
   How many character(s) (for instance 'A') is(are) in the file?
   I have added options -t in *NIX, or /t in VMS to show
   characters table.
2) WC does not has help options and I need to turn to man utility.
   But options -h is a very simple.
   I have added options -h or /h.
3) I have found a very small bags in the wc:
	- if I lost an options in a command line, then
          wc get a file "-".
          You can test it by entering $ wc - -w filename
        - also wc does not has check counter overflow.
          What will be if counter will be overflow I do not know.

So, I have spent a time to write my own version of WORD COUNT.
My goal has beed to write a very simple version of word count
with addition options and without specific libraries.

Oleg Pyzin
p.s.
Please, do not judge me severely. 
