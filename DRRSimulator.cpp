#include "DRRSimulator.h"
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <iomanip>

const double LINK_SPEED = 1000000.0; // 1 Mbps
const double FlowParameters::PACKET_SIZE = 80.0; // 80 bitów = 10 bajtów


DRRSimulator::DRRSimulator(std::vector<std::unique_ptr<FlowParameters>>&& params, double quantumSize) 
    : flowParams(std::move(params)),
      quantum(quantumSize),
      simulationTime(0),
      totalBusyTime(0),
      lastEventTime(0),
      currentlyServingQueue(false),
      currentQueueIndex(0),
      gen(std::random_device{}()),
      nextServiceCompletionTime(std::numeric_limits<double>::max()),
      isServerBusy(false), 
      currentlyServingFlow(-1) 
{
    if (flowParams.empty() || quantumSize <= 0) {
        throw std::invalid_argument("Invalid parameters");
    }

    int numFlows = flowParams.size();
    queues.resize(numFlows);
    deficits.resize(numFlows, 0);
    totalPackets.resize(numFlows, 0);
    totalQueueTime.resize(numFlows, 0);
    totalServiceTime.resize(numFlows, 0);
    areaUnderQueue.resize(numFlows, 0);
    nextArrivalTimes.resize(numFlows);
    arrivalDists.resize(numFlows);
    totalBusyTimePerFlow.resize(numFlows, 0); 

    // Inicjalizacja czasów pierwszych nadejść
    for (int i = 0; i < numFlows; i++) {
        nextArrivalTimes[i] = simulationTime + generateInterarrivalTime(i);
    }
}

bool DRRSimulator::isCBRFlow(int flowIndex) const {
    return dynamic_cast<const CBRFlowParameters*>(flowParams[flowIndex].get()) != nullptr;
}

bool DRRSimulator::isPoissonFlow(int flowIndex) const {
    return dynamic_cast<const PoissonFlowParameters*>(flowParams[flowIndex].get()) != nullptr;
}

const CBRFlowParameters* DRRSimulator::getCBRParams(int flowIndex) const {
    return dynamic_cast<const CBRFlowParameters*>(flowParams[flowIndex].get());
}

const PoissonFlowParameters* DRRSimulator::getPoissonParams(int flowIndex) const {
    return dynamic_cast<const PoissonFlowParameters*>(flowParams[flowIndex].get());
}

double DRRSimulator::generateInterarrivalTime(int flowIndex) {
    if (isCBRFlow(flowIndex)) {
        return generateCBRArrivalTime(getCBRParams(flowIndex));
    } else {
        return generatePoissonArrivalTime(flowIndex);
    }
}

double DRRSimulator::generateServiceTime(int flowIndex) {
    if (isCBRFlow(flowIndex)) {
        return generateCBRServiceTime(getCBRParams(flowIndex));
    } else {
        return generatePoissonServiceTime(getPoissonParams(flowIndex));
    }
}

double DRRSimulator::generateCBRArrivalTime(const CBRFlowParameters* params) {
    if (!params) return 0.0;
    return FlowParameters::PACKET_SIZE / params->peakRate;
}

double DRRSimulator::generateCBRServiceTime(const CBRFlowParameters* params) {
    if (!params) return 0.0;
    return FlowParameters::PACKET_SIZE / LINK_SPEED;
}

double DRRSimulator::generatePoissonServiceTime(const PoissonFlowParameters* params) {
    if (!params) return 0.0;
    return FlowParameters::PACKET_SIZE / LINK_SPEED;
}

double DRRSimulator::generatePoissonArrivalTime(int flowIndex) {
    const auto* params = getPoissonParams(flowIndex);
    if (!params) return 0.0;
    
    // Obliczamy średni czas między pakietami (w sekundach)
    double meanInterarrivalTime = FlowParameters::PACKET_SIZE / params->meanBitRate;
    
    // Generujemy czas z rozkładu wykładniczego
    std::exponential_distribution<> dist(1.0 / meanInterarrivalTime);
    
    // Minimalny czas między pakietami (w sekundach)
    double minInterarrivalTime = (FlowParameters::PACKET_SIZE / LINK_SPEED) / 100.0;   // 100 razy mniejszy niż czas transmisji pakietu
    double generatedTime;
    do {
        generatedTime = dist(gen);
    } while (generatedTime < minInterarrivalTime);
    
    return generatedTime;
}

void DRRSimulator::updateQueueStats() {
    double interval = simulationTime - lastEventTime;
    if (interval > 0) {
        for (size_t i = 0; i < queues.size(); i++) {
            areaUnderQueue[i] += queues[i].size() * interval;
        }
        
        if (isServerBusy) {
            totalBusyTime += interval;
            // Usuwamy aktualizację totalBusyTimePerFlow stąd
        }
        
        lastEventTime = simulationTime;
    }
}

void DRRSimulator::processArrival(int flowIndex) {
    Packet packet{simulationTime, generateServiceTime(flowIndex)};
    queues[flowIndex].push_back(packet);
    nextArrivalTimes[flowIndex] = simulationTime + generateInterarrivalTime(flowIndex);
    
    if (queues[flowIndex].size() == 1) {
        scheduleNextService();
    }
}

void DRRSimulator::processService() {
    if (!queues[currentQueueIndex].empty()) {
        Packet& front = queues[currentQueueIndex].front();
        
        totalQueueTime[currentQueueIndex] += simulationTime - front.arrivalTime;
        totalServiceTime[currentQueueIndex] += front.serviceTime;
        totalBusyTimePerFlow[currentQueueIndex] += FlowParameters::PACKET_SIZE / LINK_SPEED;  // Dodajemy dokładny czas transmisji
        totalPackets[currentQueueIndex]++;
        
        deficits[currentQueueIndex] = std::max(0.0, deficits[currentQueueIndex] - FlowParameters::PACKET_SIZE);
        queues[currentQueueIndex].erase(queues[currentQueueIndex].begin());
    }
    
    isServerBusy = false;
    currentlyServingFlow = -1;
    scheduleNextService();
}

void DRRSimulator::scheduleNextService() {
    bool foundPacket = false;
    int startIndex = currentQueueIndex;
    
    do {
        currentQueueIndex = (currentQueueIndex + 1) % queues.size();
        
        if (!queues[currentQueueIndex].empty()) {
            if (deficits[currentQueueIndex] <= 0) {
                deficits[currentQueueIndex] += quantum;
            }
            
            if (deficits[currentQueueIndex] >= FlowParameters::PACKET_SIZE) {
                nextServiceCompletionTime = simulationTime + 
                    queues[currentQueueIndex].front().serviceTime;
                isServerBusy = true;
                currentlyServingFlow = currentQueueIndex;  // zapisujemy który strumień będzie obsługiwany
                foundPacket = true;
                break;
            }
        } else {
            deficits[currentQueueIndex] = 0;
        }
    } while (currentQueueIndex != startIndex);
    
    if (!foundPacket) {
        nextServiceCompletionTime = std::numeric_limits<double>::max();
        isServerBusy = false;
        currentlyServingFlow = -1;  // żaden strumień nie jest obsługiwany
    }
}

void DRRSimulator::runSimulation(double maxTime) {
    lastEventTime = simulationTime;
    
    while (simulationTime < maxTime) {
        updateQueueStats();
        
        double nextEvent = nextServiceCompletionTime;
        for (const auto& arrivalTime : nextArrivalTimes) {
            nextEvent = std::min(nextEvent, arrivalTime);
        }
        
        if (nextEvent > maxTime) break;
        
        simulationTime = nextEvent;
        
        for (int i = 0; i < nextArrivalTimes.size(); i++) {
            if (simulationTime == nextArrivalTimes[i]) {
                processArrival(i);
            }
        }
        
        if (simulationTime == nextServiceCompletionTime) {
            processService();
        }
    }
    
    updateQueueStats();
}

std::vector<double> DRRSimulator::calculateRho() const {
    std::vector<double> rhoValues;
    for (size_t i = 0; i < flowParams.size(); i++) {
        if (isCBRFlow(i)) {
            const auto* params = getCBRParams(i);
            double rho = params->peakRate / LINK_SPEED;
            rhoValues.push_back(rho);
        } else {
            const auto* params = getPoissonParams(i);
            double rho = params->meanBitRate / LINK_SPEED;
            rhoValues.push_back(rho);
        }
    }
    return rhoValues;
}

std::vector<double> DRRSimulator::calculateSimulatedRho() const {
    std::vector<double> simulatedRhoValues;
    for (size_t i = 0; i < flowParams.size(); i++) {
        if (isCBRFlow(i)) {
            double bitrate = (static_cast<double>(totalPackets[i]) * FlowParameters::PACKET_SIZE) / simulationTime;
            double rho = bitrate / LINK_SPEED;
            simulatedRhoValues.push_back(rho);
        } else {
            simulatedRhoValues.push_back(0.0); // Dla ruchu Poisson nie obliczamy rho
        }
    }
    return simulatedRhoValues;
}

double DRRSimulator::getFairnessIndex() const {
    std::vector<double> throughputs;
    for (size_t i = 0; i < flowParams.size(); i++) {
        double throughput = simulationTime > 0 ? 
            (static_cast<double>(totalPackets[i]) * FlowParameters::PACKET_SIZE) / simulationTime : 0;
            
        throughputs.push_back(throughput);
    }

    if (throughputs.empty()) return 0.0;

    double numerator = 0.0;
    double denominator = 0.0;
    
    for (double throughput : throughputs) {
        numerator += throughput;
        denominator += throughput * throughput;
    }
    
    numerator = numerator * numerator;
    denominator = throughputs.size() * denominator;
    
    return denominator > 0 ? numerator / denominator : 1.0;
}

void DRRSimulator::printResults(std::ostream& out) const {
    out << "\nWyniki symulacji DRR:\n";
    out << std::string(50, '-') << "\n\n";

    out << std::fixed << std::setprecision(3);
    out << "Stały rozmiar pakietu: " << FlowParameters::PACKET_SIZE << " bitów\n\n";

    std::vector<double> rhoValues = calculateRho();
    std::vector<double> simulatedRhoValues = calculateSimulatedRho();

    for (size_t i = 0; i < flowParams.size(); i++) {
        out << "Strumień " << i + 1 << ":\n";
        out << "  Typ ruchu: " << (isCBRFlow(i) ? "CBR" : "Poisson") << "\n";

        if (isCBRFlow(i)) {
            const auto* params = getCBRParams(i);
            out << "  Peak rate: " << params->peakRate << " bps\n";
        } else {
            const auto* params = getPoissonParams(i);
            out << "  Średnia szybkość: " << params->meanBitRate << " bps\n";
            // Oblicz teoretyczną intensywność napływu z średniej szybkości
            double meanArrivalRate = params->meanBitRate / FlowParameters::PACKET_SIZE;
            out << "  Teoretyczna intensywność napływu: " << meanArrivalRate << " pakietów/s\n";
            out << "  Teoretyczne rho: " << rhoValues[i] * 100 << "%\n";
        }
        double flowUtilization = (totalBusyTimePerFlow[i] / simulationTime) * 100;
        out << "  Wykorzystanie serwera: " << flowUtilization << "%\n";
        
        if (isCBRFlow(i)) {
            out << "  Teoretyczne rho: " << rhoValues[i] * 100 << "%\n";
        }

        double throughput = simulationTime > 0 ?
            static_cast<double>(totalPackets[i]) / simulationTime : 0;
        double bitrate = throughput * FlowParameters::PACKET_SIZE;

        out << "  Obsłużone pakiety: " << totalPackets[i] << "\n";
        if (totalPackets[i] > 0) {
            out << "  Średni czas oczekiwania: " << (totalQueueTime[i] / totalPackets[i]) * 1000 << " ms\n";
            out << "  Średni czas obsługi: " << (totalServiceTime[i] / totalPackets[i]) * 1000 << " ms\n";
        }
        out << "  Średnia długość kolejki: " << areaUnderQueue[i] / simulationTime << " pakietów\n";
        out << "  Przepustowość: " << throughput << " pakietów/s\n";
        out << "  Rzeczywista szybkość: " << bitrate << " bps\n\n";
    }

    out << "Statystyki systemu:\n";
    out << "  Czas symulacji: " << simulationTime << " s\n";
    double totalUtilization = 0.0;
    for (size_t i = 0; i < flowParams.size(); i++) {
        totalUtilization += (totalBusyTimePerFlow[i] / simulationTime) * 100;
    }
    out << "  Wykorzystanie serwera: " << totalUtilization << "%\n";
    out << "  Indeks fairness: " << getFairnessIndex() << "\n";

    out << "\nAktualne wartości deficytu:\n";
    for (size_t i = 0; i < deficits.size(); i++) {
        out << "  Strumień " << i + 1 << ": " << deficits[i] << " bitów\n";
    }

    out << std::string(50, '-') << "\n";
}
