# LOGBOOK

## Time-Expanded

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

## RAPTOR

### Log 1
Przede wszystkim to potrzebujemy tutaj obliczać przejścia piesze, więc przydałoby się umieć szacować odległości między przystankami - [haversine](https://en.wikipedia.org/wiki/Haversine_formula).

### Log 2
Pomimo paru błędów, jednak udało się naprawić nieścisłości w implementacji haversine.

### Log 3
Potrzebujemy szacować czas przejść pieszych. Propozycja jest taka, by obliczyć odległość pomiędzy przystankami i podzielić przez prędkość pieszego - `raptor_util.cpp:walkingSpeed`. Prędkość dobrana jako specjalnie mała, żeby ukryć oszustwa związane z pominięciem ... wszystkiego poza odległością w linii prostej.

### Log 4
Tak samo jak przy poprzednim algorytmie, tu również rozważamy tylko kursy najpopularniejszego serwisu (tj. tego, który ma najwięcej kursów).

### Log 5
Parserka (do) tablic używanych w algorytmie zrobiona, ale z kilkoma założeniami, o różnych stopniach oszustwa:
1. W `stop_times.txt` nie ma kursów (trip_id), które nie występują w `trips.txt`
1. Pojazd nie cofa się w czasie docierając do kolejnych przystanków danego kursu.
1. Każdy kurs danej trasy ma dokładnie takie same różnice w czasach przyjazdów.
1. Docierając na przystanek interesuje nas czas przyjazdu (arrival_time), ale gdy próbujemy złapać przesiadkę, to pytamy od czas odjazdu (departure_time) tego drugiego środka transportu. Tu jest inaczej - przyjmuję, że (potencjalna) różnica tych czasów będzie mniej znacząca niż ustalony czas na przesiadkę i korzystam tylko z czasów przyjazdów.


### Log 6
Udało się znaleźć błąd w wyznaczaniu tras - jedna trasa ma dwa (różne!) kierunki. Rozwiązanie - dokleić kierunek do route_id. Po takiej poprawde czasy odjazdów w tabelce już wydają się realne. Na szczęście taka poprawka nic nie zmienia w rozwiązaniu `Time-Expanded`, bo nie korzysta ono z pola `route_id`.

