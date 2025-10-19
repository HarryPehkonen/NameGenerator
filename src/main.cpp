#include "NameGenerator.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [count] [--debug]\n"
              << "  count   - Number of names to generate (default: 10)\n"
              << "  --debug - Show pattern used for each name\n"
              << "\n"
              << "Examples:\n"
              << "  " << programName << "              # Generate 10 names\n"
              << "  " << programName << " 20           # Generate 20 names\n"
              << "  " << programName << " 100          # Generate 100 names effortlessly!\n"
              << "  " << programName << " 10 --debug   # Generate 10 names with patterns\n"
              << "  " << programName << " --debug 20   # Same as above\n";
}

int main(int argc, char* argv[]) {
    size_t count = 10;  // Default count
    bool debug = false;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "--debug" || arg == "-d") {
            debug = true;
        } else {
            // Try to parse as count
            try {
                count = std::stoull(arg);
                if (count == 0) {
                    std::cerr << "Error: Count must be greater than 0\n";
                    return 1;
                }
                if (count > 10000) {
                    std::cerr << "Error: Count is too large (max 10000)\n";
                    return 1;
                }
            } catch (const std::exception&) {
                std::cerr << "Error: Invalid argument '" << arg << "'\n";
                printUsage(argv[0]);
                return 1;
            }
        }
    }

    // Create generator and generate names
    NameGenerator generator;

    if (debug) {
        // Generate with pattern information
        auto results = generator.generateWithPattern(count);

        // Print names with patterns
        for (const auto& result : results) {
            std::cout << result.name << " [" << result.pattern << "]\n";
        }
    } else {
        // Generate names only
        auto names = generator.generate(count);

        // Print all generated names
        for (const auto& name : names) {
            std::cout << name << '\n';
        }
    }

    return 0;
}
