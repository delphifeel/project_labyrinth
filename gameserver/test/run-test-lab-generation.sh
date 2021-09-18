clang -Wall -pedantic -g \
	../lib/disjoint-set.c 	\
	../lib/CORE/CORE-system.c 	\
	../src/lab-session.c 	\
	../src/lab-points-map.c 	\
	../src/lab-points-map-reader.c 	\
	../src/lab-generation.c 	\
	../src/player.c 	\
	../src/commands-listener.c 	\
	test-lab-generation.c 	\
&& ./a.out
