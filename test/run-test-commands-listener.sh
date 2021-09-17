clang -Wall -pedantic -g \
	../lib/disjoint-set.c 	\
	../lib/CORE/CORE-system.c 	\
	../src/lab-session.c 	\
	../src/lab-points-map.c 	\
	../src/lab-points-map-reader.c 	\
	../src/lab-generation.c 	\
	../src/player.c 	\
	../src/commands-listener.c 	\
	test-commands-listener.c 	\
&& ./a.out