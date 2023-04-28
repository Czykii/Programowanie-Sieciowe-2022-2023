# Zestaw 3

## Zadanie 1
Dokończ pisanie par klient-serwer dla TCP/IPv4 oraz UDP/IPv4 (co razem daje cztery programy). Przetestuj czy działają poprawnie gdy klient i serwer są uruchomione na dwóch różnych komputerach w SPK. Wymaga to znajomości adresu IP przydzielonego komputerowi, na którym uruchamiany jest serwer — można go znaleźć w wynikach polecenia ip address show.

## Zadanie 2
Sprawdź co się dzieje, gdy podasz zły adres IP albo zły numer portu serwera. Czy jądro systemu operacyjnego daje nam w jakiś sposób o tym znać? Jeśli tak, to jak długo trzeba czekać, aż jądro poinformuje nasz proces o wystąpieniu błędu?

Pamiętaj, że protokoły sieciowe z korekcją błędów wykonują wielokrotne retransmisje pakietów w zwiększających się odstępach czasu. Może to zająć nawet kilkadziesiąt minut. Nie pomyl sytuacji „proces zawiesza się na pięć minut zanim jądro zwróci -1” z sytuacją „zawiesza się na stałe”.

Jeśli któryś z klientów może się zawiesić czekając w nieskończoność na odpowiedź z nieistniejącego serwera, to popraw jego kod aby tego nie robił. W slajdach z wykładu są pokazane funkcje, które pozwalają na wykonywanie operacji we-wy z timeoutem (możecie go Państwo ustawić np. na 10 sekund).

## Zadanie 3
Przeanalizuj niniejszą specyfikację protokołu dodawania i odejmowania liczb naturalnych. Czy jest ona jednoznaczna, czy też może zostawia pewne rzeczy niedopowiedziane?

Komunikacja pomiędzy klientem a serwerem odbywa się przy pomocy datagramów. Klient wysyła datagram zawierający wyrażenie zbudowane z liczb i operatorów dodawania / odejmowania. Serwer odpowiada datagramem zawierającym pojedynczą liczbę (obliczony wynik) bądź komunikat o błędzie.

Zawartość datagramów interpretujemy jako tekst w ASCII. Ciągi cyfr ASCII interpretujemy jako liczby dziesiętne. Datagram może zawierać albo cyfry, plusy i minusy, albo pięć znaków składających się na słowo „ERROR”. Żadne inne znaki nie są dozwolone (ale patrz następny akapit).

Aby ułatwić ręczne testowanie serwera przy pomocy ncat, serwer może również akceptować datagramy mające na końcu dodatkowy znak \n (czyli bajt o wartości 10) albo dwa znaki \r\n (bajty 13, 10). Serwer może wtedy, ale nie musi, dodać \r\n do zwracanej odpowiedzi.

## Zadanie 4
Napisz serwer UDP/IPv4 nasłuchujący na porcie nr 2020 i implementujący powyższy protokół.

Serwer musi weryfikować odebrane dane i zwracać komunikat o błędzie jeśli są one nieprawidłowe w sensie zgodności ze specyfikacją protokołu.

W kodzie używaj zmiennych roboczych któregoś ze standardowych typów całkowitoliczbowych (int, long int, int32_t, itd.). Co za tym idzie, odebrany ciąg cyfr będzie mógł reprezentować liczbę zbyt dużą, aby dało się ją zapisać w zmiennej wybranego typu. Może też się zdarzyć, że podczas dodawania bądź odejmowania wystąpi przepełnienie (ang. integer overflow / wraparound). Serwer ma obowiązek wykrywać takie sytuacje i zwracać błąd. Uwadze Państwa polecam pliki nagłówkowe limits.h oraz stdint.h, w których znaleźć można m.in. parę stałych INT_MIN i INT_MAX oraz parę INT32_MIN i INT32_MAX.

# Podsumowanie analizy specyfikacji z zajęć nr 3
Dyskusje w grupach wyłapały wszystkie ważne niejasności. Poniżej są spisane podjęte decyzje, proszę je uwzględnić przy implementowaniu serwera.

Jak wyglądają wyrażenia? Czy są może w odwrotnej notacji polskiej?
Wyrażenia są w notacji infiksowej, czyli codziennej. Poprawne wyrażenia to np. 2+2 lub 45+1044-512+0+28-0077.

Nie będziemy pozwalać na używanie plusa w roli znaku liczby, może on występować tylko w roli operatora dodawania. Co za tym idzie, +2+2, 100++15 itp. są niepoprawnymi zapytaniami.

Inne przykłady niepoprawnych wyrażeń: 2 + 2 (nie wolno używać spacji), -20+60 (rozważamy protokół dodawania i odejmowania liczb naturalnych, a -20 nie jest liczbą naturalną).

Odejmowanie liczb naturalnych może dać wynik nie będący liczbą naturalną. Co wtedy?
Można było albo zdecydować się na zwracanie błędu, albo dopuścić ujemne wyniki. Wybraliśmy tę drugą możliwość. Do specyfikacji trzeba dopisać „wyniki zwracane przez serwer są liczbami całkowitymi”.

Czy wyrażenie zawierające tylko jedną liczbę, bez żadnego operatora, jest dopuszczalne? A wyrażenie puste, czyli zapytanie długości zero bajtów?
Wyrażenia z jedną liczbą będziemy traktować jako poprawne. Wyrażenia puste mają powodować zwrócenie przez serwer komunikatu o błędzie.

Jak wielkie mogą być liczby? Czy jest może jakiś limit, np. nie więcej niż dziesięć cyfr?
Nie będziemy narzucali ograniczeń na to, co może wysyłać klient, postawimy za to pewne minimalne wymagania serwerowi.

Aby serwer mógł być uznany za poprawnie implementujący protokół, musi być on w stanie poprawnie obliczać wartości wyrażeń, w których wszystkie liczby, wynik końcowy i wyniki pośrednie mieszczą się w zakresie typu int16_t. Serwer może używać do obliczeń typu o większym zakresie wartości.

Co za tym idzie, po wysłaniu serwerowi 2+2 albo 10000+20000 na pewno dostaniemy w odpowiedzi poprawnie obliczoną sumę, ale gdy wyślemy 30000+30000 możemy dostać albo 60000, albo ERROR.

Czy jest jakiś limit na długość zapytań-datagramów wysyłanych przez klienta?
Nie, na poziomie naszego protokołu aplikacyjnego nie. Natomiast datagramowe protokoły transportowe, z których będzie on korzystał, będą miały jakieś swoje własne limity. W szczególności UDP ma limit ciut poniżej 64 KiB.