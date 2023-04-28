# Zestaw 8

## Zadanie 1
Napisz program sprawdzający, czy pewna określona witryna działa poprawnie. Użyj C/C++, Javy lub Pythona wraz z odpowiednią biblioteką obsługującą wysyłanie zapytań HTTP. Program ma się dawać skompilować i uruchomić na spk-ssh.

Sprawdzenie ma polegać na pobraniu strony spod ustalonego adresu (np. spod http://th.if.uj.edu.pl/). Proszę nie zapomnieć o zweryfikowaniu, czy na pewno udało się ją poprawnie pobrać (status 200) i czy to jest strona HTML (typ text/html). Następnie należy sprawdzić, czy rzeczywiście jest to spodziewana strona, a nie np. komunikat o wewnętrznym błędzie serwera WWW — to można zweryfikować sprawdzając czy w pobranej treści znajduje się pewien zadany z góry ciąg znaków (np. „Institute of Theoretical Physics”).

Program, w zależności od wyniku sprawdzenia, musi zwracać jako wynik funkcji main kod sukcesu (zero) bądź porażki (wartość większa od zera). Osoby piszące w Pythonie powinny użyć sys.exit(0) albo sys.exit(1), a w Javie należy wywołać metodę System.exit z odpowiednim argumentem.

Programy tego typu używane są w systemach monitorowania usług sieciowych. Jeśli na filmie z centrum zarządzania siecią widać ekran z listą serwerów i usług, a przy nich zielone komunikaty „OK” i gdzieniegdzie czerwone komunikaty błędów, to za tymi kolorami kryją się uruchamiane w regularnych odstępach czasu programy sprawdzające status danej usługi.

## Zadanie 2
Dokumenty, które są przeznaczone nie dla oczu użytkowników, lecz dla programów, często są udostępniane w formacie JSON. Sprawdź, czy w Twoim ulubionym języku programowania jest standardowa biblioteka pozwalająca serializować i deserializować dane w tym formacie. Zapoznaj się z nią, spróbuj znaleźć przykład pokazujący zamianę ciągu bajtów / znaków na struktury danych tego języka programowania (czyli deserializację ciągu).