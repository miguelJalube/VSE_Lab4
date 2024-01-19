#include "nbcomputenode.h"

#include "fpgaaccess.h"

NbComputeApi::NbComputeApi(
        std::shared_ptr<Pistache::Http::Endpoint> _pEndpoint, std::shared_ptr<Pistache::Rest::Router> _pRouter)
    : m_httpEndpoint(_pEndpoint), m_router(_pRouter){};

void NbComputeApi::setupRoutes()
{
    using namespace Pistache::Rest;

    Routes::Post(*m_router, m_base + "/nbcompute", Routes::bind(&NbComputeApi::computeRequestsHandler, this));
    Routes::Options(*m_router, m_base + "/nbcompute", Routes::bind(&NbComputeApi::optionsHandler, this));
    Routes::Post(*m_router, m_base + "/resetnbcompute", Routes::bind(&NbComputeApi::resetNbComputeHandler, this));
    Routes::Options(*m_router, m_base + "/resetnbcompute", Routes::bind(&NbComputeApi::optionsHandler, this));
}

void NbComputeApi::optionsHandler(
        const Pistache::Rest::Request& /*_request*/, Pistache::Http::ResponseWriter _response)
{

    // For CORS compatibility
    _response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
    _response.headers().add<Pistache::Http::Header::AccessControlAllowMethods>("POST,OPTIONS");
    _response.headers().add<Pistache::Http::Header::AccessControlAllowHeaders>("*");
    _response.headers().add<Pistache::Http::Header::Allow>(Pistache::Http::Method::Post);
    _response.headers().add<Pistache::Http::Header::Allow>(Pistache::Http::Method::Options);

    try {
        _response.send(Pistache::Http::Code::Ok, "");
    }
    catch (std::runtime_error& e) {
        //send a 400 error
        _response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    }
}

void NbComputeApi::computeRequestsHandler(
        const Pistache::Rest::Request& _request, Pistache::Http::ResponseWriter _response)
{

    // For CORS compatibility
    _response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");

    _response.headers().add<Pistache::Http::Header::ContentType>("application/xml");

    try {
        this->compute_requests(_request.body(), _response);
    }
    catch (std::runtime_error& e) {
        //send a 400 error
        _response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    }
}

void NbComputeApi::resetNbComputeHandler(const Pistache::Rest::Request &_request, Pistache::Http::ResponseWriter _response)
{

    // For CORS compatibility
    _response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");

    _response.headers().add<Pistache::Http::Header::ContentType>("application/xml");

    try {
        this->resetNbCcomputeRequest(_request.body(), _response);
    }
    catch (std::runtime_error& e) {
        //send a 400 error
        _response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    }
}

void NbComputeApi::computationApiDefaultHandler(
        const Pistache::Rest::Request& _request, Pistache::Http::ResponseWriter _response)
{
    UNUSED(_request);

    // For CORS compatibility
    _response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");

    _response.send(Pistache::Http::Code::Not_Found, "The requested method does not exist (ComputationApi)");
}




NbComputeApiImpl::NbComputeApiImpl(
        std::shared_ptr<Pistache::Http::Endpoint> _pEndpoint, std::shared_ptr<Pistache::Rest::Router> _pRouter)
    : NbComputeApi(_pEndpoint, _pRouter)
{
}

void NbComputeApiImpl::compute_requests(const std::string& _content, Pistache::Http::ResponseWriter& _response)
{
    // Parse content

    std::cout << "Request for NbCompute" << std::endl;

    uint32_t result = FPGAAccess::getInstance().getNbCompute();

    // Prepare response
    std::stringstream stream;
    stream << "{\"result\":" << result << "}";


    std::string response = stream.str();


    std::cout << "Response: " << response << std::endl;

    _response.send(Pistache::Http::Code::Ok, response);

}


void NbComputeApiImpl::resetNbCcomputeRequest(const std::string &_content, Pistache::Http::ResponseWriter &_response)
{
    // Parse content

    std::cout << "Reset NbCompute" << std::endl;

    FPGAAccess::getInstance().resetNbCompute();

    // Prepare response
    std::stringstream stream;
    stream << "{\"result\": \"ok\"}";

    std::string response = stream.str();

    _response.send(Pistache::Http::Code::Ok, response);

}
