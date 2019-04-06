# Zestaw 4. Sygnały
Rodzaje sygnałów: SIGINT, SIGQUIT, SIGKILL, SIGTSTP, SIGSTOP, SIGTERM, SIGSEGV, SIGHUP, SIGALARM, SIGCHLD, SIGUSR1, SIGUSR2

Sygnały czasu rzeczywistego: SIGRTMIN, SIGRTMIN+n, SIGRTMAX

Przydatne polecenia Unix: kill, ps

Przydatne funkcje systemowe: kill, raise, sigqueue, signal, sigaction, sigemptyset, sigfillset, sigaddset, sigdelset, sigismember, sigprocmask, sigpending, pause, sigsuspend



## Zadanie 1 (25%)
Napisz program wypisujący w pętli nieskończonej aktualną datę i godzinę. Po odebraniu sygnału SIGTSTP (CTRL+Z) program zatrzymuje się, wypisując komunikat "Oczekuję na CTRL+Z - kontynuacja albo CTR+C - zakończenie programu". Po ponownym wysłaniu SIGTSTP program powraca do pierwotnego wypisywania.

Program powinien również obsługiwać sygnał SIGINT. Po jego odebraniu program wypisuje komunikat "Odebrano sygnał SIGINT" i kończy działanie. W kodzie programu, do przechwycenia sygnałów użyj zarówno funkcji signal, jak i sigaction (np. SIGINT odbierz za pomocą signal, a SIGTSTP za pomocą sigaction).

Zrealizuj powyższe zadanie, tworząc program potomny, który będzie wywoływał jedną z funkcji z rodziny exec skrypt shellowy zawierający zapętlone systemowe polecenie date. Proces macierzysty będzie przychwytywał powyższe sygnały i przekazywał je do procesu potomnego, tj po otrzymaniu SIGTSTP kończy proces potomka, a jeśli ten został wcześniej zakończony, tworzy nowy, wznawiając działanie skryptu, a po otrzymaniu SIGINT kończy działanie potomka (jeśli ten jeszcze pracuje) oraz programu.




## Zadanie 2 (35%)
Zmodyfikuj program 2 (monitor - wersję z alokowaniem pamięci) z poprzedniego zestawu tak, aby to program macierzysty był odpowiedzialny za zakończenie procesów potomnych. Nie ma ograniczenia czasowego dla działania programu. Po uruchomieniu programu proces macierzysty wypisuje listę plików przydzielonych do określonych procesów potomnych. Po utworzeniu procesów potomnych, które działają w nieskończonych pętlach proces macierzysty nie przechodził w stan uśpienia, czekając na zakończenie procesów, tylko obiera komendy od użytkownika:

* LIST - program wypisuje listę procesów monitorujących pliki
* STOP PID - program zatrzymuje (nie kończy) monitorowanie procesu o numerze PID. Można to zrealizować poprzez wysłanie do procesu potomnego sygnału SIGUSR1. Proces macierzysty po odebraniu sygnału zatrzymuje pętlę poprzez zmianę flagi.
* STOP ALL - program zatrzymuje (nie kończy) monitorowanie wszystkich procesów potomnych
* START PID - program wznawia monitorowanie procesu o numerze PID (również poprzez wysłanie sygnału i zmianę flagi).
* START ALL - program wznawia działanie wszystkich procesów potomnych
* END - program kończy działanie wszystkich procesów i wyświetla końcowy raport.

**Zdefiniuj dodatkowo obsługę sygnału SIGINT, który będzie działał jak END, czyli wypisze raport końcowy.**

## Zadanie 3 (40%)
Napisz dwa programy: sender program wysyłający sygnały SIGUSR1 i  catcher - program zliczający ilość odebranych sygnałów. Ilość sygnałów SIGUSR1 wysyłanych przez pierwszy program powinna być określana w parametrze wywołania tego programu. Program catcher jest uruchamiany najpierw, wypisuje swój numer PID i czeka na sygnały SIGUSR1 i SIGUSR2. Wszystkie pozostałe sygnały są blokowane. Program sender przyjmuje trzy parametry: PID procesu catcher, ilość sygnałów do wysłania i tryb wysłania sygnałów.

Po transmisji wszystkich sygnałów SIGUSR1 sender powinien wysłać sygnał SIGUSR2, po otrzymaniu którego catcher wysyła do sendera tyle sygnałów SIGUSR1, ile sam ich otrzymał a „transmisję” kończy wysłaniem sygnału SIGUSR2, wypisaniem liczby odebranych sygnałów i zakończeniem działania. PID sendera catcher pobiera ze struktury SIGACTION po przechwyceniu od niego sygnału. Program sender po otrzymaniu sygnału SIGUSR2 wyświetla komunikat o ilości otrzymanych sygnałów SIGUSR1 oraz o tym, ile powinien ich otrzymać i kończy działanie.

UWAGA! W żaden sposób nie opóźniamy wysyłania sygnałów, wszelkie "gubienie" sygnałów jest zjawiskiem naturalnym.

**Wysyłanie sygnałów w obu programach należy wykonać w następujących trybach:**
* KILL - za pomocą funkcji kill
* SIGQUEUE - za pomocą funkcji sigqueue - wraz z przesłanym sygnałem catcher wysyła numer kolejnego odsyłanego sygnału, dzięki czemu sender wie, ile dokładnie catcher odebrał, a tym samym wysłał do niego sygnałów. Wypisz tę dodatkową informację w senderze.
* SIGRT - zastępując SIGUSR1 i SIGUSR2 dwoma dowolnymi sygnałami czasu rzeczywistego wysyłanymi za pomocą kill. Jaka liczba sygnałów będzie teraz odebrana?

Zmodyfikuj powyższe programy, dodając potwierdzenie odbioru sygnału po każdorazowym ich odebraniu przez program catcher. W tym celu, catcher wysyła do sendera sygnał SIGUSR1 informujący o odbiorze sygnału. Sender powinien wysłać kolejny sygnał dopiero po uzyskaniu tego potwierdzenia. Zapewnij rozwiązanie, w którym ilość sygnałów odebranych jest zgodna z ilością sygnałów wysłanych, i w którym nie dochodzi do zakleszczenia. (15%)
