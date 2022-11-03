#ifndef UNTITLED_MES_QUEUE
#define UNTITLED_MES_QUEUE

#include "mes_queue.h"
namespace M_QUEUE {
    void guess_player(mqd_t mqd) {
        int end_range = 1 + (int) random() % 20;
        int value = 1 + (int) random() % end_range;
        bool flag = false;

        str_display("A number is guessed");
        mes_display("Our range is: ", end_range);
        mes_display("Value is: ", value);
        check(mq_send(mqd, (char *) &value, sizeof(value), 0));

        do {
            int *assumption = new int[8 * 1024 / sizeof(int)];
            if (check(mq_receive(mqd, (char *) assumption, 8 * 1024, nullptr))) {
                if (*assumption == value)
                    flag = true;
                check(mq_send(mqd, (char *) &flag, sizeof(flag), 0));
                answer_display(flag);
            } else
                _exit(EXIT_FAILURE);
        } while (!flag);
        return;
    }

    void assump_player(mqd_t mqd) {
        int *range = new int[8 * 1024 / sizeof(int)];
        int value = 0;
        bool *flag = new bool[sizeof(bool)];
        std::vector<int> attempt = {};

        if (check(mq_receive(mqd, (char *) range, 8 * 1024, nullptr))) {
            for (int i = 1; i < *range; i++) {
                attempt.push_back(i);
            }
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(attempt.begin(), attempt.end(), g);

            do {
                value = attempt[0];
                assumption_display(value);
                check(mq_send(mqd, (char *) &value, 1, 0));
                if (check(mq_receive(mqd, (char *) flag, sizeof(bool), nullptr))) {
                    if (!flag[0]) {
                        attempt.erase(attempt.begin());
                        increment_game_count();
                    } else
                        return;
                } else {
                    _exit(EXIT_FAILURE);
                }
            } while (!flag[0]);
            return;
        }
        _exit(EXIT_FAILURE);
    }
}
#endif
