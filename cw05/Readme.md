# Zestaw 5 - Potoki nazwane i nienazwane
## Zadanie 1
Napisz interpreter poleceń przechowywanych w pliku (sciezka do pliku to pierwszy argument wywolania programu).

Polecenia w pliku przechowywane są w kolejnych liniach w postaci: 

prog1 arg1 ... argn1 | prog2 arg1 ... argn2 | ... | progN arg1 ... argnN

Dla każdej  takiej linii interpreter powinien uruchomić wszystkie N poleceń w osobnych procesach, zapewniając przy użyciu potoków nienazwanych oraz funkcji dup2, by wyjście standardowe procesu k było przekierowane do wejścia standardowego procesu (k+1). Można założyć ograniczenie górne na ilość obsługiwanych argumentów oraz ilość połączonych komend w pojedynczym poleceniu (co najmniej 5). Po uruchomieniu ciągu programów składających się na pojedczyne polecenie (linijkę) interpreter powinien oczekiwać na zakończenie wszystkich tych programów.

__Uwaga: należy użyć pipe/fork/exec, nie popen__

# Zadanie 2
Należy napisać dwa programy - master oraz slave - które będą komunikować się poprzez potok nazwany (kolejkę FIFO), do której ścieżkę będą dostawać jako argument wywołania. Do potoku pisać będzie wiele procesów wykonujących program slave, a czytał będzie z niej jeden proces master.

Master przyjmuje jeden argument - ścieżkę do potoku nazwanego. Tworzy on ten potok nazwany, a następnie czyta kolejne linijki z potoku nazwanego i wypisuje je na ekran.

Slave przyjmuje dwa argumenty - ścieżkę do potoku nazwanego i liczbę całkowitą N. Wykonuje następujące akcje:

* otwiera potok nazwany
* wypisuje swój PID na wyjście standardowe
* N razy zapisuje do potoku nazwanego linijkę składającą się ze swojego PIDu oraz obecnej daty
  * datę należy wygenerować programem date uruchomionym przy użyciu funkcji popen
  * po każdym zapisie należy odczekać losową ilość czasu (np. 2-5 sekund)
