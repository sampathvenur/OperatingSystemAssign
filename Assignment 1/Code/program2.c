#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;       // Process ID
    int burst;     // Burst Time
    int priority;  // Priority
    int arrival;   // Arrival Time
} Process;

// Function to compare based on arrival time
int compare_arrival(const void *a, const void *b) {
    return ((Process *)a)->arrival - ((Process *)b)->arrival;
}

// Function to compare based on burst time
int compare_burst(const void *a, const void *b) {
    return ((Process *)a)->burst - ((Process *)b)->burst;
}

// Function to compare based on priority
int compare_priority(const void *a, const void *b) {
    return ((Process *)a)->priority - ((Process *)b)->priority;
}

// FCFS Scheduling
void fcfs(Process *proc, int n) {
    qsort(proc, n, sizeof(Process), compare_arrival);
    int wait_time[n], turnaround_time[n];
    int total_wait = 0, total_turnaround = 0;

    wait_time[0] = 0;
    for (int i = 1; i < n; i++) {
        wait_time[i] = proc[i-1].burst + wait_time[i-1];
    }
    for (int i = 0; i < n; i++) {
        turnaround_time[i] = proc[i].burst + wait_time[i];
        total_wait += wait_time[i];
        total_turnaround += turnaround_time[i];
    }

    printf("FCFS Scheduling:\n");
    printf("Average Waiting Time: %.2f\n", (float)total_wait / n);
    printf("Average Turnaround Time: %.2f\n\n", (float)total_turnaround / n);
}

// SJF Scheduling
void sjf(Process *proc, int n) {
    qsort(proc, n, sizeof(Process), compare_burst);
    int wait_time[n], turnaround_time[n];
    int total_wait = 0, total_turnaround = 0;

    wait_time[0] = 0;
    for (int i = 1; i < n; i++) {
        wait_time[i] = proc[i-1].burst + wait_time[i-1];
    }
    for (int i = 0; i < n; i++) {
        turnaround_time[i] = proc[i].burst + wait_time[i];
        total_wait += wait_time[i];
        total_turnaround += turnaround_time[i];
    }

    printf("SJF Scheduling:\n");
    printf("Average Waiting Time: %.2f\n", (float)total_wait / n);
    printf("Average Turnaround Time: %.2f\n\n", (float)total_turnaround / n);
}

// Priority Scheduling
void priority_scheduling(Process *proc, int n) {
    qsort(proc, n, sizeof(Process), compare_priority);
    int wait_time[n], turnaround_time[n];
    int total_wait = 0, total_turnaround = 0;

    wait_time[0] = 0;
    for (int i = 1; i < n; i++) {
        wait_time[i] = proc[i-1].burst + wait_time[i-1];
    }
    for (int i = 0; i < n; i++) {
        turnaround_time[i] = proc[i].burst + wait_time[i];
        total_wait += wait_time[i];
        total_turnaround += turnaround_time[i];
    }

    printf("Priority Scheduling:\n");
    printf("Average Waiting Time: %.2f\n", (float)total_wait / n);
    printf("Average Turnaround Time: %.2f\n\n", (float)total_turnaround / n);
}

// Round Robin Scheduling
void round_robin(Process *proc, int n, int quantum) {
    int wait_time[n], turnaround_time[n], remaining_burst[n];
    int total_wait = 0, total_turnaround = 0, time = 0;
    for (int i = 0; i < n; i++) {
        remaining_burst[i] = proc[i].burst;
    }

    while (1) {
        int done = 1;
        for (int i = 0; i < n; i++) {
            if (remaining_burst[i] > 0) {
                done = 0;
                if (remaining_burst[i] > quantum) {
                    time += quantum;
                    remaining_burst[i] -= quantum;
                } else {
                    time += remaining_burst[i];
                    wait_time[i] = time - proc[i].burst;
                    remaining_burst[i] = 0;
                }
            }
        }
        if (done) {
            break;
        }
    }
    for (int i = 0; i < n; i++) {
        turnaround_time[i] = proc[i].burst + wait_time[i];
        total_wait += wait_time[i];
        total_turnaround += turnaround_time[i];
    }

    printf("Round Robin Scheduling (Quantum %d):\n", quantum);
    printf("Average Waiting Time: %.2f\n", (float)total_wait / n);
    printf("Average Turnaround Time: %.2f\n\n", (float)total_turnaround / n);
}

int main() {
    Process proc[] = {
        {1, 10, 3, 0},
        {2, 5, 1, 0},
        {3, 8, 2, 0},
        {4, 6, 4, 0}
    };
    int n = sizeof(proc) / sizeof(proc[0]);
    int quantum = 3;

    fcfs(proc, n);
    sjf(proc, n);
    priority_scheduling(proc, n);
    round_robin(proc, n, quantum);

    return 0;
}
