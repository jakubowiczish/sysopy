# Zadania - zestaw 1.

Zarządzanie pamięcią, biblioteki, pomiar czasu 

## Zadanie 1. Alokacja tablicy z wskaźnikami na bloki pamięci zawierające znaki (25%)

Zaprojektuj i przygotuj zestaw funkcji (bibliotekę) do zarządzania tablicą bloków, w których to blokach pamięci zapisywane są rezultaty operacji

przeglądania (poleceniem find) katalogów przekazywanych jako odpowiedni parametr w poszukiwaniu plików o nazwie podanych jako kolejny parametr funkcji

Biblioteka powinna umożliwiać: 

- utworzenie tablicy wskaźników w której będą przechowywane wskaźniki na bloki pamięci zawierające wyniki przeszukiwań,

- ustawienie aktualnie przeszukiwanego katalogu oraz poszukiwanego pliku,

- przeprowadzenie przeszukania tego katalogu i zapisanie wyniku poszukiwania w pliku tymczasowym 

- zarezerwowanie bloku pamięci o rozmiarze odpowiadającym rozmiarowi pliku tymczasowego i zapisanie w tej pamięci jego zawartości, ustawienie w tablicy wskaźników wskazania na ten blok, funkcja powinna zwrócić indeks stworzonego bloku w tablicy,

- usunięcie z pamięci bloku o zadanym indeksie

Tablice i bloki powinny być alokowane przy pomocy funkcji calloc (alokacja dynamiczna).

Przygotuj plik Makefile, zawierający polecenia kompilujące pliki źródłowe biblioteki oraz tworzące biblioteki w dwóch wersjach: statyczną i współdzieloną.

## Zadanie 2. Program korzystający z biblioteki (25%)

Napisz program testujący działanie funkcji z biblioteki z zadania 1.

Jako argumenty przekaż liczbę elementów tablicy oraz listę zadań do wykonania. Zadania mogą stanowić zadania przeszukania katalogów, opisane przez listę  (katalog, poszukiwany plik) lub zadania usunięcia bloku o podanym indeksie.

Operacje mogą być specyfikowane w linii poleceń na przykład jak poniżej:

* create_table rozmiar - stworzenie tablicy o rozmiarze "rozmiar" 
* search_directory dir file name_file_temp  wartość - wyszukanie pliku o nazwie file w katalogu  dir i zapisanie wyjścia polecenia find w pliku name_file_temp  
* remove_block index - usuń z tablicy bloków o indeksie index


Program powinien stworzyć tablice bloków o zadanej liczbie elementów 

W programie zmierz, wypisz na konsolę i zapisz  do pliku z raportem  czasy realizacji podstawowych operacji:

- przeprowadzenie przeszukania katalogów o różnych poziomach zagłębień i różnych liczbach zawartych plików (omownie - dla zawierającego mało, średnią liczbę i dużo plików i podkatalogów) 

- zapisanie w pamięci bloków o różnych rozmiarach (odpowiadających rozmiarom różnych przeprowadzonych przeszukiwań)

- usunięcie zaalokowanych bloków o różnych rozmiarach  (odpowiadających rozmiarom różnych przeprowadzonych przeszukiwań)

- na przemian  kilkakrotne dodanie i usunięcie zadanej liczby bloków 

Mierząc czasy poszczególnych operacji zapisz trzy wartości: czas rzeczywisty, czas użytkownika i czas systemowy. Rezultaty umieść pliku raport2.txt i dołącz do archiwum zadania.

## Zadanie 3. Testy i pomiary (50%)

a) (25%) Przygotuj plik Makefile, zawierający polecenia kompilujące program z zad 2 na trzy sposoby:
- z wykorzystaniem bibliotek statycznych,
- z wykorzystaniem bibliotek dzielonych (dynamiczne, ładowane przy uruchomieniu programu),
- z wykorzystaniem bibliotek ładowanych dynamicznie (dynamiczne, ładowane przez program),
oraz uruchamiający testy.

Wyniki pomiarów zbierz w pliku results3a.txt. Otrzymane wyniki krótko skomentuj.

b) (25%) Rozszerz plik Makefile z punktu 3a) dodając możliwość skompilowania programu na trzech różnych  poziomach optymalizacji -O0...-Os. Przeprowadź ponownie pomiary kompilując i uruchamiając program dla rożnych poziomów optymalizacji.

Wyniki pomiarów dodaj do pliku results3b.txt. Otrzymane wyniki krotko skomentuj.

Wygenerowane pliki z raportami załącz jako element rozwiązania.

Uwaga:  Do odczytania pliku można użyć funkcji read (man read), do wywołania zewnętrznego polecenia Unixa można użyć funkcji system (man system).
