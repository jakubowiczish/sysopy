## Zadania - Zestaw 9

#### Opis problemu

##### W ramach zadana należy zaimplementować rozwiązanie problemu synchronizacyjnego wagoników roller coaster.

- Po torze w wesołym miasteczku porusza się kilka wagoników roller coaster.
 
- Wagoniki nie mogą się wyprzedzać (poruszają się po jednym torze). 

- Po podjechaniu do platformy wagonik otwiera drzwi. 

- Następnie z wagonika wysiadają pasażerowie (jeśli wagonik nie był pusty).
 
- Gdy wszyscy pasażerowie wysiądą, do wagonika wsiada kolejna grupa pasażerów. 

- Gdy do wagonika wsiądzie dokładnie c pasażerów jeden z nich (wybrany losowo) wciska przycisk start. 
 
- Wówczas wagonik zamyka drzwi i odjeżdża. Do platformy może następnie podjechać kolejny wagonik.

### Zadanie

Zaimplementuj poprawne rozwiązanie problemu synchronizacji wagoników roller coaster,
w którym każdy pasażer i każdy wagonik to osobny wątek. 
 
##### Wagonik wypisuje komunikaty o następujących zdarzeniach:

- Zamknięcie drzwi.

- Rozpoczęcie jazdy. 
    - Zakładamy, że jazda trwa pewien losowy okres czasu. Na potrzeby zadania można przyjąć czas w przedziale od 0 do kilku milisekund.

- Zakończenie jazdy.

- Otwarcie drzwi.

- Zakończenie pracy wątku.


##### Pasażer wypisuje komunikaty o następujących zdarzeniach:

- Wejście do wagonika. Komunikat ten zawiera aktualną liczbę pasażerów w wagoniku.

- Opuszczenie wagonika. Komunikat ten zawiera aktualną liczbę pasażerów w wagoniku.

- Naciśnięcie przycisku start.

- Zakończenie pracy wątku.

- Każdy wypisywany komunikat musi zawierać znacznik czasowy z dokładnością do milisekund oraz identyfikator, odpowiednio, wagonika lub pasażera. 
Sekwencja zdarzeń musi gwarantować poprawy przewóz pasażerów. 

##### Niedopuszczalne jest na przykład:

- wejście pasażera do wagonika gdy w wagoniku znajduje się pasażer z poprzedniej jazdy,

- wejście do wagonika więcej niż c pasażerów,

- naciśnięcie przycisku start gdy w wagoniku jest mniej niż c pasażerów,

- zamknięcie drzwi przed naciśnięciem przycisku start,

- podjechanie wagonika do platformy zanim poprzedni wagonik z niej odjedzie,

- zmiana kolejności wagoników na torze (podjeżdżają do platformy w kolejności innej, niż z niej uprzednio odjechały), etc.

- Niedopuszczalne jest również rozwiązanie, w którym niektóre wątki są głodzone (niektórzy pasażerowie 
w ogóle nie wsiadają do wagoników, podczas gdy inni jeżdżą wielokrotnie).

#

##### Program należy zaimplementować korzystając z wątków i mechanizmów synchronizacji biblioteki POSIX Threads. 

#### Argumentami wywołania programu są:
- liczba wątków pasażerów, 

- liczba wątków wagoników,

- pojemność wagonika (c) 

- liczba przejazdów (n).
 
## 
- Po uruchomieniu programu wątek główny tworzy wątki dla pasażerów i dla wagoników.
 
Następnie każdy wagonik wykonuje n przejazdów (wagoniki jadą po torze równocześnie, zachowując swoją początkową kolejność) po czym kończy pracę.
 
Wątki pasażerów działają tak długo, jak długo pracuje choć jeden wagonik. 

W tym czasie wątek główny oczekuje na zakończenie wszystkich stworzonych wątków.
 
Niedopuszczalne jest tworzenie dodatkowych wątków, których celem byłoby zapewnienie synchronizacji (lub wykorzystanie w tym celu wątku głównego).