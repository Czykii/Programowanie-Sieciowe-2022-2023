# Zestaw 6

## Zadanie 1
Zapoznaj się z poniższą specyfikacją strumieniowego protokołu obliczania wartości wyrażeń:

Komunikacja pomiędzy klientem a serwerem odbywa się przy pomocy połączenia strumieniowego. Klient wysyła jedną lub więcej linii zawierających wyrażenia. Dla każdej odebranej linii serwer zwraca linię zawierającą pojedynczą liczbę (obliczony wynik) albo komunikat o błędzie.

Ogólna definicja linii jest zapożyczona z innych protokołów tekstowych: ciąg drukowalnych znaków ASCII (być może pusty) zakończony dwuznakiem \r\n.

Linia z wyrażeniem może zawierać tylko cyfry oraz znaki plusa i minusa. Postać wyrażeń jest dokładnie taka, jak w poprzednio rozważanym protokole datagramowym (a więc wyrażenie musi zawierać przynajmniej jedną liczbę itd.).

Linia z odpowiedzią serwera zawiera albo opcjonalny znak minusa i niepusty ciąg cyfr, albo pięć liter składających się na słowo „ERROR”.

Wszystkie linie, i te wysyłane przez klientów, i przez serwer, mają oczywiście do opisanej powyżej zawartości dołączony terminator linii, czyli \r\n.

Serwer może, ale nie musi, zamykać połączenie w reakcji na nienaturalne zachowanie klienta. Obejmuje to wysyłanie danych binarnych zamiast znaków ASCII, wysyłanie linii o długości przekraczającej przyjęty w kodzie źródłowym serwera limit, długi okres nieaktywności klienta, itd. Jeśli serwer narzuca maksymalną długość linii, to limit ten powinien wynosić co najmniej 1024 bajty (1022 drukowalne znaki i dwubajtowy terminator linii).

Serwer nie powinien zamykać połączenia gdy udało mu się odebrać poprawną linię w sensie ogólnej definicji, ale dane w niej zawarte są niepoprawne (np. oprócz cyfr i operatorów arytmetycznych są przecinki). Powinien wtedy zwracać komunikat o błędzie i przechodzić do przetwarzania następnej linii przesłanej przez klienta.

Serwer powinien zwracać komunikat błędu gdy przesłane przez klienta liczby bądź wyniki prowadzonych na nich obliczeń przekraczają zakres typu całkowitoliczbowego wykorzystywanego przez serwer.

## Zadanie 2
Napisz serwer TCP/IPv4 domyślnie nasłuchujący na porcie nr 2020 i implementujący powyższy protokół. Serwer musi być w stanie równocześnie obsługiwać co najmniej 100 połączeń. Użytym językiem może być C albo C++. Proszę pisać kod tak, aby się kompilował bez ostrzeżeń odpowiednio pod gcc -std=c99 -pedantic -Wall bądź g++ -std=c++17 -pedantic -Wall.