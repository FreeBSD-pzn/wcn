$! Makefile to compile
$! WORD COUNT NEW
$! WORD COUNT NEW VAX STYLE
$ WRITE SYS$OUTPUT "Compile file TABLE.C ..."
$ CC TABLE
$ WRITE SYS$OUTPUT "Compile file WCN.C with *NIX style command line ..."
$ CC WCN
$ WRITE SYS$OUTPUT "Compile file WCN.C with VMS style command line ..."
$ CC /DEFINE=VMS_STYLE/OBJECT=WCNV WCN
$! To create *NIX style command line
$ WRITE SYS$OUTPUT "Linking executable file with *NIX style command line ..."
$ LINK WCN,TABLE
$! To create VMS style command line
$ WRITE SYS$OUTPUT "Linking executable file with VMS style command line ..."
$ LINK WCNV,TABLE
$ EXIT
