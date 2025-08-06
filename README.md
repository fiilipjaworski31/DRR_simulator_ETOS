(English version/ polish below)

# Deficit Round Robin (DRR) Algorithm Simulator

## 1\. Introduction

This project implements a simulator for a single network node that uses the **Deficit Round Robin (DRR)** algorithm for fair packet scheduling. The simulator was created to study and verify the operation of DRR under various load conditions and with different types of network traffic. The implementation is written in C++.

## 2\. Project Goal and Scope

The main goal of the project was to create a functional simulator that handles two types of traffic:

- **Poisson traffic**: traffic with an exponential distribution of inter-arrival times.
- **CBR (Constant Bit Rate) traffic**: traffic with a constant bit rate.

The simulator allows for the parameterization of both traffic sources (e.g., intensity, packet size) and the configuration of the DRR algorithm. After the simulation, key performance metrics are generated, such as:

- Average number of packets in the queue.
- Average packet waiting time.
- Average server load (utilization).

## 3\. Implementation

### System Architecture

The system is implemented in **C++** using modern programming techniques, such as polymorphism and smart pointers (`std::unique_ptr`). The main components are:

- **Stream parameter class hierarchy**: `CBRFlowParameters` and `PoissonFlowParameters` classes inherit from the base class `FlowParameters`, allowing for flexible handling of different traffic types.
- **Queuing model**: Queues are implemented using `std::vector`, which ensures efficient FIFO (First-In, First-Out) operations.
- **Traffic generator**: It uses the `std::random` library with `std::mt19937` for generating pseudo-random numbers and `std::exponential_distribution` for modeling Poisson traffic.

### DRR Algorithm

The DRR scheduling mechanism works as follows:

1. **Initialization**: The deficit counters for each queue are set to 0.
2. **Serving**: In each round, a fixed quantum is added to the deficit counter. Packets are served as long as the deficit is sufficient.
3. **Packet processing**: After a packet is served, the deficit is reduced by its size.
4. **Queue switching**: If a queue is empty or a packet is too large to be served in the current round, the algorithm moves to the next queue, and the unused deficit is saved for the next round.

## 4\. Results Analysis

Two main simulation scenarios were conducted:

1. **Two CBR streams**:
   - Stream 1: 264 kbps (26.4% of link bandwidth).
   - Stream 2: 120 kbps (12.0% of link bandwidth).
2. **Two Poisson streams**:
   - Stream 1: 320 kbps (32.0% of link bandwidth).
   - Stream 2: 160 kbps (16.0% of link bandwidth).

In both scenarios, a constant packet size of 80 bits, a link speed of 1 Mbps, a quantum of 12000 bits, and a simulation time of 10000 seconds were used.

### Key findings from the analysis:

- **Accuracy**: For CBR traffic, the results showed a perfect match with theoretical values. For Poisson traffic, the accuracy was over 99.7% of the theoretical values.
- **Performance**: The DRR algorithm ensured a fair bandwidth allocation, which was confirmed by the high fairness index values (approx. 0.877 for CBR and 0.900 for Poisson).
- **Stability**: The system demonstrated stable and predictable service times (0.080 ms) and low waiting times (0.093-0.148 ms) and queue lengths.

## 5\. Usage

To compile and run the simulator, a C++ compiler is required.

````bash
# Example compilation
g++ -std=c++17 main.cpp -o drr_simulator
# Example run
./drr_simulator

(Wersja polska)

# Projekt Symulatora Algorytmu Deficit Round Robin (DRR)

## 1\. Wprowadzenie

Ten projekt to implementacja symulatora pojedynczego węzła sieciowego, który wykorzystuje algorytm **Deficit Round Robin (DRR)** do sprawiedliwego planowania pakietów. Symulator został stworzony w celu badania i weryfikacji działania DRR w różnych warunkach obciążenia i przy różnych typach ruchu sieciowego. Implementacja powstała w języku C++.

## 2\. Cel i Zakres Projektu

Głównym celem projektu było stworzenie działającego symulatora obsługującego dwa typy ruchu:

- **Ruch Poissonowski**: ruch o rozkładzie wykładniczym czasów międzyprzyjść.
- **Ruch CBR (Constant Bit Rate)**: ruch o stałej szybkości bitowej.

Symulator umożliwia parametryzację obu źródeł ruchu (np. intensywność, wielkość pakietu) oraz konfigurację algorytmu DRR. Po przeprowadzeniu symulacji generowane są kluczowe metryki wydajności, takie jak:

- Średnia liczba pakietów w kolejce.
- Średni czas oczekiwania pakietów.
- Średnie obciążenie serwera (wykorzystanie).

## 3\. Implementacja

### Architektura systemu

System został zaimplementowany w języku **C++** z wykorzystaniem nowoczesnych technik programowania, takich jak polimorfizm i inteligentne wskaźniki (`std::unique_ptr`). Główne komponenty to:

- **Hierarchia klas parametrów strumienia**: Klasy `CBRFlowParameters` i `PoissonFlowParameters` dziedziczą po klasie bazowej `FlowParameters`, co pozwala na elastyczną obsługę różnych typów ruchu.
- **Model kolejkowy**: Kolejki są zaimplementowane przy użyciu `std::vector`, co zapewnia wydajne operacje FIFO (First-In, First-Out).
- **Generator ruchu**: Wykorzystuje bibliotekę `std::random` z `std::mt19937` do generowania liczb pseudolosowych i `std::exponential_distribution` do modelowania ruchu Poissonowskiego.

### Algorytm DRR

Mechanizm planowania DRR działa w następujący sposób:

1. **Inicjalizacja**: Liczniki deficytu dla każdej kolejki są ustawiane na 0.
2. **Obsługa**: W każdej rundzie do licznika deficytu dodawany jest stały kwant. Pakiety są obsługiwane, dopóki deficyt jest wystarczający.
3. **Przetwarzanie pakietów**: Po obsłużeniu pakietu, deficyt jest zmniejszany o jego rozmiar.
4. **Zmiana kolejki**: Jeśli kolejka jest pusta lub pakiet jest zbyt duży, aby obsłużyć go w bieżącej rundzie, algorytm przechodzi do następnej kolejki, a niewykorzystany deficyt jest zachowywany na następną rundę.

## 4\. Analiza wyników

Przeprowadzono dwa główne scenariusze symulacyjne:

1. **Dwa strumienie CBR**:
    - Strumień 1: 264 kbps (26,4% przepustowości łącza).
    - Strumień 2: 120 kbps (12,0% przepustowości łącza).
2. **Dwa strumienie Poisson**:
    - Strumień 1: 320 kbps (32,0% przepustowości łącza).
    - Strumień 2: 160 kbps (16,0% przepustowości łącza).

W obu scenariuszach użyto stałego rozmiaru pakietu 80 bitów, prędkości łącza 1 Mbps, kwantu 12000 bitów i czasu symulacji 10000 sekund.

### Kluczowe wnioski z analizy:

- **Dokładność**: Dla ruchu CBR uzyskano idealne dopasowanie wyników do wartości teoretycznych. Dla ruchu Poissonowskiego dokładność wyniosła ponad 99,7% wartości teoretycznych.
- **Wydajność**: Algorytm DRR zapewnił sprawiedliwy podział przepustowości, co potwierdziły wysokie wartości wskaźnika sprawiedliwości (ok. 0,877 dla CBR i 0,900 dla Poisson).
- **Stabilność**: System wykazał stabilne i przewidywalne czasy obsługi (0,080 ms) oraz kontrolowane, niskie czasy oczekiwania (0,093-0,148 ms) i długości kolejek.

## 5\. Użycie

Aby skompilować i uruchomić symulator, należy użyć kompilatora C++.

```bash
# Przykład kompilacji
g++ -std=c++17 main.cpp -o drr_simulator
# Przykład uruchomienia
./drr_simulator
````
