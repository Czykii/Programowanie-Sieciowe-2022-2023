# Zestaw 12

## Zadanie 1
Uważnie przeanalizuj i uruchom przykłady demonstrujące implementowanie REST-owych usług w środowisku WSGI: hello_webapp.py, rest_webapp.py, rest_webapp.sh.

Zwróć uwagę na to, w jaki sposób metody klasy OsobyApp nawzajem się wywołują w zależności od tego, jakie zapytanie przyszło od klienta. Część z nich, np. sql_select, jest wywoływana z kilku różnych miejsc. Jeśli się w tym zgubisz spróbuj rozrysować na kartce papieru graf wywołań, to powinno pomóc.

Uruchom rest_webapp.py bezpośrednio, bez pomocy załączonego skryptu. Wyślij do uruchomionego serwera kilka zapytań i sprawdź, czy dostajesz takie odpowiedzi, jakich się spodziewałaś(-eś).

## Zadanie 2
Spróbuj znaleźć w powyższym kodzie rzeczy, które serwer przyjmuje na wiarę, bez weryfikacji. Zastanów się, czy napastnik mógłby je wykorzystać aby włamać się do serwera lub w inny sposób zakłócić jego pracę.

## Zadanie 3
Rozszerz aplikację tak, aby można było wyszukiwać osoby o zadanym imieniu i / lub nazwisku (tzn. zaimplementuj obsługę URL-i postaci /osoby/search?imie=Adam&nazwisko=Nowak).

## Zadanie 4 
##### (alternatywa dla powyższego zadania, jeśli ktoś nie lubi Pythona)

Sprawdź, czy Twój ulubiony język programowania wspiera tworzenie serwerów REST. Jeśli tak, odszukaj i przejrzyj przykład takiego serwera, albo i dwa przykłady. Jak przekazywane są informacje o szczegółach zapytania, i jak zwracana jest wygenerowana odpowiedź? Zaimplementuj w tym środowisku odpowiednik hello_webapp.py.

## Zadanie 5
Przejrzyj dokumentację pythonowych modułów xmlrpc.client i xmlrpc.server. Zorientuj się (w ogólnych zarysach) jak w Pythonie pisze się programy korzystające z XML-RPC.

## Zadanie 6
Zastanów się, w jaki sposób można byłoby przepisać naszą przykładową bazę osób z REST na XML-RPC. Jak musiałby zmienić się jej interfejs? Czym zastąpić przesyłanie dokumentów w formacie TSV?

## Zadanie 7
(nieobowiązkowe) Spróbuj dokonać takiej reimplementacji. Nie musi być pełna, wystarczy zaimplementować obsługę jednej-dwóch zdalnych metod; już to powinno Państwu pozwolić opanować podstawy korzystania z bibliotek, które postanowicie wykorzystać do napisania serwera XML-RPC