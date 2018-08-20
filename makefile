nexpr: y.tab.o main.o lex.yy.o utils.o error.o help.o strings.o keywords.o \
	func.o cmdhdlr.o cmds.o format.o io.o control.o
	cc -g -o nexpr y.tab.o main.o utils.o error.o lex.yy.o help.o \
		strings.o keywords.o func.o cmdhdlr.o cmds.o format.o io.o \
		control.o -lm

lex.yy.o: lex.yy.c  y.tab.h nexpr_types.h error.h
	cc -g -c lex.yy.c

lex.yy.c: nexprlexer.l
	lex nexprlexer.l

y.tab.o: y.tab.c nexpr_types.h error.h utils.h func.h strings.h format.h io.h
	cc -g -c y.tab.c

y.tab.c: nexpr.y 
	#yacc -d --debug --verbose nexpr.y
	yacc -d nexpr.y

main.o: main.c error.h y.tab.h nexpr_types.h utils.h strings.h func.h \
	cmd_defs.h basics.h format.h io.h
	cc -g -c main.c

utils.o: utils.c nexpr_types.h y.tab.h error.h basics.h main.h format.h io.h
	cc -g -c utils.c

error.o: error.c error.h io.h
	cc -g -c error.c

help.o: help.c io.h
	cc -g -c help.c

strings.o: strings.c nexpr_types.h y.tab.h error.h utils.h keywords.h basics.h \
	main.h format.h io.h
	cc -g -c strings.c

keywords.o: keywords.c error.h y.tab.h nexpr_types.h basics.h
	cc -g -c keywords.c

func.o: func.c error.h utils.h keywords.h strings.h basics.h io.h
	cc -g -c func.c

cmdhdlr.o: cmdhdlr.c cmdhdlr.h cmd_defs.h error.h format.h help.h
	cc -g -c cmdhdlr.c

cmds.o:	cmds.c cmd_defs.h basics.h keywords.h error.h nexpr_types.h strings.h \
	format.h
	cc -g -c cmds.c

format.o: format.c io.h main.h
	cc -g -c format.c

io.o: io.c basics.h
	cc -g -c io.c

control.o: control.c
	cc -g -c control.c

clean:
	rm -f y.tab.c y.tab.h y.tab.o main.o lex.yy.o lex.yy.c utils.o error.o \
		help.o strings.o keywords.o func.o cmdhdlr.o cmds.o format.o \
		io.o control.o y.output
