# Zestaw 13

## Zadanie 1
Stwórz przykładowy plik *.x z definicjami kilku typów (tylko typów, programów jeszcze nie), przetwórz go przy pomocy rpcgen, przeanalizuj wygenerowane pliki źródłowe C. Upewnij się, że rozumiesz w jaki sposób złożone typy XDR (tablice, ciągi, unie) są przekładane na typy C.

## Zadanie 2
Załóż nowy plik *.x, przepisz do niego ze slajdów definicję usługi sumującej liczby, przeanalizuj to co wygeneruje rpcgen.

Można byłoby zaimplementować serwer tej usługi, ale nie dałoby się go potem uruchomić w wydziałowej pracowni. Współczesne dystrybucje Linuksa zakładają, że jedynymi programami korzystającymi z RPC są demony systemowe, i nie pozwalają procesom nie mającym praw roota rejestrować się w portmapperze.

A skoro nie dałoby się uruchomić serwera, to implementowanie klienta też byłoby bez sensu — nie mielibyśmy jak go przetestować.

## Zadanie 3
W wydziałowej sieci działa za to serwer YP (Yellow Pages; rozszerzona wersja YP znana jest jako NIS, czyli Network Information Service), który m.in. udostępnia centralną bazę kont linuksowych. To dzięki niemu możecie Państwo się logować na dowolnym komputerze, mimo tego że Waszych kont nie ma w lokalnych plikach /etc/passwd.

Proszę zajrzeć do katalogu /usr/include/rpcsvc/, znajdziecie tam pliki RPCL definiujące usługi będące częścią systemu operacyjnego. Nam potrzebny jest plik /usr/include/rpcsvc/yp.x. Proszę go przejrzeć i szczególną uwagę zwrócić na procedurę YPPROC_MATCH — to ona służy do wyszukiwania w bazach YP potrzebnych nam informacji. Co jest podawane jako jej argument i co zwraca?

Sprawdź co wypisują polecenia ypdomainname i ypwhich oraz zawartość plików /etc/defaultdomain i /etc/yp.conf. Czy masz już teraz wszystkie informacje niezbędne aby wypełnić strukturę ypreq_key?

## Zadanie 4 
Skopiuj /usr/include/rpcsvc/yp.x, uruchom rpcgen. Napisz klienta wywołującego funkcję ypproc_match_2 w celu znalezienia informacji o Twoim własnym koncie oraz o koncie „nowak_123” (nie ma takiego, powinien więc zostać zwrócony błąd YP_NOKEY).