## Zadania - Zestaw 10

### Celem zadania jest napisanie prostego systemu pozwalającego na wykonywanie zadań na "klastrze obliczeniowym".

- System oparty jest o centralny serwer oraz połączonych do niego klientów.

- Serwer nasłuchuje jednocześnie na gnieździe sieciowym i gnieździe lokalnym. 
    - Z jego poziomu zlecane są zadania.

- Klienci po uruchomieniu przesyłają do serwera swoją nazwę, a ten sprawdza czy klient o takiej nazwie już istnieje
    - jeśli tak, to odsyła klientowi komunikat że nazwa jest już w użyciu; 
    - jeśli nie, to zapamiętuje klienta.


- Obsługiwane obliczenia polegają na zliczeniu ilości wszystkich słów, oraz ilości poszczególnych słów w zadanym tekście

- Zlecenia obliczeń są wpisane bezpośrednio w konsoli serwera poprzez podanie ściezki do pliku z tekstem do przetworzenia.

- Serwer tworzy strukturę opisującą zlecenie (text), a następnie przesyła ją do klienta, który nie wykonuje w danym momencie obliczeń. 

- Jeżeli wszyscy klienci wykonują w danym momencie obliczenia, serwer wysyła zlecenie do dowolnego klienta.

- Klient po odebraniu zlecenia oblicza wynik i odsyła go do serwera, który wyświetla wynik na standardowym wyjściu.

- Klient przy wyłączeniu Ctrl+C powinien wyrejestrować się z serwera.


- W procesie serwera obsługa wprowadzania zleceń z terminala i obsługa sieci powinny być zrealizowane w dwóch osobnych wątkach.

- Wątek obsługujący sieć powinien obsługiwać gniazdo sieciowe i gniazdo lokalne jednocześnie,
 wykorzystując w tym celu funkcje do monitorowania wielu deskryptorów (epoll/poll/select).
 
- Dodatkowo, osobny wątek powinien cyklicznie "pingować" zarejestrowanych klientów,
 aby zweryfikować że wciąż odpowiadają na żądania i jeśli nie - usuwać ich z listy klientów.
 
- Można przyjąć, że ilość klientów zarejestrowanych na serwerze jest ograniczona do maksymalnie kilkunastu.

- Należy zapewnić możliwość ustalenia dla którego zlecenia wyświetlony został wynik 
    - (np. umieścić w serwerze globalny licznik, wyświetlany i inkrementowany po dodaniu każdego nowego zlecenia i umieszczany jako dodatkowy identyfikator w komunikatach wysyłanych do klientów i odpowiedziach od nich przychodzących) i który klient go obliczył.

#### Serwer przyjmuje jako swoje argumenty:

- numer portu TCP/UDP (zależnie od zadania)
- ścieżkę gniazda UNIX

#### Klient przyjmuje jako swoje argumenty:

- swoją nazwę (string o z góry ograniczonej długości)
- sposób połączenia z serwerem (sieć lub komunikacja lokalna przez gniazda UNIX)
- adres serwera (adres IPv4 i numer portu lub ścieżkę do gniazda UNIX serwera)

### Zadanie 1
Komunikacja klientów i serwera odbywa się z użyciem protokołu strumieniowego.

### Zadanie 2
Komunikacja klientów i serwera odbywa się z użyciem protokołu datagramowego.

#### Wskazówka: 
Na łączu strumieniowym można zasymulować komunikację datagramową korzystając z TLV (Type-Length-Value)
- najpierw wysyłana jest stałej wielkości (np. 1 bajt) wartość mówiącą o typie komunikatu/danych, 
 
- później stałej wielkości (np. 2 bajty) wartość mówiącą o długości przesyłanych danych, 
 
- aż wreszcie przesyłane są faktyczne dane w ilości określonej chwilę wcześniej; można też np. przechowywać 
 typ danych jako pierwszy bajt przesyłanej wartości - tak jak robią to struktury opisujące adresy gniazd. 
 
Użycie tych technik nie jest wymagane w zadaniu.