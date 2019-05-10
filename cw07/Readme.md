## Zadania - Zestaw 7
### IPC - pamieć wspólna, semafory

##### Przydatne funkcje:

#### System V:
```C
<sys/shm.h> <sys/ipc.h> - shmget, shmclt, shmat, shmdt
```
#### POSIX:
```C
<sys/mman.h> - shm_open, shm_close, shm_unlink, mmap, munmap
```

#### Zadanie
- Przy taśmie transportowej pracują pracownicy, którzy mogą wrzucać na taśmę ładunki o masach odpowiednio od 1 do N jednostek 
(N jest wartością całkowitą). 

- Na końcu taśmy stoi ciężarówka o ładowności X jednostek, którą należy zawsze załadować do pełna tak, że nie można dołożyć następnej paczki z taśmy.
 
- Wszyscy pracownicy starają się układać paczki na taśmie najszybciej jak to możliwe.
 
- Taśma może przetransportować w danej chwili maksymalnie K sztuk paczek. 

- Jednocześnie jednak taśma ma ograniczony udźwig: maksymalnie M jednostek masy, 
tak, że niedopuszczalne jest położenie np. samych najcięższych paczek (N*K>M).
 
- Po zapełnieniu ciężarówki następuje wyładowanie i pojawia się pusta o takich samych parametrach.
  
- Paczki „zjeżdżające” z taśmy muszą od razu trafić na samochód dokładnie w takiej kolejności, w jakiej zostały położone na taśmie.

- Zakładamy, że pracownicy i ciężarówki to osobne procesy loader i trucker.
 
- Kolejni pracownicy są uruchamiani w pętli przez jeden proces macierzysty (za pomocą funkcji fork oraz exec). 

- Ich liczba jest podana w argumencie programu.
 
- Identyfikatorem pracownika jest jego PID. 

- Taśma transportowa o ładowności K sztuk i M jednostek jest umieszczona w pamięci wspólnej.
 
- Pamięć wspólną i semafory tworzy i usuwa program ciężarówki trucker. 

- Należy obsłużyć SIGINT, aby pousuwać pamięć wspólną i utworzone semafory.
 
- W przypadku uruchomienia programu loader przed uruchomieniem truckera,
  powinien zostać wypisany odpowiedni komunikat 
    - (obsłużony błąd spowodowany brakiem dostępu do nieutworzonej pamięci),
   
- W przypadku, gdy trucker kończy pracę, 
    - powinien zablokować semafor taśmy transportowej dla pracowników, 
    
    - załadować to, co pozostało na taśmie, 
        - w przypadku wersji Posix dodatkowo poinformować pracowników, aby ze swojej strony pozamykali mechanizmy synchronizacyjne i pousuwać je.
        

- Program ciężarówki przyjmuje jako argument X (ładowność ciężarówki) oraz K i M (ładowność taśmy) 
i wypisuje cyklicznie na ekranie komunikaty o następujących zdarzeniach:

    - podjechanie pustej ciężarówki,

    - czekanie na załadowanie paczki,

    - ładowanie paczki do ciężarówki - identyfikator pracownika, różnica czasu od próby załadowania na taśmę do załadowania na ciężarówkę, liczba jednostek - stan ciężarówki - ilość zajętych i wolnych miejsc,

    - brak miejsca - odjazd i wyładowanie pełnej ciężarówki.
    

- Program pracownika przyjmuje jako argument wartość od 1 do N (liczba jednostek paczki) 
i wypisuje cyklicznie (zakładamy, że ładunki nie są toksyczne i proces nie umiera po załadowaniu jednej paczki, 
opcjonalny parametr C może zdefiniować liczbę cykli, w przypadku nie podania go, działa w pętli nieskończonej) 
na ekranie komunikaty o następujących zdarzeniach:

    - Załadowanie paczki o N jednostkach z podaniem identyfikatora pracownika i czasu załadowania.
    
    - Czekanie na zwolnienie taśmy.
    

- Każdy komunikat ciężarówki lub pracownika powinien zawierać znacznik czasowy z dokładnością do mikrosekund.
 
- Każdy komunikat pracownika powinien ponadto zawierać informacje o swoim identyfikatorze -PID. 

- Każde zdarzenie związane z ładowaniem na taśmę i zwalnianie jej powinno dodatkowo poinformować o liczbie wolnych/zajętych miejsc oraz jednostek.

- Do implementacji programów należy wykorzystać semafory zliczające, operujące na semaforach wielowartościowych 
(atomowe zmniejszanie i zwiększanie semafora o dowolną wartość) oraz pamięć wspólną.

- Synchronizacja procesów musi wykluczać zakleszczenia i gwarantować sekwencję zdarzeń zgodną ze schematem działania taśmy.
 
- Niedopuszczalne jest na przykład:

    - załadowanie paczki na taśmę, kiedy przekroczona została maksymalna liczba jednostek albo maksymalna liczba paczek,

    - pobranie paczki z taśmy, gdy osiągnięta została maksymalna ładowność ciężarówki,
    
    - załadowanie paczki na taśmę w momencie gdy ciężarówka jest pełna i odjeżdża do wyładowania,
    
    - dopuszczenie do sytuacji, gdy któryś z pracowników nie jest w stanie przez dłuższy czas położyć paczki na taśmie,
    
    - pobranie paczki w innej kolejności niż tej, w której zostały położone na taśmie.

#### Zrealizuj powyższy problem synchronizacyjny , wykorzystując mechanizmy synchronizacji procesów oraz pamięć współdzieloną ze standardu:

##### IPC - System V (50%)
##### IPC - Posix (50%)