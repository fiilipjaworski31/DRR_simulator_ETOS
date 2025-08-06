Here are the README files in English and Polish, formatted for a repository.

(English version/ polish below)

# Deficit Round Robin (DRR) Algorithm Simulator

## 1\. Introduction

[cite\_start]This project implements a simulator for a single network node that uses the **Deficit Round Robin (DRR)** algorithm for fair packet scheduling[cite: 17, 19]. [cite\_start]The simulator was created to study and verify the operation of DRR under various load conditions and with different types of network traffic[cite: 22]. [cite\_start]The implementation is written in C++[cite: 34].

## 2\. Project Goal and Scope

The main goal of the project was to create a functional simulator that handles two types of traffic:

- [cite\_start]**Poisson traffic**: traffic with an exponential distribution of inter-arrival times[cite: 24, 46].
- [cite\_start]**CBR (Constant Bit Rate) traffic**: traffic with a constant bit rate[cite: 24, 52].

[cite\_start]The simulator allows for the parameterization of both traffic sources (e.g., intensity, packet size) and the configuration of the DRR algorithm[cite: 25]. After the simulation, key performance metrics are generated, such as:

- [cite\_start]Average number of packets in the queue[cite: 26].
- [cite\_start]Average packet waiting time[cite: 26].
- [cite\_start]Average server load (utilization)[cite: 26].

## 3\. Implementation

### System Architecture

[cite\_start]The system is implemented in **C++** using modern programming techniques, such as polymorphism and smart pointers (`std::unique_ptr`)[cite: 34, 217]. The main components are:

- [cite\_start]**Stream parameter class hierarchy**: `CBRFlowParameters` and `PoissonFlowParameters` classes inherit from the base class `FlowParameters`, allowing for flexible handling of different traffic types[cite: 219, 220, 221].
- [cite\_start]**Queuing model**: Queues are implemented using `std::vector`, which ensures efficient FIFO (First-In, First-Out) operations[cite: 35, 237].
- [cite\_start]**Traffic generator**: It uses the `std::random` library with `std::mt19937` for generating pseudo-random numbers and `std::exponential_distribution` for modeling Poisson traffic[cite: 36, 37, 249, 250].

### DRR Algorithm

[cite\_start]The DRR scheduling mechanism works as follows[cite: 117]:

1.  [cite\_start]**Initialization**: The deficit counters for each queue are set to 0[cite: 146].
2.  **Serving**: In each round, a fixed quantum is added to the deficit counter. [cite\_start]Packets are served as long as the deficit is sufficient[cite: 113, 150, 151].
3.  [cite\_start]**Packet processing**: After a packet is served, the deficit is reduced by its size[cite: 154].
4.  [cite\_start]**Queue switching**: If a queue is empty or a packet is too large to be served in the current round, the algorithm moves to the next queue, and the unused deficit is saved for the next round[cite: 157, 158].

## 4\. Results Analysis

[cite\_start]Two main simulation scenarios were conducted[cite: 382]:

1.  **Two CBR streams**:
    - [cite\_start]Stream 1: 264 kbps (26.4% of link bandwidth)[cite: 384].
    - [cite\_start]Stream 2: 120 kbps (12.0% of link bandwidth)[cite: 384].
2.  **Two Poisson streams**:
    - [cite\_start]Stream 1: 320 kbps (32.0% of link bandwidth)[cite: 386].
    - [cite\_start]Stream 2: 160 kbps (16.0% of link bandwidth)[cite: 386].

[cite\_start]In both scenarios, a constant packet size of 80 bits, a link speed of 1 Mbps, a quantum of 12000 bits, and a simulation time of 10000 seconds were used[cite: 382, 384, 386].

### Key findings from the analysis:

- [cite\_start]**Accuracy**: For CBR traffic, the results showed a perfect match with theoretical values[cite: 434, 459]. [cite\_start]For Poisson traffic, the accuracy was over 99.7% of the theoretical values[cite: 422, 435].
- [cite\_start]**Performance**: The DRR algorithm ensured a fair bandwidth allocation, which was confirmed by the high fairness index values (approx. 0.877 for CBR and 0.900 for Poisson)[cite: 401, 426].
- [cite\_start]**Stability**: The system demonstrated stable and predictable service times (0.080 ms) and low waiting times (0.093-0.148 ms) and queue lengths[cite: 437, 438, 445, 446].

## 5\. Usage

To compile and run the simulator, a C++ compiler is required.

```bash
# Example compilation
g++ -std=c++17 main.cpp -o drr_simulator
# Example run
./drr_simulator
```

---

(Wersja polska)

# Projekt Symulatora Algorytmu Deficit Round Robin (DRR)

## 1\. Wprowadzenie

[cite\_start]Ten projekt to implementacja symulatora pojedynczego węzła sieciowego, który wykorzystuje algorytm **Deficit Round Robin (DRR)** do sprawiedliwego planowania pakietów[cite: 17, 19]. [cite\_start]Symulator został stworzony w celu badania i weryfikacji działania DRR w różnych warunkach obciążenia i przy różnych typach ruchu sieciowego[cite: 22]. [cite\_start]Implementacja powstała w języku C++[cite: 34].

## 2\. Cel i Zakres Projektu

[cite\_start]Głównym celem projektu było stworzenie działającego symulatora obsługującego dwa typy ruchu[cite: 24]:

- [cite\_start]**Ruch Poissonowski**: ruch o rozkładzie wykładniczym czasów międzyprzyjść[cite: 24, 46].
- [cite\_start]**Ruch CBR (Constant Bit Rate)**: ruch o stałej szybkości bitowej[cite: 24, 52].

[cite\_start]Symulator umożliwia parametryzację obu źródeł ruchu (np. intensywność, wielkość pakietu) oraz konfigurację algorytmu DRR[cite: 25]. [cite\_start]Po przeprowadzeniu symulacji generowane są kluczowe metryki wydajności, takie jak[cite: 26]:

- [cite\_start]Średnia liczba pakietów w kolejce[cite: 26].
- [cite\_start]Średni czas oczekiwania pakietów[cite: 26].
- [cite\_start]Średnie obciążenie serwera (wykorzystanie)[cite: 26].

## 3\. Implementacja

### Architektura systemu

[cite\_start]System został zaimplementowany w języku **C++** z wykorzystaniem nowoczesnych technik programowania, takich jak polimorfizm i inteligentne wskaźniki (`std::unique_ptr`)[cite: 34, 217]. Główne komponenty to:

- [cite\_start]**Hierarchia klas parametrów strumienia**: Klasy `CBRFlowParameters` i `PoissonFlowParameters` dziedziczą po klasie bazowej `FlowParameters`, co pozwala na elastyczną obsługę różnych typów ruchu[cite: 219, 220, 221].
- [cite\_start]**Model kolejkowy**: Kolejki są zaimplementowane przy użyciu `std::vector`, co zapewnia wydajne operacje FIFO (First-In, First-Out)[cite: 35, 237].
- [cite\_start]**Generator ruchu**: Wykorzystuje bibliotekę `std::random` z `std::mt19937` do generowania liczb pseudolosowych i `std::exponential_distribution` do modelowania ruchu Poissonowskiego[cite: 36, 37, 249, 250].

### Algorytm DRR

[cite\_start]Mechanizm planowania DRR działa w następujący sposób[cite: 117]:

1.  [cite\_start]**Inicjalizacja**: Liczniki deficytu dla każdej kolejki są ustawiane na 0[cite: 146].
2.  [cite\_start]**Obsługa**: W każdej rundzie do licznika deficytu dodawany jest stały kwant[cite: 113, 150]. [cite\_start]Pakiety są obsługiwane, dopóki deficyt jest wystarczający[cite: 151].
3.  [cite\_start]**Przetwarzanie pakietów**: Po obsłużeniu pakietu, deficyt jest zmniejszany o jego rozmiar[cite: 154].
4.  [cite\_start]**Zmiana kolejki**: Jeśli kolejka jest pusta lub pakiet jest zbyt duży, aby obsłużyć go w bieżącej rundzie, algorytm przechodzi do następnej kolejki, a niewykorzystany deficyt jest zachowywany na następną rundę[cite: 157, 158].

## 4\. Analiza wyników

[cite\_start]Przeprowadzono dwa główne scenariusze symulacyjne[cite: 382]:

1.  **Dwa strumienie CBR**:
    - [cite\_start]Strumień 1: 264 kbps (26,4% przepustowości łącza)[cite: 384].
    - [cite\_start]Strumień 2: 120 kbps (12,0% przepustowości łącza)[cite: 384].
2.  **Dwa strumienie Poisson**:
    - [cite\_start]Strumień 1: 320 kbps (32,0% przepustowości łącza)[cite: 386].
    - [cite\_start]Strumień 2: 160 kbps (16,0% przepustowości łącza)[cite: 386].

[cite\_start]W obu scenariuszach użyto stałego rozmiaru pakietu 80 bitów, prędkości łącza 1 Mbps, kwantu 12000 bitów i czasu symulacji 10000 sekund[cite: 382, 384, 386].

### Kluczowe wnioski z analizy:

- [cite\_start]**Dokładność**: Dla ruchu CBR uzyskano idealne dopasowanie wyników do wartości teoretycznych[cite: 434, 459]. [cite\_start]Dla ruchu Poissonowskiego dokładność wyniosła ponad 99,7% wartości teoretycznych[cite: 422, 435].
- [cite\_start]**Wydajność**: Algorytm DRR zapewnił sprawiedliwy podział przepustowości, co potwierdziły wysokie wartości wskaźnika sprawiedliwości (ok. 0,877 dla CBR i 0,900 dla Poisson)[cite: 401, 426].
- [cite\_start]**Stabilność**: System wykazał stabilne i przewidywalne czasy obsługi (0,080 ms) oraz kontrolowane, niskie czasy oczekiwania (0,093-0,148 ms) i długości kolejek[cite: 437, 438, 445, 446].

## 5\. Użycie

Aby skompilować i uruchomić symulator, należy użyć kompilatora C++.

```bash
# Przykład kompilacji
g++ -std=c++17 main.cpp -o drr_simulator
# Przykład uruchomienia
./drr_simulator
```
