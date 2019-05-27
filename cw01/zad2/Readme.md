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
