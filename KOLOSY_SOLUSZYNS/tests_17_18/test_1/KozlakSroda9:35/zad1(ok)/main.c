#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/times.h>

static void display_times(clock_t, struct tms*, struct tms*);

static void do_cmd(char*, int instruction, int final);

int main(int argc, char* argv[]) {
    int i;

    setbuf(stdout, NULL);
    for (i = 1; i < argc; i++)
        do_cmd(argv[i], i, argc - 1);
    exit(0);
}

static void do_cmd(char* cmd,
                   int instruction,
                   int final) /* wykonaj program, policz jego czasy, sprawdz,
                                 czy to nie jest ostatni program */
{
    struct tms tmsstart, tmsend;
    clock_t start, end;
    int status;

    printf("\ncommand: %s\n", cmd);

    /* CHANGES FROM HERE */

    /*DO ZROBIENIA: zapamietaj czas w chwili aktualnej w zmiennych start i
     * tmsstart*/

    if ((start = (times(&tmsstart))) ==
        -1) /* zapamietaj wartosci poczatkowe pomiaru czasu*/
    {
        printf("times error");
        exit(-1);
    }

    if ((status = system(cmd)) < 0) /* wykonaj polecenie */
    {
        printf("system() error");
        exit(-1);
    }

    /*DO ZROBIENIA: zapamietaj czas w chwili aktualnej w zmiennych end i
     * tmsend*/

    if ((end = (times(&tmsend))) == -1) /* zapamietaj wartosci koncowe */
    {
        printf("times error");
        exit(-1);
    }

    /* TILL HERE */

    display_times(end - start, &tmsstart, &tmsend);

    if (instruction == final)
        exit(status);
}

static void display_times(clock_t real,
                          struct tms* tmsstart,
                          struct tms* tmsend) {
    static long clktck = 0;

    if (clktck == 0) /* sprawdz, czy stan zmiennej odczytywany pierwszy raz */
        if ((clktck = sysconf(_SC_CLK_TCK)) < 0) {
            printf("sysconf error");
            exit(-1);
        }

    printf("  real:  %7.2f\n", real / (double)clktck);
    printf("  user:  %7.2f\n",
           (tmsend->tms_utime - tmsstart->tms_utime) / (double)clktck);
    printf("  sys:   %7.2f\n",
           (tmsend->tms_stime - tmsstart->tms_stime) / (double)clktck);
    printf("  child user:  %7.2f\n",
           (tmsend->tms_cutime - tmsstart->tms_cutime) / (double)clktck);
    printf("  child sys:   %7.2f\n",
           (tmsend->tms_cstime - tmsstart->tms_cstime) / (double)clktck);
}