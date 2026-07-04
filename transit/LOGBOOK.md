# LOGBOOK

### Log 1
Parserka plików GTFS'owych zrobiona, structy dla wybranych danych również.

### Log 2
Klepanie grafu czasowo-rozszerzonego okazuje się nie być tak proste, nie mam pomysłu jak napisać to w jednym przejściu listy `times`.

### Log 3
Rzeczywiście, jest to lekka męczarnia.

### Log 4
Klepanie grafu zakończone, pora na testowanie.

### Log 5
Pisanie zakończone, chyba pomyślnie. Parę testów sugeruje, że kod działa. Choć wymaga wytłumaczenia:

1. Kompilacja jest poprzez makefile (dalsze szczegóły tam).
1. Czas jest liczony w sekundach od północy. Może wyjeżdżać poza `86400` (dobę), bo tak są kodowane autobusy jadące podczas północy w GTFS.
1. Czas na przesiadkę jest arbitralnie ustawiony na `179` sekund, patrz `time_expanded.cpp:changeTime`.
1. Obecne dane to tramwaje w Krakowie, w jeden dzień.
1. Wybrany jest tylko jeden serwis - ten, który ma najwięcej tripów.
1. NIE MA CHODZENIA. Ten sam przystanek, ale o innym numerze (np. po drugiej stronie ulicy) jest stricte innym przystankiem, na który trzeba osobno dojechać. Przez to tworzą się kwiatki w stylu (zapytanie) `21 [35, 39] <czas>`, czyli przejazd z "Dworzec Towarowy 3" na "Rondo Mogilskie [1, 5]", gdzie może występować znaczna różnica w czasie dojazdu (między przystankami odległymi o może 3 minuty spacerku).

Miłej zabawy!

