#include "DRRSimulator.h"
#include <iostream>
#include <memory>
#include <fstream>

int main() {
    try {
        // ---- Scenariusz 1: Dwa strumienie CBR ----
        std::vector<std::unique_ptr<FlowParameters>> cbrParams;
        
        // CBR Strumień 1 - Wyższa szybkość
        cbrParams.push_back(std::make_unique<CBRFlowParameters>(
            264000     // Peak rate: 264 kbps
        ));
        
        // CBR Strumień 2 - Niższa szybkość
        cbrParams.push_back(std::make_unique<CBRFlowParameters>(
            120000     // Peak rate: 120 kbps
        ));
        
        std::cout << "\nUruchamianie symulacji dla ruchu CBR z dwoma strumieniami..." << std::endl;
        DRRSimulator cbrSim(std::move(cbrParams), 12000); // Quantum: 120000 bitów (1500 bajtów)
        cbrSim.runSimulation(10000.0); // Czas symulacji: 10000 sekund
        
        // Zapis wyników do pliku i na ekran
        std::ofstream cbrOutFile("cbr_results.txt");
        if (cbrOutFile.is_open()) {
            cbrSim.printResults(cbrOutFile);
            cbrOutFile.close();
            std::cout << "Wyniki zapisano do pliku: cbr_results.txt" << std::endl;
        }
        cbrSim.printResults(std::cout);

        // ---- Scenariusz 2: Dwa strumienie Poisson ----
        std::vector<std::unique_ptr<FlowParameters>> poissonParams;
        
        // Poisson Strumień 1 - Wyższa szybkość
        poissonParams.push_back(std::make_unique<PoissonFlowParameters>(
            320000     // Średnia szybkość: 320 kbps
        ));
        // Poisson Strumień 2 - Niższa szybkość
        poissonParams.push_back(std::make_unique<PoissonFlowParameters>(
            160000     // Średnia szybkość: 160 kbps
        ));
        
		std::cout << "\nUruchamianie symulacji dla ruchu Poisson z dwoma strumieniami..." << std::endl;
        DRRSimulator poissonSim(std::move(poissonParams), 12000); // Quantum: 1500 bajtów = 12000 bitów
        poissonSim.runSimulation(10000.0); // Czas symulacji: 10000 sekund
          
        // Zapis wyników do pliku i na ekran
        std::ofstream poissonOutFile("poisson_results.txt");
        if (poissonOutFile.is_open()) {
            poissonSim.printResults(poissonOutFile);
            poissonOutFile.close();
            std::cout << "Wyniki zapisano do pliku: poisson_results.txt" << std::endl;
        }
        poissonSim.printResults(std::cout);

    } catch (const std::exception& e) {
        std::cerr << "Błąd: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
