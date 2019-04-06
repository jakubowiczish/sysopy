# Zadanie - zestaw 3
Tworzenie procesów. Środowisko procesu, sterowanie procesami.
## Zadanie 1. Drzewo procesów (15%)
Modyfikując zadanie 2 z poprzedniego zestawu, napisz program, który dla każdego z podkatalogów utworzy proces potomny i wywoła polecenie ls -l. Wynik ls poprzedź wypisaniem ścieżki względnej od katalogu podanego jako argument oraz numeru PID procesu odpowiedzialnego za przeglądanie określonego poziomu.

## Zadanie 2. Kopie zapasowe plików (50%)
Napisz program monitor do robienia kopii zapasowych plików, zawartych w pliku lista podawanym jako argument programu.  Plik lista zawiera w pojedynczej linii nazwę monitorowanego pliku wraz ze ścieżką względną bądź bezwzględną oraz liczbę, co ile sekund plik ma być monitorowany. Dla każdego pliku z lista program monitor tworzy odpowiednią liczbę procesów potomnych, aby każdy monitorował jeden plik. Drugim argumentem programu monitor jest czas monitorowania wyrażony w sekundach. Po upłynięciu tego czasu każdy z procesów potomnych kończy swoje działanie, zwracając do procesu macierzystego poprzez kod wyjścia procesu liczbę wykonanych kopii pliku. Program macierzysty pobiera statusy procesów potomnych, wypisuje raport: "Proces PID utworzył n kopii pliku" i  kończy swoje działanie. UWAGA! Nie używamy sygnałów, które są tematem następnych zajęć.

Kopiowanie pliku może odbywać się na dwa sposoby: (tryb - trzeci argument programu)

Proces kopiuje do pamięci zawartość monitorowanego pliku oraz datę jego ostatniej modyfikacji, po czym jeśli data modyfikacji pliku (a tym samym zawartość pliku) zostanie zmieniona, to proces zapisuje skopiowaną do pamięci starą wersję pliku we wspólnym dla wszystkich podkatalogu archiwum a do pamięci zapisuje aktualną wersję pliku.
Plik nie jest zapisywany w pamięci, natomiast na samym początku oraz po każdej zanotowanej modyfikacji pliku, proces utworzy nowy proces, który wywoła jedną z funkcji z rodziny exec, aby wykonać cp.
Nazwa pliku utworzonej kopii to nazwa monitorowanego pliku, rozszerzona o datę modyfikacji pliku w formacie: _RRRR-MM-DD_GG-MM-SS.

Napisz pomocniczy testowy program tester, przyjmujący argumenty: plik, pmin pmax i bytes, który do zadanego jako pierwszy argument pliku z losową częstością od pmin do pmax wyrażoną w sekundach dopisuje na jego koniec linię tekstu zawierającą pid procesu, wylosowaną liczbę sekund, aktualną datę i czas  (nie liczą się do liczby bajtów) oraz dowolnego ciągu znaków o długości określonej w bytes. Program ten pozwoli w krótkim czasie wygenerować kilka wersji kopii zapasowych. 

## Zadanie 3. Zasoby procesów (35%)
Zmodyfikuj program z Zadania 2 tak, aby każdy kopiujący proces miał nałożone pewne twarde ograniczenie na dostępny czas procesora oraz rozmiar pamięci wirtualnej. Wartości tych ograniczeń (odpowiednio w sekundach i megabajtach) powinny być przekazywane jako czwarty i piąty argument wywołania monitora. Ograniczenia powinny być nakładane przez proces potomny, w tym celu należy użyć funkcji setrlimit. Zakładamy, że wartości nakładanych ograniczeń są dużo niższe (t.j. bardziej restrykcyjne) niż ograniczenia, które system operacyjny narzuca na użytkownika uruchamiającego monitor.

Zaimplementuj w monitorze raportowanie zużycia zasobów systemowych dla każdego procesu potomnego, w tym czas użytkownika i czas systemowy. Realizując tę część zadania zwróć uwagę na funkcję getrusage i flagę RUSAGE_CHILDREN.
