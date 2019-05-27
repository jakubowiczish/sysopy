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