#ifndef SIGNAL
#define SIGNAL

#include "signals.h"
namespace SIGNALS {
    volatile sig_atomic_t last_signal_id;
    volatile sig_atomic_t signal_value;
    void sig_handler(int signal_id) {
        last_signal_id = signal_id;
    }

    void rt_sig_handler(int signal_id, siginfo_t *signal_info, void *ctx) {
        last_signal_id = signal_id;
        signal_value = signal_info->si_value.sival_int;
    }

    bool exists(pid_t p, bool is_parent) {
        if (!is_parent)
            return (getpgid(p) >= 0);
        int stat;
        return waitpid(p, &stat, WNOHANG) == 0;
    }


    void guess_player(pid_t p_id) {
        struct sigaction
                real_time_action{},
                guesser_action{},
                assumper_action{},
                quit_action{};

        sigset_t set;
        sigemptyset(&set);

        real_time_action.sa_sigaction = rt_sig_handler;
        real_time_action.sa_flags = SA_SIGINFO;
        check(sigaction(SIGALRM, &real_time_action, nullptr));

        guesser_action.sa_handler = sig_handler;
        check(sigaction(SIGUSR1, &guesser_action, nullptr));

        assumper_action.sa_handler = sig_handler;
        check(sigaction(SIGUSR2, &assumper_action, nullptr));

        quit_action.sa_handler = sig_handler;
        check(sigaction(SIGCHLD, &quit_action, nullptr));

        raise(SIGSTOP);

        int end_range = 12 + (int) random() % 22;
        int value = 1 + (int) random() % end_range;
        str_display("A number is guessed");
        mes_display("Our range is: ", end_range);
        mes_display("Value is: ", value);
        std::cout << " 343333333" << std::endl;
        check(sigqueue(p_id, SIGALRM, sigval{end_range}));

        bool flag = false;
        while (!flag) {
            sigsuspend(&set);

            if (signal_value == value)
                flag = true;
            answer_display(flag);
            check(kill(p_id, (flag ? SIGUSR1 : SIGUSR2)));
        }
        return;
    }

    void assump_player(pid_t pid) {
        struct sigaction
                real_time_action{},
                guesser_action{},
                assumper_action{},
                quit_action{};

        sigset_t set;
        sigemptyset(&set);

        real_time_action.sa_sigaction = rt_sig_handler;
        real_time_action.sa_flags = SA_SIGINFO;
        check(sigaction(SIGALRM, &real_time_action, nullptr));

        guesser_action.sa_handler = sig_handler;
        check(sigaction(SIGUSR1, &guesser_action, nullptr));

        assumper_action.sa_handler = sig_handler;
        check(sigaction(SIGUSR2, &assumper_action, nullptr));

        quit_action.sa_handler = sig_handler;
        check(sigaction(SIGCHLD, &quit_action, nullptr));

        kill(pid, SIGCONT);
        sigsuspend(&set);
        int range = signal_value;
        bool flag = false;

        std::vector<int> attempt = {};
        for (int i = 1; i < range + 1; i++) {
            attempt.push_back(i);
        }
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(attempt.begin(), attempt.end(), g);

        while (!flag) {
            if (!exists(pid, false))
                exit(EXIT_FAILURE);

            int value = attempt[0];
            assumption_display(value);
            check(sigqueue(pid, SIGALRM, sigval{value}));

            sigsuspend(&set);

            if (last_signal_id == SIGUSR1)
                flag = true;
            attempt.erase(attempt.begin());
            //increment_game_count();
        }
        return;
    }
}
#endif