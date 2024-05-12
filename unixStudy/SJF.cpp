#include <iostream>
#include <queue>
using namespace std;
typedef double TimeType;
const int MAXN = 1e3 + 5;

int task_num;
TimeType now_time;        // 从第一个到达的作业时间开始
TimeType avg_spend_time;  // 平均周转时间
TimeType avg_weight_time; // 平均带权周转时间
TimeType avg_wait;
TimeType last_run[MAXN];

struct StruTask
{
    int id;                       // 序号
    TimeType arrive_time;         // 到达时间（输入）
    TimeType service_time;        // 服务时间（输入）
    TimeType start_time;          // 开始时间（设置）
    TimeType end_time;            // 结束时间（设置）
    TimeType spend_time;          // 周转时间:end_time - arrive_time
    TimeType weight_time;         // 带权周转时间spend_time/service_time
    TimeType remain_service_time; // 剩余服务时间

} arrayTask[MAXN];

TimeType get_task()
{
    cout << "请输入任务数:" << endl;
    cin >> task_num;
    if (task_num <= 0)
        return -1;
    cout << "----------请输入任务相关信息-------" << endl;
    TimeType minx = 1e15;
    for (int i = 1; i <= task_num; ++i)
    {
        cout << "请输入第" << i << "个任务的 到达时间 和 服务时间:";
        arrayTask[i].id = i;
        cin >> arrayTask[i].arrive_time;
        minx = min(minx, arrayTask[i].arrive_time);
        cin >> arrayTask[i].service_time;
        arrayTask[i].remain_service_time = arrayTask[i].service_time;
    }
    return minx;
}

int get_next_task()
{
    TimeType minx = 1e15;
    int aim = -1;
    for (int i = 1; i <= task_num; ++i)
    {
        if (arrayTask[i].arrive_time <= now_time && arrayTask[i].remain_service_time)
        {
            if (arrayTask[i].remain_service_time < minx)
            {
                minx = arrayTask[i].remain_service_time;
                aim = i;
            }
        }
    }
    return aim;
}

void run_task(StruTask &task)
{
    now_time = max(now_time, task.arrive_time);
    avg_wait += now_time - (last_run[task.id] == -1 ? task.arrive_time : last_run[task.id]);

    task.start_time = now_time;
    task.end_time = now_time + 1; // 执行一秒后询问，便于实现
    task.remain_service_time -= 1;
    now_time = task.end_time;
    last_run[task.id] = now_time;
    if (!task.remain_service_time)
    {
        task.spend_time = task.end_time - task.arrive_time;
        avg_spend_time += task.spend_time;
        task.weight_time = task.spend_time / task.service_time;
        avg_weight_time += task.weight_time;
    }
}

void print_result(StruTask &task)
{
    cout << " id: " << task.id
         << " arrive_time: " << task.arrive_time << " service: " << task.service_time;
    cout << " start: " << task.start_time << " end: " << task.end_time;
    cout << " spend: " << task.spend_time << " weight: " << task.weight_time << endl;
}

/*SJF 抢占式:又叫最短剩余时间优先
Shortest remaining time first
*/
void SRTF()
{

    avg_spend_time = 0;
    avg_weight_time = 0;
    avg_wait = 0;
    for (int i = 0; i < MAXN; ++i)
    {
        last_run[i] = -1;
    }
    int id;
    while (1)
    {
        id = get_next_task();
        if (id == -1)
            break;
        run_task(arrayTask[id]);
        print_result(arrayTask[id]);
    }
}

int main()
{
    now_time = get_task();
    if (now_time == -1)
        return 0;
    SRTF();
    avg_spend_time /= task_num;
    avg_weight_time /= task_num;
    avg_wait /= task_num;
    cout << " avg_spend: " << avg_spend_time << " avg_weight: " << avg_weight_time << endl;
    cout << " avg_wait: " << avg_wait << endl;
    return 0;
}
