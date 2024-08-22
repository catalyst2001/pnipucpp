#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <math.h>
#include <queue>
#include <random>

#define ARRSIZE(x) (sizeof(x) / sizeof(x[0]))

/* */
#define MAX_CHANNELS 3
#define MAX_QUEUE_SIZE 4

std::queue<int> queue;

const size_t num_exps = 50; //total expirements
double statistic_avg = 10.0; //avg orders in day
double total_exp_time = 50.0; //expirenent total 50 hours

/**
* service channel
*/
class service_channel
{
  int    tank;
  double service_time;
  double start_service_time;
public:
  service_channel(double _service_time) : tank(0), service_time(_service_time), start_service_time(0.0) {}
  ~service_channel() {}

  void set_busy(double current_time, int tank_number) {
    tank = tank_number;
    start_service_time = current_time;
  }

  bool is_free(double current_time) {
    return current_time - start_service_time > service_time;
  }

  void reset() {
    tank = 0;
    start_service_time = 0.0;
  }
};

const double tank_volume = 10.0; //volume all tanks is 10 m^3

service_channel service_channels[] = {
  service_channel(tank_volume / 1.0), //1 m^3 in hour
  service_channel(tank_volume / 2.0), //2 m^3 in hour
  service_channel(tank_volume / 2.0) //2 m^3 in hour
};

bool get_any_free_service_channel(size_t &dst, double current_time)
{
  for (size_t i = 0; i < ARRSIZE(service_channels); i++) {
    service_channel& channel = service_channels[i];
    if (channel.is_free(current_time)) {
      dst = i;
      return true;
    }
  }
  return false;
}

bool service_channels_check_queue(double current_time)
{
  size_t free_channel_idx;
  if (get_any_free_service_channel(free_channel_idx, current_time)) {
    if (!queue.empty()) {
      int tank_num = queue.front(); queue.pop();
      printf("Бак %.4d перешел в обслуживание на линию %zd из очереди\n", tank_num, free_channel_idx);
      return true;
    }
  }
  printf("-----------\n");
  return false;
}

/**
* Set busy status for any free service channel
*/
bool set_any_service_channel_busy(double current_time, int tank_number)
{
  for (size_t i = 0; i < ARRSIZE(service_channels); i++) {
    if (service_channels[i].is_free(current_time)) {
      printf("Бак %.4d перешел в обслуживание в канал %zd\n", tank_number, i);
      service_channels[i].set_busy(current_time, tank_number);
      return true;
    }
  }
  return false;
}

int main()
{
  double random_number, order_time, total_time = 0.0;
  setlocale(LC_ALL, "ru");
  srand((unsigned int)time(nullptr));

  for (size_t i = 0; i < num_exps; i++) {
    printf("======== Эксперимент %zd =========\n", i);
    total_time = 0.0;
    order_time = 0.0;
    random_number = 0.0;

    /* reset queue */
    while (!queue.empty())
      queue.pop();

    /* reset time in all service channels */
    for (auto channel : service_channels)
      channel.reset();

    std::default_random_engine re;
    std::uniform_real_distribution<double> unif(0.0, 1.0);
    while (total_time < total_exp_time) {
      random_number = unif(re);
      order_time = (1.0 / random_number) * log(statistic_avg);
      total_time += order_time;

      //printf("total: %lf  rand: %lf  order_time: %lf\n", total_time, random_number, order_time);

      int new_tank_id = rand() % 9999;

      /* 1. check queue for start service any tank from queue */
      service_channels_check_queue(total_time);

      /* 2. check all service channels for busy */
      if (!set_any_service_channel_busy(total_time, new_tank_id)) {

        // All channels is busy. Add this order to queue
        if (queue.size() >= MAX_QUEUE_SIZE) {
          printf("Бак %.4d получил отказ\n", new_tank_id); //drop
          continue;
        }
        printf("Бак %.4d встал в очередь\n", new_tank_id);
        queue.push(new_tank_id);
      }
    }
    printf("\n\n\n\n\n");
  }
  return 0;
}