#include "u_pipe.h"
#include "signals.h"
#include "mes_queue.h"

/*
int main()
{
    auto d = check(mq_open("/mymq", O_CREAT, S_IRUSR | S_IWUSR, NULL));
    check(mq_close(d));

    pid_t p = check(fork());
    if (p) {
        mqd_t mqd = check(mq_open("/mymq",O_WRONLY));
        M_QUEUE::guess_player(mqd);
        int stat;
        wait(&stat);
        mq_unlink("/mymq");
    }
    else {
        mqd_t mqd = check(mq_open("/mymq", O_RDONLY));
        M_QUEUE::assump_player(mqd);
    }
}
*/


int main()
{
    int game_mode_var = 0;
    str_display("Please, select a game execution: ");
    str_display("1 - By processes with synchronization type - unnamed pipe");
    str_display("2 - By processes with synchronization type - message queue");
    str_display("3 - By signals with synchronization type - real time signals");


    std::cin >> game_mode_var;
    switch(game_mode_var)
    {
        case 1:
        {
            int count = 0;
            bool game_flag = true;

            int fd[2];
            int fd_s[2];
            check(pipe(fd));
            check(pipe(fd_s));
            pid_t p_id = check(fork());

            str_display("GAME STARTING");

            do {
                unsigned int start_time = clock();
                if(count % 2 != 0) game_flag = false;
                if (p_id) {
                    if(game_flag) U_PIPE::guess_player(fd, fd_s);
                    else
                    {
                        sleep(1);
                        U_PIPE::assump_player(fd, fd_s);
                    }
                }
                else
                {
                    if(game_flag) U_PIPE::assump_player(fd, fd_s);
                    else
                    {
                        sleep(1);
                        U_PIPE::guess_player(fd, fd_s);
                    }
                }
                count++;
                if (!game_flag) game_flag = true;
                get_game_space(game_mode_var);
                unsigned int end_time = clock();
                set_game_time(end_time - start_time);
            }while(count < 11);
            exit(EXIT_SUCCESS);
            break;
        }
        case 2:
        {
            auto d = check(mq_open("/mymq", O_CREAT, S_IRUSR | S_IWUSR, NULL));
            check(mq_close(d));

            pid_t p = check(fork());
            if (p) {
                mqd_t mqd = check(mq_open("/mymq",O_WRONLY));
                M_QUEUE::guess_player(mqd);
                int stat;
                wait(&stat);
                mq_unlink("/mymq");
            }
            else {
                mqd_t mqd = check(mq_open("/mymq", O_RDONLY));
                M_QUEUE::assump_player(mqd);
            }
            break;
        }
        case 3:
        {
            str_display("GAME STARTING");
            pid_t p_id = check(fork());

            for (int i = 0; i < 10; i++)
            {
                unsigned int start_time = clock();
                if (p_id) {
                    if (i % 2 == 0)
                    {
                        SIGNALS::guess_player(p_id);
                    }
                    else
                    {
                        sleep(2);
                        SIGNALS::assump_player(p_id);
                    }
                }
                else
                {
                    if (i % 2 == 0)
                    {
                        SIGNALS::assump_player(getppid());
                    }
                    else
                    {
                        sleep(2);
                        SIGNALS::guess_player(getppid());
                    }
                }
                get_game_space(game_mode_var);
                unsigned int end_time = clock();
                set_game_time(end_time - start_time);
            }
            std::cout << "GAME END";
            exit(EXIT_SUCCESS);
            break;
        }
        default:
        {
            exit(EXIT_FAILURE);
        }
    }

}


/*
int main()
{

}
// time, attempts
*/
