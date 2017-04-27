#include <iostream>

#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "aldente.h"
#include "aldente_client.h"


int main(int argc, const char *argv[]) {
    Aldente *aldente;

    if (argc > 1) {
        // Grab first (non-program) arg and shift the rest
        const std::string selection = argv[1];
        argv[1] = argv[0];
        argv++;
        argc--;

        if (selection == "test") {
            exit(Catch::Session().run(argc, argv));
        } else if (selection == "server") {
            std::cerr << "Server not implemented yet." << std::endl;
            exit(EXIT_FAILURE);
        } else {
            std::cerr << "Invalid argument." << std::endl;
            exit(EXIT_FAILURE);
        }
    } else {
        aldente = new AldenteClient;
    }

    aldente->start();
    exit(EXIT_SUCCESS);
}
