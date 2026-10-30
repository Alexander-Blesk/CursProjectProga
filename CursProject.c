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

int main(void) {
    setlocale(LC_CTYPE, "RUS");

    
    float rpc[POINTS_COUNT] = { 45.5f, 46.0f, 46.5f, 47.0f, 52.9f };
    float nic[POINTS_COUNT] = { 55.6f, 53.6f, 57.0f, -1.0f, 53.0f };
    float hdd[POINTS_COUNT] = { 33.9f, -1.0f, 32.4f, 31.6f, 30.9f };

    float q_rpc[POINTS_COUNT] = { 0 };
    float q_nic[POINTS_COUNT] = { 0 };
    float q_hdd[POINTS_COUNT] = { 0 };

    int state_rpc[POINTS_COUNT] = { 0 };
    int state_nic[POINTS_COUNT] = { 0 };
    int state_hdd[POINTS_COUNT] = { 0 };

    Incident incidents[POINTS_COUNT];
    int incident_count = 0;

    int i;

    
    printf("Контроль облачной инфраструктуры (Вариант 6, КТ1)\n\n");
    printf("Исходные данные телеметрии:\n");
    printf("Отсчёт\tRPC\tNIC\tHDD\n");
    for (i = 0; i < POINTS_COUNT; i++) {
        printf("%d\t", i + 1);

        if (rpc[i] >= 0) printf("%.1f\t", rpc[i]); else printf("—\t");
        if (nic[i] >= 0) printf("%.1f\t", nic[i]); else printf("—\t");
        if (hdd[i] >= 0) printf("%.1f\t", hdd[i]); else printf("—\t");

        printf("\n");
    }
    printf("\n");

    
    for (i = K; i < POINTS_COUNT; i++) {
        if (rpc[i] > 0 && rpc[i - K] > 0) {
            q_rpc[i] = log(rpc[i] / rpc[i - K]);
        }
        if (nic[i] > 0 && nic[i - K] > 0) {
            q_nic[i] = log(nic[i] / nic[i - K]);
        }
        if (hdd[i] > 0 && hdd[i - K] > 0) {
            q_hdd[i] = log(hdd[i] / hdd[i - K]);
        }
    }

    
    printf("Логарифмический показатель Q (при k = %d):\n", K);
    printf("Отсчёт\tQ(RPC)\t\tQ(NIC)\t\tQ(HDD)\n");
    for (i = K; i < POINTS_COUNT; i++) {
        printf("%d\t%+.4f\t\t%+.4f\t\t%+.4f\n",
            i + 1, q_rpc[i], q_nic[i], q_hdd[i]);
    }
    printf("\n");

    for (i = 0; i < POINTS_COUNT; i++) {
        if (i >= K && rpc[i] >= 0 && rpc[i - K] >= 0) {
            if (q_rpc[i] < -L_THRESHOLD || q_rpc[i] > L_THRESHOLD) {
                state_rpc[i] = 1; 
            }
        }
        if (i >= K && nic[i] >= 0 && nic[i - K] >= 0) {
            if (q_nic[i] < -L_THRESHOLD || q_nic[i] > L_THRESHOLD) {
                state_nic[i] = 1; 
            }
        }
        if (i >= K && hdd[i] >= 0 && hdd[i - K] >= 0) {
            if (q_hdd[i] < -L_THRESHOLD || q_hdd[i] > L_THRESHOLD) {
                state_hdd[i] = 1; 
            }
        }

        
        int prob_count = state_rpc[i] + state_nic[i] + state_hdd[i];
        if (prob_count == 1) {
            incidents[incident_count].time = i + 1;
            incidents[incident_count].type = 1;

            
            if (state_rpc[i]) incidents[incident_count].value = q_rpc[i];
            else if (state_nic[i]) incidents[incident_count].value = q_nic[i];
            else incidents[incident_count].value = q_hdd[i];

            incident_count++;
        }
    }

    printf("Состояние компонентов (порог L = %.1f):\n", L_THRESHOLD);
    printf("Отсчёт\tRPC\t\tNIC\t\tHDD\n");
    for (i = 0; i < POINTS_COUNT; i++) {
        printf("%d\t%s\t\t%s\t\t%s\n",
            i + 1,
            state_rpc[i] ? "ТРЕВОГА" : "НОРМА",
            state_nic[i] ? "ТРЕВОГА" : "НОРМА",
            state_hdd[i] ? "ТРЕВОГА" : "НОРМА"
        );
    }
    printf("\n");

    
    printf("Журнал инцидентов:\n");
    if (incident_count == 0) {
        printf("Инцидентов не зафиксировано (все компоненты в норме).\n");
    }
    else {
        for (i = 0; i < incident_count; i++) {
            printf("Отсчёт %d: зафиксирован инцидент (тип %d, Q = %.4f)\n",
                incidents[i].time, incidents[i].type, incidents[i].value);
        }
    }

    return 0;
}