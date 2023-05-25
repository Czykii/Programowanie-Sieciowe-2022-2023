# Zestaw 11

## Zadanie 1
Napisz program, który w oparciu o informacje z Discogs sprawdza, czy członkowie (aktualni bądź byli) danego zespołu grali razem w jakichś innych zespołach. Wyświetl nazwy tych innych zespołów oraz nazwiska muzyków. Załóż, że numeryczny identyfikator zespołu podawany jest jako argv[1].

Dla przykładu: 359282 to Budka Suflera, według Discogs grali w niej Mieczysław Jurecki (id 702387) i Marek Stefankiewicz (id 532854), którzy grali też w Perfekcie (id 669348).

Zadbaj aby drukowane wyniki były posortowane po nazwie zespołu, oraz aby w przypadku więcej niż dwóch osób grających razem w innym zespole kompresować wyniki (np. w wypadku czterech osób grających w innym zespole X należy je wypisać jako pojedynczą czwórkę, a nie jako sześć par).

## Zadanie 2
(nieobowiązkowe) Rozszerz powyższy program tak, aby jako argument można było podawać nie tylko numer, lecz również nazwę zespołu. Najprawdopodobniej będzie wymagać to zarejestrowania się na witrynie Discogs w celu uzyskania tokena dla Państwa aplikacji, bo operacji wyszukiwania zespołu po nazwie zdaje się nie można wywoływać anonimowo.