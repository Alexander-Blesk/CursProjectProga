# Курсовой проект: Программа контроля облачной инфраструктуры

## Контрольная точка 1 (КТ1): Пять тестовых значений — формулы и базовый контроль

### Вариант 6

- **Компоненты:** `RPC`, `NIC`, `HDD`
- **Порядок записи во входных данных:** формат `В` (`компонент время значение`, разделитель — точка с запятой).
- **Тип последовательности (4):** Порядок произвольный; возможны пропуски измерений отдельных компонентов.
- **Телеметрия за пять отсчётов (одна строка):**

  ```
  RPC;4;47.0; HDD;1;33.9; NIC;3;57.0; RPC;1;45.5; NIC;2;53.6; RPC;5;52.9; NIC;5;53.0; NIC;1;55.6; HDD;4;31.6; RPC;2;46.0; HDD;5;30.9; RPC;3;46.5; HDD;3;32.4
  ```
- **Производный показатель $Q$:** Логарифм отношения
  Формула: Q[i] = ln(X[i] / X[i−k]); k = 3; L = 1.5; D = 20.
  Параметры: $k = 3$, $L = 2.0\%$, $D = 25$.
- **Правило состояния компонента (Выход за диапазон):**  
  Если Q < Lmin или Q > Lmax, состояние «ТРЕВОГА», иначе «НОРМА».
- **Правило инцидента - Ровно один проблемный:**  
  Инцидент возникает,если ровно один компонент проблемный, а два других находятся в «НОРМА».
- **Структура Incident:** typedef struct { int time; int type; float value; } Incident;
  Поля:номер отсчёта, тип инцидента, значение показателя.

## 1. Алгоритм и блок-схема

### Описание алгоритма
- **Инициализация**: Задать исходные массивы данных телеметрии для трех компонентов (`RPC`, `NIC`, `HDD`) на интервале времени от $1$ до $5$ с учётом возможных пропусков
- **Расчет показателя $Q$:** Для каждого отсчёта $t$ (при $t > k$, где $k = 3$) и для каждого компонента вычислить производный показатель по формуле $Q[i] = \ln(X[i] / X[i-3])$
- **Определение состояния компонентов:** Для отсчётов $t \le 3$ или при наличии пропусков статус устанавливается как «НОРМА». Сравнить значение $Q$ с порогами: если $\vert{}Q\vert{} > 1.5$, зафиксировать статус «ТРЕВОГА», иначе — «НОРМА».
- **Проверка возникновения инцидента:** Проверить правило: если ровно один компонент находится в состоянии «ТРЕВОГА», а остальные два — в «НОРМА», зафиксировать инцидент с сохранением номера отсчёта, типа и значения показателя.
- **Вывод результатов:** Вывести исходные данные телеметрии, рассчитанные значения $Q$, статусы компонентов и итоговый журнал инцидентов.
### Блок-схема алгоритма (КТ1)

<img width="628" height="806" alt="image" src="https://github.com/user-attachments/assets/b2c7ee92-4a62-41e5-811d-d12f08214292" />

Ссылка на блок-схему к КТ(1) - <a href="https://viewer.diagrams.net/?tags=%7B%7D&lightbox=1&highlight=0000ff&edit=_blank&layers=1&nav=1&dark=auto#R%3Cmxfile%3E%3Cdiagram%20name%3D%22%D0%A1%D1%82%D1%80%D0%B0%D0%BD%D0%B8%D1%86%D0%B0-1%22%20id%3D%22WJzUy3EMjr-eLaRKOxhb%22%3E7Vpbc5s4FP41zLQPzQhxf7SN03amu5NuutvuIzGKocHII%2BPEzq9fgSSQwPfIuPVsJoPF0ZEQ55xP54IMazRbfSTRPPkDxygzIIhXhhUaEJp2YNGfkrJmlMCFjDAlacyZGsJ9%2Boo4EXDqMo3RQmEsMM6KdK4SJzjP0aRQaBEh%2BEVle8SZ%2BtR5NEUdwv0kyrrU72lcJIzqQ6%2Bhf0LpNBFPNt2A9cwiwczfZJFEMX6RSNbYsEYE44K1ZqsRykrhCbmwcbdbeuuFEZQXhwxI8qf8Mfg7f%2F02SJKH6es%2F6PvDB5vNguKOFJppOWmBl2SCdswl%2BIq1EF457T2%2FxaRI8BTnUTZuqEOCl3mMyhUCetfwfMF4TokmJf5ERbHmhhEtC0xJSTHLeG8tSGqBCM9QQdZ0DQRlUZE%2Bq%2B8UcVOY1nz10Duc0reFgJut5XGdcaMVt2KGIiJTVPBBjchpQ1pFQ6oUcYRSduhAkm2WUQyUMnxJ0gLdz6NKOS8UhqqEqN3N0ed8QXEkyQyC5yhb8rkMKv1hWF2BQVl8T7TpdVhdx3wQIgVaSWvqyj6R4GBzwb000HE5SYjaVkVtgu3KUuR8rFBdjZbuXK2lm26%2Fpu4cYuqq6PaY%2B0bjDvzKrM3q6nCDLq%2B2MH1KZ4ZOVwSkLiBho8XmRrPy2fnDovz5645KgP6DPz%2FzxqcwbDNR6lM5f7kwYDHCl5pg3jgVqT3GzahohnH6TJvTshnWQ8o9n3VT8UscJ6HV7aLVhC24mrt3Rm1w9TXC1btauEKnX7h625WgE64DIPkiU%2FJITtWGAqK3EpIHElw9aTgUA2tXxtpBexLI%2BG0xJ20zhlHVtpQZ9mP0q%2BEMU8NpoJrl735INKoaCBhhDA3fNwL4RLveXxbPnf1fG6DFg7QgOrhaRNfxUE%2BIDnpywLcdp2tJQINSL1Tg3IC0xmNNtAUkaddt5Vqt0OoLKZ2tVx9SHI1IMa8nKzN9VQM27Dkt05lCXJFeHO%2FCejkoXyYJnj0sF1ry5a%2Fv0vd1CFB56zJnH9Wpgy%2FFCjSQBQp%2F5c85%2BwlblbdhqwocZauCHrxxdnsVfZuVdYjw6QsWqpQ3KWFREPyERjjDhBJznJc2%2F5hmWYsUZek0p7cT%2BkRE6cNShOkkyga8Y5bG8QbAbPZMlRcJwEnKsPbWN%2Bry3%2Fk1Yf%2Fumqiy7KAKzwfwTPqw3b70AXUGvOZFcli6TLL%2BUY6vKhPV7b98uuomXCl3a0W9bNvf9VLBG%2F3R2%2FDSVzILpdjVkULcsRQGn7b9bCivtsNWE%2B5xzvrs3dRp7%2F7%2F9q7b3v1%2B7J1t4%2B0Kyjlt3DF7s3Gt38uup4rRTgFc0G8KAHWWiy%2BjlzPvPdA9QvFcqx%2FAjSuKEVttYUBItJYY5qWOF9tNpe2QHNuRLWIvv2uBnfztglqLnzbYirc8zd48urdc9rB6nMZcVqq3y%2FU3sYlDucuWsttQdA0khlGnLB9KYU%2BoVu9Zg1X4a4dhqvXAqrLXzOPXKbVgOyCysm5Pcjx%2B1%2FHAluOBrWTCATvQ8jbHAw%2Bxi185uTtvmm27XqvkcUZd%2FPYljz4SbbpxdzRytrDM0%2Bj%2B4UVOd%2FxK7l%2B%2Ffno65jGQHdRA8i3MXbiil38vUryZwlD7OtVxnZ7FHPCBqZ3FdGJofeoIdMLFvQRc3mjNh4WOXgBugPQHew0FhWB3g6aK8GiTckVZhjI8JdGMCmiOSEpXUPoKte%2Bu6Tg%2BenxMV0gc0t1eAIDSOSk5ILOluG3jmYyRcsijie3qsG%2FEvqSMTgobA23I3fu9xQ3OhV3hePs9HBrIQX4oack9SabHHwv1ThAovW1OVzNUNmfUrfF%2F%3C%2Fdiagram%3E%3C%2Fmxfile%3E"> Схема КТ(1) </a>

## 2. Контрольный пример: ручной расчёт

### Исходные данные телеметрии (упорядочены по отсчётам)

<img width="1172" height="432" alt="image" src="https://github.com/user-attachments/assets/27861f35-d190-44be-a8f2-99664fbff05f" />

### Ручной расчёт производного показателя $Q$ ($k = 3$)

- **Отсчёт $t = 4$ (пара с $t = 1$):**
  - RPC: $Q[4] = \ln\left(\frac{47.0}{45.5}\right) = \ln(1.032967) \approx 0.0324$
  - NIC: данные отсутствуют
  - HDD: $Q[4] = \ln\left(\frac{31.6}{33.9}\right) = \ln(0.93215) \approx -0.0702$
- **Отсчёт $t = 5$ (пара с $t = 2$):**
  - RPC: $Q[5] = \ln\left(\frac{52.9}{46.0}\right) = \ln(1.15) \approx 0.1398$
  - NIC: $Q[5] = \ln\left(\frac{53.0}{53.6}\right) = \ln(0.9888) \approx -0.0112$
  - HDD: данные отсутствуют

Ожидаемый результат: в рассматриваемом интервале из 5 отсчётов все значения $Q$ находятся в пределах допустимого диапазона ($\vert{}Q\vert{} \le 1.5$), поэтому инциденты не возникают.

### Определение состояний и инцидентов ($L = 1.5\%$)

<img width="1496" height="311" alt="image" src="https://github.com/user-attachments/assets/f272d2de-d827-4950-a30c-99aa410c6376" />

## 3. Реализация программы

Исходный код программы: [CursProject.c](CursProject.c)


## 4. Результаты работы программы

Вывод программы в консоли:

<img width="650" height="575" alt="image" src="https://github.com/user-attachments/assets/c5d820af-104f-4dae-a5b4-d99a515a493c" />

## 5. Вывод

Вывод программы полностью совпадает с результатами ручного контрольного расчёта.

## 6. Информация о разработчике

**Студент:** Васянин Александр Сергеевич

**Группа:** бОТИ-261

**Год:** 2026
