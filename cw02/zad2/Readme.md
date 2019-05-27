## Zadanie 2. Operacje na strukturze katalogów (45%)
Napisz program wyszukujący w drzewie katalogu (ścieżka do katalogu jest pierwszym argumentem programu), 
w zależności od wartości drugiego argumentu ('<', '>','=') , pliki zwykłe z datą modyfikacji wcześniejszą, późniejszą lub 
równą dacie podanej jako trzeci argument programu. Program ma wypisać na standardowe wyjście następujące informacje znalezionych plików:

* ścieżka bezwzględna pliku,
* rodzaj pliku (zwykły plik - file, katalog - dir, urządzenie znakowe - char dev, urządzenie blokowe - block dev, potok nazwany - fifo, link symboliczny - slink, soket - sock) 
* rozmiar w bajtach,
* datę ostatniego dostępu,
* datę ostatniej modyfikacji.

Ścieżka podana jako argument wywołania może być względna lub bezwzględna.  Program nie powinien podążać za dowiązaniami symbolicznymi do katalogów.

Program należy zaimplementować w dwóch wariantach:
* Korzystając z funkcji opendir, readdir oraz funkcji z rodziny stat (25%)
* Korzystając z funkcji nftw (20%)
* W ramach testowania funkcji utwórz w badanej strukturze katalogów jakieś dowiązania symboliczne, zwykłe pliki i katalogi.
