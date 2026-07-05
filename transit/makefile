
TIME_EXPANDED=time_expanded.bin
RAPTOR=raptor.bin

#Graf czasowo-rozszerzony
time_expanded: clean_time_expanded
	g++ time_expanded.cpp parser.cpp -o ${TIME_EXPANDED}

clean_time_expanded:
	@rm ${TIME_EXPANDED} || true


raptor: clean_raptor
	g++ raptor.cpp raptor_util.cpp parser.cpp -o ${RAPTOR}

clean_raptor:
	@rm ${RAPTOR} || true

clean: clean_time_expanded clean_raptor

