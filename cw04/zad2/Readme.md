## Zadanie 2 (35%)
Zmodyfikuj program 2 (monitor - wersję z alokowaniem pamięci) z poprzedniego zestawu tak, aby to program macierzysty był odpowiedzialny za zakończenie procesów potomnych. Nie ma ograniczenia czasowego dla działania programu. Po uruchomieniu programu proces macierzysty wypisuje listę plików przydzielonych do określonych procesów potomnych. Po utworzeniu procesów potomnych, które działają w nieskończonych pętlach proces macierzysty nie przechodził w stan uśpienia, czekając na zakończenie procesów, tylko obiera komendy od użytkownika:

* LIST - program wypisuje listę procesów monitorujących pliki
* STOP PID - program zatrzymuje (nie kończy) monitorowanie procesu o numerze PID. Można to zrealizować poprzez wysłanie do procesu potomnego sygnału SIGUSR1. Proces macierzysty po odebraniu sygnału zatrzymuje pętlę poprzez zmianę flagi.
* STOP ALL - program zatrzymuje (nie kończy) monitorowanie wszystkich procesów potomnych
* START PID - program wznawia monitorowanie procesu o numerze PID (również poprzez wysłanie sygnału i zmianę flagi).
* START ALL - program wznawia działanie wszystkich procesów potomnych
* END - program kończy działanie wszystkich procesów i wyświetla końcowy raport.

**Zdefiniuj dodatkowo obsługę sygnału SIGINT, który będzie działał jak END, czyli wypisze raport końcowy.**