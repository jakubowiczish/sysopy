# Zadanie 2
Należy napisać dwa programy - master oraz slave - które będą komunikować się poprzez potok nazwany (kolejkę FIFO), do której ścieżkę będą dostawać jako argument wywołania. Do potoku pisać będzie wiele procesów wykonujących program slave, a czytał będzie z niej jeden proces master.

Master przyjmuje jeden argument - ścieżkę do potoku nazwanego. Tworzy on ten potok nazwany, a następnie czyta kolejne linijki z potoku nazwanego i wypisuje je na ekran.

Slave przyjmuje dwa argumenty - ścieżkę do potoku nazwanego i liczbę całkowitą N. Wykonuje następujące akcje:

* otwiera potok nazwany
* wypisuje swój PID na wyjście standardowe
* N razy zapisuje do potoku nazwanego linijkę składającą się ze swojego PIDu oraz obecnej daty
  * datę należy wygenerować programem date uruchomionym przy użyciu funkcji popen
  * po każdym zapisie należy odczekać losową ilość czasu (np. 2-5 sekund)
