#include <iostream>

#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"

#include "computationnode.h"

#include "fpgaaccess.h"

using namespace std;
using namespace Pistache;

void init(Http::Endpoint& _pEndpoint, size_t _thr = 2, uint16_t _maxRequestSize = 20000)
{
    auto opts = Pistache::Http::Endpoint::options().threads(static_cast<int>(_thr)).maxRequestSize(_maxRequestSize);
    _pEndpoint.init(opts);
}

void start(Http::Endpoint& _pEndpoint, Rest::Router& _router)
{
    _pEndpoint.setHandler(_router.handler());
    try {
        _pEndpoint.serve();
    }
    catch (std::runtime_error& e) {
        std::cout << "Server error: " << e.what() << std::endl;
        exit(1);
    }
}

void shutdown(Http::Endpoint& _pEndpoint)
{
    _pEndpoint.shutdown();
}



int main(int /*_argc*/, char** /*_argv*/)
{

    FPGAAccess fpga;
    fpga.init();

    uint16_t port = 12345;
    Address addr(Ipv4::any(), Port(port));

    shared_ptr<Rest::Router> pRouter = make_shared<Rest::Router>();
    shared_ptr<Http::Endpoint> pEndpoint = make_shared<Http::Endpoint>(addr);
    init(*pEndpoint, 1, 10000);


    ComputationApiImpl computationServer(pEndpoint, pRouter);
    computationServer.setupRoutes();



    std::cout << "Waiting on port : " << addr.port().toString() << std::endl;

    start(*pEndpoint, *pRouter);

    shutdown(*pEndpoint);

    return 0;
}
