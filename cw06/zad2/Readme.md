#### Odpalanie
Kompilacja:
```bash
make all
```
odpalenie serwera w jednym terminalu:
 ```bash
 ./server
```

odpalenie różnych klientów w różnych terminalach:
```bash
./client 
```
w kazdym terminalu

wczytywanie z pliku w kliencie: 
```bash
READ scieżka_do_pliku_z_komendami
```

komendy:
```bash
ECHO string

LIST

FRIENDS lista_id_klientów

2ALL string

2FRIENDS string

2ONE id_klienta string

STOP
```

## Zadanie 2. Prosty chat - POSIX (50%)
##### Zrealizuj zadanie analogiczne do Zadania 1, wykorzystując kolejki komunikatów POSIX.
 
Kolejka klienta powinna mieć losową nazwę zgodną z wymaganiami stawianymi przez POSIX. 
 
Na typ komunikatu można zarezerwować pierwszy bajt jego treści.
  
Obsługa zamykania kolejek analogiczna jak w zadaniu 1, z tym, że aby można było usunąć kolejkę,
wszystkie procesy powinny najpierw ją zamknąć. 
   
Przed zakończeniem pracy klient wysyła do serwera komunikat informujący,
że serwer powinien zamknąć po swojej stronie kolejkę klienta.
  
Następnie klient zamyka i usuwa swoją kolejkę.
   
Serwer przed zakończeniem pracy zamyka wszystkie otwarte kolejki, 
informuje klientów, aby usunęli swoje kolejki oraz zamknęli kolejkę serwera i usuwa kolejkę, którą utworzył.


