
TIME_EXPANDED=time_expanded.bin
RAPTOR=raptor.bin

#Graf czasowo-rozszerzony
time_expanded: clean
	g++ time_expanded.cpp parser.cpp -o ${TIME_EXPANDED}

raptor: clean
	g++ raptor.cpp raptor_util.cpp parser.cpp -o ${RAPTOR}

clean:
	@rm ${TIME_EXPANDED} || true
	@rm ${RAPTOR} || true

