#include <iostream>
#include <iomanip>
#include <queue>
using namespace std;

struct process
{
    float AT, BT, FT, WT, RT, TAT;
    int QId;
    char name;
};

int main()
{
    int no_of_process, num, Q0_time_sum, Q1_time_sum, Q2_time_sum;
    int flag0, flag1, flag2;
    cout << "Enter Number of Process: ";
    cin >> no_of_process;
    num = no_of_process;
    process P[no_of_process];
    for (int i = 0; i < no_of_process; i++)
    {
        cout << "Enter Name(A-Z): ";
        cin >> P[i].name;
        cout << "Enter Arrival Time: ";
        cin >> P[i].AT;
        cout << "Enter Burst Time: ";
        cin >> P[i].BT;
        P[i].QId = -1;
        P[i].WT = 0;
    }
    queue<int> Q0, Q1, Q2;
    // Sort Process based on Arival Time
    for (int i = 0; i < no_of_process; i++)
    {
        for (int j = 0; j < no_of_process - i - 1; j++)
        {
            if (P[j].AT > P[j + 1].AT)
            {
                process swap_var = P[j];
                P[j] = P[j + 1];
                P[j + 1] = swap_var;
            }
        }
    }
    // Enque proces in Queue
    Q0.push(0);
    P[0].QId = 0;
    float Time = P[0].AT;
    while (no_of_process)
    {
        flag0 = flag1 = flag2 = 0;
        Q0_time_sum = Q1_time_sum = Q2_time_sum = 0;
        while (!Q0.empty())
        {
            flag0 = 1;
            int id = Q0.front();
            if (P[id].AT <= Time + Q0_time_sum)
            {
                Q0.pop();
                P[id].RT = Time + Q0_time_sum - P[id].AT;
                no_of_process--;
            }
            int reducer = (P[id].BT > 8) ? 8 : (P[id].BT);
            P[id].BT -= reducer;
            if (P[id].BT)
            {
                Q1.push(id);
                P[id].QId = 1;
                no_of_process++;
            }
            P[id].FT = Time + Q0_time_sum + reducer;
            P[id].WT = Time + Q0_time_sum - P[id].AT;
            Q0_time_sum += reducer;
            for (int i = 0; i < num; i++)
            {
                if (P[i].AT <= Time + Q0_time_sum && P[i].QId == -1)
                {
                    Q0.push(i);
                    P[i].QId = 0;
                }
            }
        }
        Time += Q0_time_sum;
        while (!Q1.empty() && Q0.empty())
        {
            flag1 = 1;
            int id = Q1.front();
            if (P[id].AT <= Time + Q1_time_sum)
            {
                Q1.pop();
                no_of_process--;
            }
            int red = (P[id].BT > 16) ? 16 : (P[id].BT), k;
            for (k = 1; k <= red; k++)
            {
                P[id].BT -= 1;
                for (int i = 0; i < num; i++)
                {
                    if (P[i].AT <= Time + k && P[i].QId == -1)
                    {
                        Q0.push(i);
                        P[i].QId = 0;
                        break;
                    }
                }

                if (!Q0.empty())
                {
                    k += 1;
                    break;
                }
            }
            if (P[id].BT && k == 17)
            {
                Q2.push(id);
                P[id].QId = 2;
                no_of_process++;
            }
            else if (P[id].BT)
            {
                Q1.push(id);
                P[id].QId = 1;
                no_of_process++;
            }
            P[id].WT += Time + Q1_time_sum - P[id].FT;
            P[id].FT = Time + Q1_time_sum + k - 1;
            Q1_time_sum += k - 1;
        }
        Time += Q1_time_sum;
        while (!Q2.empty() && Q1.empty() && Q0.empty())
        {
            flag2 = 1;
            int id = Q2.front();
            if (P[id].AT <= Time + Q2_time_sum)
            {
                Q2.pop();
                no_of_process--;
            }
            int red = P[id].BT, k;
            for (k = 1; k <= red; k++)
            {
                P[id].BT -= 1;
                for (int i = 0; i < num; i++)
                {
                    if (P[i].AT <= Time + k + Q2_time_sum && P[i].QId == -1)
                    {
                        Q0.push(i);
                        P[i].QId = 0;
                        break;
                    }
                }
                if (!Q0.empty())
                {
                    k += 1;
                    break;
                }
            }
            if (P[id].BT)
            {
                Q2.push(id);
                P[id].QId = 2;
                no_of_process++;
            }
            P[id].WT += Time + Q2_time_sum - P[id].FT;
            Q2_time_sum += k - 1;
            P[id].FT = Time + Q2_time_sum;
        }
        Time += Q2_time_sum;
        for (int i = 0; i < num; i++)
        {
            if (P[i].AT <= Time + 1 && P[i].QId == -1)
            {
                Q0.push(i);
                P[i].QId = 0;
            }
        }
        Time += (flag0 == 0 && flag1 == 0 && flag2 == 0) ? 1 : 0;
    }
    cout << "Name     FT       RT     WT      TAT\n";
    float avg_FT = 0, avg_RT = 0, avg_WT = 0, avg_TAT = 0;
    for (int j = 0; j < num; j++)
    {
        P[j].TAT = P[j].FT - P[j].AT;
        avg_FT += P[j].FT;
        avg_RT += P[j].RT;
        avg_WT += P[j].WT;
        avg_TAT += P[j].TAT;
        cout << "  " << P[j].name << setw(8) << P[j].FT << setw(8) << P[j].RT << setw(8) << P[j].WT << setw(8) << P[j].TAT << endl;
    }
    avg_FT /= num;
    avg_RT /= num;
    avg_WT /= num;
    avg_TAT /= num;
    cout << "\nAverage Finishing Time: " << avg_FT << "\nAverage Response Time: " << avg_RT << "\nAverage Waiting Time: " << avg_WT << "\nAverage Turn Around Time: " << avg_TAT << endl;
    return 0;
}
