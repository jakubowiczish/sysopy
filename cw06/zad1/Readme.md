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

## Zadanie 1. Prosty chat - System V (50%)

Napisz prosty chat typu klient-serwer, w którym komunikacja zrealizowana jest za pomocą kolejek komunikatów - 
jedna, na zlecenia klientów dla serwera, druga, prywatna, na odpowiedzi.

Serwer po uruchomieniu tworzy nową kolejkę komunikatów systemu V. 

Za pomocą tej kolejki klienci będą wysyłać komunikaty do serwera. 

Wysyłane zlecenia mają zawierać rodzaj zlecenia jako rodzaj komunikatu oraz informację od którego klienta zostały wysłane (ID klienta),
w odpowiedzi rodzajem komunikatu ma być informacja identyfikująca czekającego na nią klienta.

Klient bezpośrednio po uruchomieniu tworzy kolejkę z unikalnym kluczem IPC  
i wysyła jej klucz komunikatem do serwera (komunikat INIT).
 
Po otrzymaniu takiego komunikatu,
serwer otwiera kolejkę klienta, przydziela klientowi identyfikator (np. numer w kolejności zgłoszeń)
i odsyła ten identyfikator do klienta (komunikacja w kierunku serwer->klient odbywa się za pomocą kolejki klienta).

Po otrzymaniu identyfikatora, klient rozpoczyna wysyłanie zleceń do serwera (w pętli), 
zlecenia są czytane ze standardowego wyjścia w postaci typ_komunikatu albo z pliku tekstowego
w którym w każdej linii znajduje się jeden komunikat (napisanie po stronie klienta READ plik zamiast typu komunikatu).

Przygotuj pliki z dużą liczbą zleceń, aby można było przetestować działanie zleceń i priorytetów.


### Rodzaje zleceń

```bash
ECHO string:
```

- Klient wysyła ciąg znaków. Serwer odsyła ten sam ciąg z powrotem, dodatkowo podając datę jego otrzymania. 

- Klient po odebraniu wysyła go na standardowe wyjście.

```bash
LIST:
```
- Zlecenie wypisania listy wszystkich aktywnych klientów

```bash
FRIENDS lista_id_klientów
```

- Klient wysyła do serwera listę klientów, z którymi chce się grupowo komunikować. 

- Serwer przechowuje tylko ostatnią listę. Kolejne wysłanie komunikatu FRIENDS nadpisuje poprzednią listę.

- Wysłanie samego FRIENDS czyści listę. 
 
 
##### Grupę można modyfikować, wysyłając do serwera komunikaty:

```bash
ADD lista_id_klientów oraz DEL lista_id_klientów.
```

- Wysłanie ADD lista_id_klientów po uprzednim wyczyszczeniu listy jest analogiczne z wysłaniem FRIENDS lista_id_klientów. 

- Próba wysłania ADD i DEL bez argumentów powinna zostać obsłużona po stronie klienta.

```bash
2ALL string: 
```

- Zlecenie wysłania komunikatu do wszystkich pozostałych klientów.
 
- Klient wysyła ciąg znaków. Serwer wysyła ten ciąg wraz z identyfikatorem klienta-nadawcy oraz aktualną datą do wszystkich pozostałych klientów.

```bash
2FRIENDS string: 
```

- Zlecenie wysłania komunikatu do zdefiniowanej wcześniej grupy klientów.

- Klient wysyła ciąg znaków.

- Serwer wysyła ten ciąg wraz z identyfikatorem klienta-nadawcy oraz aktualną datą do zdefiniowanej wcześniej grupy klientów.

```bash
2ONE id_klienta string:
```

- Zlecenie wysłania komunikatu do konkretnego klienta.
 
- Klient wysyła ciąg znaków podając jako adresata konkretnego klienta o identyfikatorze z listy aktywnych klientów.
 
- Serwer wysyła ten ciąg wraz z identyfikatorem klienta-nadawcy oraz aktualną datą do wskazanego klienta.


```bash
STOP:
```

- Zgłoszenie zakończenia pracy klienta. 

- Klient wysyła ten komunikat, kiedy kończy pracę, aby serwer mógł usunąć z listy jego kolejkę. 

- Następnie kończy pracę, usuwając swoją kolejkę.
 
- Komunikat ten wysyłany jest również, gdy po stronie klienta zostanie wysłany sygnał SIGINT.
 
##
- Zlecenia powinny być obsługiwane zgodnie z priorytetami, najwyższy priorytet ma STOP, potem LIST oraz FRIENDS i reszta. 

Można tego dokonać poprzez sterowanie parametrem MTYPE w funkcji msgsnd.

Poszczególne rodzaje komunikatów należy identyfikować za pomocą typów komunikatów systemu V.

Klucze dla kolejek mają być generowane na podstawie ścieżki $HOME.
 
Małe liczby do wygenerowania kluczy oraz rodzaje komunikatów mają być zdefiniowane we wspólnym pliku nagłówkowym.
 
Dla uproszczenia można założyć, że długość komunikatu (lub maksymalna długość łańcucha znaków przy usłudze echa)
 jest ograniczona pewną stałą (jej definicja powinna znaleźć się w pliku nagłówkowym).
 
- Klient i serwer należy napisać w postaci osobnych programów (nie korzystamy z funkcji fork).
 
- Serwer musi być w stanie pracować z wieloma klientami naraz.
 
- Przed zakończeniem pracy każdy proces powinien usunąć kolejkę którą utworzył (patrz IPC_RMID oraz funkcja atexit).
 
Dla uproszczenia można przyjąć, że serwer przechowuje informacje o klientach w statycznej tablicy
 (rozmiar tablicy ogranicza liczbę klientów, którzy mogą się zgłosić do serwera).
 
## 
##### Serwer może wysłać do klientów komunikaty:

- inicjujący pracę klienta (kolejka główna serwera)
- wysyłający odpowiedzi do klientów (kolejki klientów)
- informujący klientów o zakończeniu pracy serwera - po wysłaniu takiego sygnału i odebraniu wiadomości STOP 
od wszystkich klientów serwer usuwa swoją kolejkę i kończy pracę.(kolejki klientów)

- Należy obsłużyć przerwanie działania serwera lub klienta za pomocą CTRL+C.

- Po stronie klienta obsługa tego sygnału jest równoważna z wysłaniem komunikatu STOP.



