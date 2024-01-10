#include "computationnode.h"

#include "fpgaaccess.h"

ComputationApi::ComputationApi(
        std::shared_ptr<Pistache::Http::Endpoint> _pEndpoint, std::shared_ptr<Pistache::Rest::Router> _pRouter)
    : m_httpEndpoint(_pEndpoint), m_router(_pRouter){};

void ComputationApi::setupRoutes()
{
    using namespace Pistache::Rest;

    Routes::Post(*m_router, m_base + "/computation", Routes::bind(&ComputationApi::computeRequestsHandler, this));
    Routes::Options(*m_router, m_base + "/computation", Routes::bind(&ComputationApi::optionsHandler, this));
}

void ComputationApi::optionsHandler(
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

void ComputationApi::computeRequestsHandler(
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

void ComputationApi::computationApiDefaultHandler(
        const Pistache::Rest::Request& _request, Pistache::Http::ResponseWriter _response)
{
    UNUSED(_request);

    // For CORS compatibility
    _response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");

    _response.send(Pistache::Http::Code::Not_Found, "The requested method does not exist (ComputationApi)");
}




ComputationApiImpl::ComputationApiImpl(
        std::shared_ptr<Pistache::Http::Endpoint> _pEndpoint, std::shared_ptr<Pistache::Rest::Router> _pRouter)
    : ComputationApi(_pEndpoint, _pRouter)
{
}

void ComputationApiImpl::compute_requests(const std::string& _content, Pistache::Http::ResponseWriter& _response)
{
    // Parse content

    std::cout << "Computing response" << std::endl;

    //std::string _content = "{\"a\":123,\"b\":45,\"c\":678}";
    std::string s1 = _content.substr(1, _content.size() - 2);
    auto po1 = s1.find_first_of(":");
    auto po2 = s1.find_first_of(",");
    std::string sa = s1.substr(po1 + 1, po2 - po1 - 1);
    std::string s2 = s1.substr(po2 + 1);
    po1 = s2.find_first_of(":");
    po2 = s2.find_first_of(",");
    std::string sb = s2.substr(po1 + 1, po2 - po1 - 1);
    std::string s3 = s2.substr(po2 + 1);
    po1 = s3.find_first_of(":");
    std::string sc = s3.substr(po1 + 1);

    auto a = atol(sa.data());
    auto b = atol(sb.data());
    auto c = atol(sc.data());

    uint32_t result = FPGAAccess::getInstance().compute(a, b, c);

    // Prepare response
    std::stringstream stream;
    stream << "{\"result\":" << result << "}";

    std::string response = stream.str();

    _response.send(Pistache::Http::Code::Ok, response);

}
