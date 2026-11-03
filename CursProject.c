#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <math.h>

#define POINTS_COUNT 5
#define K 3
#define L_THRESHOLD 1.5f


typedef struct {
    int time;
    int type;
    float value;
} Incident;


float get_telemetry(int comp, int t) {
    if (comp == 0) { 
        float rpc_data[] = { 0, 45.5f, 46.0f, 46.5f, 47.0f, 52.9f };
        return rpc_data[t];
    }
    else if (comp == 1) { 
        float nic_data[] = { 0, 55.6f, 53.6f, 57.0f, -1.0f, 53.0f };
        return nic_data[t];
    }
    else if (comp == 2) { 
        float hdd_data[] = { 0, 33.9f, -1.0f, 32.4f, 31.6f, 30.9f };
        return hdd_data[t];
    }
    return -1.0f;
}

float calculate_q(int comp, int t) {
    if (t <= K) return 0.0f;
    float curr = get_telemetry(comp, t);
    float prev = get_telemetry(comp, t - K);
    if (curr < 0 || prev < 0 || prev == 0) return 0.0f;
    return log(curr / prev); 
}


int check_state(int comp, int t) {
    if (t <= K) return 0;
    float curr = get_telemetry(comp, t);
    float prev = get_telemetry(comp, t - K);
    if (curr < 0 || prev < 0) return 0;

    float q = calculate_q(comp, t);
    if (q < -L_THRESHOLD || q > L_THRESHOLD) {
        return 1; 
    }
    return 0; 
}

int main(void) {
    setlocale(LC_ALL, "Russian");

    Incident incidents[POINTS_COUNT];
    int incident_count = 0;
    int t, c;


    printf("Контроль облачной инфраструктуры (Вариант 6, КТ1)\n\n");
    printf("Исходные данные телеметрии:\n");
    printf("Отсчёт\tRPC\tNIC\tHDD\n");
    for (t = 1; t <= POINTS_COUNT; t++) {
        printf("%d\t", t);
        for (c = 0; c < 3; c++) {
            float val = get_telemetry(c, t);
            if (val >= 0) printf("%.1f\t", val);
            else printf("—\t");
        }
        printf("\n");
    }
    printf("\n");


    printf("Логарифмический показатель Q (при k = %d):\n", K);
    printf("Отсчёт\tQ(RPC)\t\tQ(NIC)\t\tQ(HDD)\n");
    for (t = K + 1; t <= POINTS_COUNT; t++) {
        printf("%d\t", t);
        for (c = 0; c < 3; c++) {
            float curr = get_telemetry(c, t);
            float prev = get_telemetry(c, t - K);
            if (curr >= 0 && prev >= 0) {
                float q = calculate_q(c, t);
                printf("%+.4f\t\t", q);
            }
            else {
                printf("—\t\t");
            }
        }
        printf("\n");
    }
    printf("\n");


    for (t = 1; t <= POINTS_COUNT; t++) {
        int state_rpc = check_state(0, t);
        int state_nic = check_state(1, t);
        int state_hdd = check_state(2, t);


        int prob_count = state_rpc + state_nic + state_hdd;
        if (prob_count == 1) {
            incidents[incident_count].time = t;
            incidents[incident_count].type = 1;
            if (state_rpc) incidents[incident_count].value = calculate_q(0, t);
            else if (state_nic) incidents[incident_count].value = calculate_q(1, t);
            else incidents[incident_count].value = calculate_q(2, t);
            incident_count++;
        }
    }

   
    printf("Состояние компонентов (порог L = %.1f):\n", L_THRESHOLD);
    printf("Отсчёт\tRPC\t\tNIC\t\tHDD\n");
    for (t = 1; t <= POINTS_COUNT; t++) {
        printf("%d\t%s\t\t%s\t\t%s\n",
            t,
            check_state(0, t) ? "ТРЕВОГА" : "НОРМА",
            check_state(1, t) ? "ТРЕВОГА" : "НОРМА",
            check_state(2, t) ? "ТРЕВОГА" : "НОРМА"
        );
    }
    printf("\n");

    printf("Журнал инцидентов:\n");
    if (incident_count == 0) {
        printf("Инцидентов не зафиксировано (все компоненты в норме).\n");
    }
    else {
        for (int i = 0; i < incident_count; i++) {
            printf("Отсчёт %d: зафиксирован инцидент (тип %d, Q = %.4f)\n",
                incidents[i].time, incidents[i].type, incidents[i].value);
        }
    }

    return 0;
}