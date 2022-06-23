#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Modify definec value if needed
#define MaxArrivalTime 10
#define MaxBurstTime 20
#define N 8 // total number of process
#define PairSize 100

#define INT_MAX 2147483647


struct pair
{
    int first;
    int second;
    int third;
};


void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int getMinInt(int* n, int size) {
  int min = n[0];
  for (int i = 1; i < size; i++)
    if (n[i] < min) min = n[i];
  return min;
}

void PrintMenu();
void ShortestJobFirst();
void RoundRobin();
void PrintValue();
void PrintGantt();
int getMinInt(int* n, int size);
int random_time_quan();

int main()
{
    srand((unsigned)time(NULL)); // uncomment to upgrade random
    // burst time, process no, arrival time, priority
    int bt[N], p[N], at[N], pri[N];
    int time_quantum = 0;

    // menu
    PrintMenu();
    while (1)
    {
        char c = getchar();
        while ((getchar()) != '\n')
        {
        };
        if (c == '4')
            break;
        else
            PrintMenu();
        switch (c)
        {
        case '1':

            for (int i = 0; i < N; i++)
            {
                bt[i] = rand() % MaxBurstTime + 1;
                p[i] = i + 1;
                pri[i] = i + 1;
            }
            // unique random for at
            for (int i = 0; i < N; i++)
            {
                at[i] = rand() % (MaxArrivalTime + 1);
                for (int j = 0; j < i; j++)
                {
                    if (at[i] == at[j])
                    {
                        i--;
                        break;
                    }
                }
            }
            // shuffle priority
            for (int i = 0; i < 100; i++)
            {
                int a = rand() % (N - 1) + 1;
                swap(&pri[0], &pri[a]);
            }
            // print process info
            printf("Generating Process\n");
            printf("___________________\n");
            printf("P#      BT      AT      Pri\n");
            for (int i = 0; i < N; i++)
            {
                PrintValue(p[i]);
                PrintValue(bt[i]);
                PrintValue(at[i]);
                PrintValue(pri[i]);
                printf("\n");
            }
            printf("___________________\n");
            break;
        case '2':
            ShortestJobFirst(p, bt, at, pri);
            break;
        case '3':
        	printf("Enter time quantum : ");
        	scanf("%d",&time_quantum);
            RoundRobin(p, bt, at, time_quantum);
            break;
        default:
            break;
        }
    }
}

void PrintMenu()
{
    printf("_____ M E N U _____\n");
    printf("1. Generate Process\n");
    printf("2. Shortest Job First\n");
    printf("3. Round Robin\n");
    printf("4. EXIT\n");
    printf("___________________\n");
}

void PrintValue(int a)
{
    if (a < 10)
    {
        printf("%d       ", a);
    }
    else if (a > 99)
    {
        printf("%d     ", a);
    }
    else
    {
        printf("%d      ", a);
    }
}

void PrintGantt(struct pair order[N], int bt[]) {

    // first line
    printf("+");
    for (int i = 0; i < N; i++) {
    	if(order[i].first >0){
    		printf("-");
        	for (int j = 0; j < bt[order[i].first-1]; j++) {
        	printf("-");
    	}
   	 	printf("-+");
		}
    }
    // second line => show process number
    printf("\n|");
    for (int i = 0; i < N; i++) {
		if(order[i].first >0){
        for (int j = 0; j < floor(bt[order[i].first-1]/2); j++) {
            printf("-");
        }
        printf("P%d", order[i].first);
        for (int j = floor(bt[order[i].first-1]/2); j < bt[order[i].first-1]; j++) {
            printf("-");
        }
        printf("|");
    }
}
    // third line == first line
    printf("\n+");
    for (int i = 0; i < N; i++) {
    	if(order[i].first >0){
        printf("-");
        for (int j = 0; j < bt[order[i].first-1]; j++) {
            printf("-");
        }
        printf("-+");
    }
}
    // last line => show start time
    printf("\n");
    for (int i = 0; i < N; i++) {
    	if(order[i].first > 0){
        if (order[i].second < 10) printf("%d-", order[i].second);
        else printf("%d", order[i].second);
        printf("-");
        if(order[i].second >=100){
        	for (int j = 0; j < bt[order[i].first-1] - 1 ; j++) {
            printf("-");
		} }
		else {
			 for (int j = 0; j < bt[order[i].first-1]; j++) {
            printf("-");
        }
		}

    }
}
    printf("-");

printf("\n");


}


void ShortestJobFirst(int p[], int bt[], int at[], int pri[])
{
    // execute every time process arrives
    int time[(MaxArrivalTime + 1) * MaxBurstTime * N];
    int excl[N], ready[N];
    memset(time, 0, (MaxArrivalTime + 1) * MaxBurstTime * N * sizeof(int));
    memset(excl, 0, N * sizeof(int));
    memset(ready, 0, N * sizeof(int));

    int wt[N], tat[N], rt[N];

    struct pair order[N];
    memset(order, 0, N * sizeof(struct pair));
    int q = 0;

    for (int i = 0; i < N; i++)
    {
        time[at[i]] = 1;
    }

    int i = 0;
    while (order[N - 1].first == 0)
    {
        // execute sjf
        if (time[i])
        {
            // include arrived process
            memset(ready, 0, N * sizeof(int));
            for (int j = 0; j < N; j++)
            {
                // exclude currently running & done process
                if (at[j] <= i && excl[j] != 1)
                {
                    ready[j] = 1;
                }
            }

            // execute shortest job
            int job_index = -1;
            int min = INT_MAX;
            int prev_pri = -1;
            for (int j = 0; j < N; j++)
            {
                if (ready[j] && bt[j] <= min)
                {
                    // same burst time more important process
                    if (bt[j] == min && pri[j] < prev_pri)
                    {
                        continue;
                    }
                    min = bt[j];
                    job_index = j;
                    prev_pri = pri[j];
                }
            }
            if (job_index == -1)
            {
                // if no process is ready
                i += 1;
                continue;
            }
            excl[job_index] = 1; // running
            order[q].first = job_index + 1;
            order[q].second = i;
            q += 1;

            // Waiting time = current time - arrived time
            wt[job_index] = i - at[job_index];
            // Turn around time = waiting time + burst time
            tat[job_index] = wt[job_index] + bt[job_index];

            // when job is done also need to execute sjf
            // and until then no need to be executed
            i += bt[job_index];
            time[i] = 1;
        }
        else
        {
            i += 1; // time[i] == 0 >> pass time
        }
    }

    // All process done
    // calc average
    float average_wt = 0;
    float average_tat = 0;

    for (int i = 0; i < N; i++)
    {
        average_wt += wt[i];
        average_tat += tat[i];
    }
    average_wt /= N;
    average_tat /= N;

    // print value
    printf("Shortest Job First\n");
    printf("___________________\n");
    printf("P#      BT      AT      WT      TAT     RT\n");
    for (int i = 0; i < N; i++)
    {
        PrintValue(p[i]);
        PrintValue(bt[i]);
        PrintValue(at[i]);
        PrintValue(wt[i]);
        PrintValue(tat[i]);
        PrintValue(wt[i]);
        printf("\n");
    }
    printf("___________________\n");
    printf("OR(P#)  StartTime      \n");
    for (int i = 0; i < N; i++)
    {
        PrintValue(order[i].first);
        PrintValue(order[i].second);
        printf("\n");
    }
    printf("average waiting time : %.1f\n", average_wt);
    printf("average response time : %.1f\n", average_wt);
    printf("average turn around time : %.1f\n", average_tat);
    printf("___________________\n");

    // draw gantt (with order data)
    PrintGantt(order, bt);
    // order.first == p#
    // order.second == starting time
    // bt[order.first] == burst time
}

//도착시간에 따라 재정렬하는 함수
void rdSort(int readyQ[N], int at[],int p[]){
	int at_cp[N];
	memcpy(at_cp, at, N * sizeof(int));
	for(int i=0; i<N; i++){
    	for(int j=0; i<N; j++){
    		int min = getMinInt(at_cp,N);
    		if(at[j] == min){
    			readyQ[i]=p[j]-1; //readyQ의 배열에 도착시간대로 정렬하여 프로세스 번호 넣어줌
    			at_cp[j] = MaxArrivalTime + 1;
    			break;
    		}
    	}
    }
}

void RoundRobin(int p[], int bt[], int at[], int time_quantum){

	/* 변수 선언 및 초기화 */
    int temp[N];
	int total_bt_time = 0;
    int wait_time[N], turnaround_time[N], counter[N],Res[N],readyQ[N],spent[N];
    float average_wait_time=0;
	float average_turnaround_time = 0;
	float average_response_time = 0;
	struct pair order[N];
    //초기화
    memset(temp, 0, N * sizeof(int));
	memset(spent, 0, N * sizeof(int));
	memset(wait_time, 0, N * sizeof(int));
	memset(turnaround_time, 0, N * sizeof(int));
	memset(counter, 0, N * sizeof(int));
	memset(Res, 0, N * sizeof(int));
	//memset(readyQ, 0, N * sizeof(int));


    //우선순위 (도착시간) 정렬
    rdSort(readyQ, at,p);
    int total = at[readyQ[0]];
     for (int i = 0; i < N; i++)
    {
        temp[i] = bt[i]; //temp = 남아있는 burst time.
        spent[i] = bt[i];
        total_bt_time = total_bt_time + bt[i];
    }


    // print value
    printf("___________________\n");
    printf("\nRound Robin\n");
    printf("time quantum : %d",time_quantum);
    printf("\n___________________\n");

    int count = 1;
    while(total_bt_time>0){ // 모든 burst time이 소진될때까지
    memset(order, 0, sizeof(order));
    printf("\n%d Round\n", count);
 		for(int i=0; i<N; i++){
 			int idx = readyQ[i];  // 현재 idx는 readyQ의 i번째 우선순위 프로세스
 			if(temp[idx]>0)  { // temp[idx] =  우선순위가 idx인 프로세스의 남은 burst time
 				if(temp[idx]<=time_quantum){ // time quantum 보다 남아있는 burst time이 작을때
	 				if(count == 1) Res[idx] = total;
	 					order[i].first = idx+1;
	 					order[i].second = total;
	 					spent[idx] = temp[idx]; //사용된  cpu time
	 					total_bt_time = total_bt_time - temp[idx];
		 				total = total + temp[idx];
		 				counter[idx] = count; //round 횟수
		 				wait_time[idx] = total - at[idx] - bt[idx];  // Waiting time = current time - arrived time
		 				turnaround_time[idx] = wait_time[idx] + bt[idx]; // Turn around time = waiting time + burst time
		                temp[idx] = 0;
		                continue;
					 }
				 else{
				 	if(count==1) Res[idx] = total;
				 	order[i].first = idx+1;
 					order[i].second = total;
 					spent[idx] = time_quantum;
				 	total_bt_time = total_bt_time - time_quantum;
					total = total + time_quantum;
				 	counter[idx] = count;
				 	temp[idx] = temp[idx] - time_quantum; //남은 burst time 은 현재 burst time에 time quantum 뺀 값.
	                continue;
			 	}
			 }else continue;
			 }
			 count++;
			 printf("\n");
			 PrintGantt(order, spent);
			 }


	printf("__________________________________________________\n");
    printf("P#      BT      AT	ROUND  	WT      TAT	RT\n");

    for (int i = 0; i < N; i++)
    {
        PrintValue(p[i]);
        PrintValue(bt[i]);
        PrintValue(at[i]);
        PrintValue(counter[i]);
        PrintValue(wait_time[i]);
        PrintValue(turnaround_time[i]);
        PrintValue(Res[i]-at[i]);
        printf("\n") ;
    }

 for (int i = 0; i < N; i++)
    {
        average_wait_time += wait_time[i];
        average_turnaround_time += turnaround_time[i];
        average_response_time += Res[i]-at[i];
    }
      printf("\naverage waiting time : %.1f\n", average_wait_time / N);
      printf("average turn around time : %.1f\n", average_turnaround_time / N);
      printf("average response time : %.1f\n", average_response_time / N);
       printf("___________________\n");

}
