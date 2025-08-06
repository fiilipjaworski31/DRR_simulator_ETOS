#ifndef __DRRSIMULATOR_H__
#define __DRRSIMULATOR_H__

#include <vector>
#include <random>
#include <memory>
#include <iostream>

struct Packet {
    double arrivalTime;    // Czas przybycia pakietu
    double serviceTime;    // Czas obsługi pakietu
};

// Klasa bazowa dla parametrów strumienia
class FlowParameters {
public:
    static const double PACKET_SIZE;  // Stały rozmiar pakietu dla wszystkich strumieni [bity]
    virtual ~FlowParameters() = default;
};

// Parametry strumienia CBR (Constant Bit Rate)
class CBRFlowParameters : public FlowParameters {
public:
    double peakRate;    // Szczytowa szybkość bitowa [bps]
    
    explicit CBRFlowParameters(double peak) 
        : peakRate(peak) {}
};

// Parametry strumienia Poisson
class PoissonFlowParameters : public FlowParameters {
public:
    double meanBitRate;     // Średnia szybkość bitowa [bps]
    
    explicit PoissonFlowParameters(double bitRate) 
        : meanBitRate(bitRate) {}
};

class DRRSimulator {
private:
    std::vector<std::unique_ptr<FlowParameters>> flowParams;
    std::vector<std::vector<Packet>> queues;       // Kolejki pakietów dla każdego strumienia
    std::vector<double> deficits;                  // Liczniki deficytu [bity]
    double quantum;                                // Quantum [bity]
	bool isServerBusy;
	int currentlyServingFlow;
	
    
    // Statystyki per strumień
    std::vector<long> totalPackets;               // Liczba obsłużonych pakietów
    std::vector<double> totalQueueTime;           // Całkowity czas oczekiwania
    std::vector<double> totalServiceTime;         // Całkowity czas obsługi
    std::vector<double> areaUnderQueue;           // Pole pod krzywą długości kolejki
    std::vector<double> totalBusyTimePerFlow;
    
    // Statystyki systemowe
    double simulationTime;                        // Aktualny czas symulacji
    double totalBusyTime;                         // Całkowity czas zajętości serwera
    double lastEventTime;                         // Czas ostatniego zdarzenia
    
    // Czasy zdarzeń
    std::vector<double> nextArrivalTimes;         // Czasy następnych nadejść
    double nextServiceCompletionTime;             // Czas zakończenia bieżącej obsługi
    bool currentlyServingQueue;                   // Czy serwer jest zajęty
    int currentQueueIndex;                        // Indeks aktualnie obsługiwanej kolejki

    // Generator liczb losowych
    std::mt19937 gen;
    std::vector<std::exponential_distribution<>> arrivalDists;

    // Metody pomocnicze
    double generateInterarrivalTime(int flowIndex);
    double generateServiceTime(int flowIndex);
    double generateCBRArrivalTime(const CBRFlowParameters* params);
    double generateCBRServiceTime(const CBRFlowParameters* params);
    double generatePoissonArrivalTime(int flowIndex);
    double generatePoissonServiceTime(const PoissonFlowParameters* params);
    
    void updateQueueStats();
    void processArrival(int flowIndex);
    void processService();
    void scheduleNextService();
    bool isCBRFlow(int flowIndex) const;
    bool isPoissonFlow(int flowIndex) const;
    const CBRFlowParameters* getCBRParams(int flowIndex) const;
    const PoissonFlowParameters* getPoissonParams(int flowIndex) const;

public:
    DRRSimulator(std::vector<std::unique_ptr<FlowParameters>>&& params, double quantumSize);
    void runSimulation(double maxTime);
    void printResults(std::ostream& out) const;
    double getFairnessIndex() const;
    std::vector<double> calculateRho() const;
    std::vector<double> calculateSimulatedRho() const;
};

#endif // __DRRSIMULATOR_H__
