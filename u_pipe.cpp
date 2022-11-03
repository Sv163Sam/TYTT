#ifndef UNTITLED_U_PIPE
#define UNTITLED_U_PIPE

#include "u_pipe.h"
namespace U_PIPE {
    void guess_player(const int pipe_fd[], const int pipe_fd_s[]) {
        int end_range = 1 + (int) random() % 23;
        int value = 1 + (int) random() % end_range;
        bool flag = false;

        set_array_size(end_range);
        str_display("A number is guessed");
        mes_display("Our range is: ", end_range);
        mes_display("Value is: ", value);
        check(write(pipe_fd[1], &end_range, sizeof(end_range)));

        do {
            int assumption;
            if (check(read(pipe_fd_s[0], &assumption, sizeof(int)))) {
                if (assumption == value)
                    flag = true;
                check(write(pipe_fd[1], &flag, sizeof(flag)));
                answer_all(flag);
            } else
                _exit(EXIT_FAILURE);
        } while (!flag);
        return;
    }

    void assump_player(const int pipe_fd[], const int pipe_fd_s[]) {
        int range;
        int value = 0;
        bool flag = false;
        std::vector<int> attempt = {};

        if (check(read(pipe_fd[0], &range, sizeof(int)))) {
            for (size_t i = 1; i < range; i++) {
                attempt.push_back(i);
            }
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(attempt.begin(), attempt.end(), g);

            do {
                value = attempt[0];
                game_all_write(value);
                check(write(pipe_fd_s[1], &value, sizeof(value)));
                if (check(read(pipe_fd[0], &flag, sizeof(bool)))) {
                    if (!flag) {
                        attempt.erase(attempt.begin());
                        increment_game_count();
                    } else
                        return;
                } else {
                    _exit(EXIT_FAILURE);
                }
            } while (!flag);
            return;
        }
        _exit(EXIT_FAILURE);
    }
}
#endif

/*
void player_1(const int pipe_fd[], const int pipe_fd_s[], bool flag)
{
    if(flag)
    {
        str_display("First player - guesser");
        guess_player(pipe_fd, pipe_fd_s);
        return;
    }
    str_display("First player - assumper");
    assump_player(pipe_fd, pipe_fd_s);
    return;
}

void player_2(const int pipe_fd[], const int pipe_fd_s[], bool flag)
{
    if (flag) {
        str_display("Second player - assumper");
        assump_player(pipe_fd, pipe_fd_s);
        return;
    }
    str_display("Second player - guesser");
    guess_player(pipe_fd, pipe_fd_s);
    return;
}
*/

