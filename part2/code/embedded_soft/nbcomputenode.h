#ifndef NBCOMPUTENODE_H
#define NBCOMPUTENODE_H


#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/http_headers.h>
#include <pistache/router.h>


class NbComputeApi
{
public:
    NbComputeApi(
            std::shared_ptr<Pistache::Http::Endpoint> _pEndpoint, std::shared_ptr<Pistache::Rest::Router> _pRouter);
    virtual ~NbComputeApi() {}

    void setupRoutes();

private:
    void computeRequestsHandler(const Pistache::Rest::Request& _request, Pistache::Http::ResponseWriter _response);
    void resetNbComputeHandler(const Pistache::Rest::Request& _request, Pistache::Http::ResponseWriter _response);
    void computationApiDefaultHandler(
            const Pistache::Rest::Request& _request, Pistache::Http::ResponseWriter _response);
    void optionsHandler(const Pistache::Rest::Request& _request, Pistache::Http::ResponseWriter _response);

    /// <summary>
    /// Send requests to process
    /// </summary>
    /// <remarks>
    /// Send a list of requests in XML format following the query.xsd definition.
    /// </remarks>
    /// <param name="body">Query object containing the list of requests</param>
    virtual void compute_requests(const std::string& _content, Pistache::Http::ResponseWriter& _response) = 0;

    virtual void resetNbCcomputeRequest(const std::string& _content, Pistache::Http::ResponseWriter& _response) = 0;

public:
    const std::string m_base = "/";

private:
    std::shared_ptr<Pistache::Http::Endpoint> m_httpEndpoint;
    std::shared_ptr<Pistache::Rest::Router> m_router;
};

class NbComputeApiImpl : public NbComputeApi
{
public:
    NbComputeApiImpl(
            std::shared_ptr<Pistache::Http::Endpoint> _pEndpoint, std::shared_ptr<Pistache::Rest::Router> _pRouter);
    ~NbComputeApiImpl() override {}

    void compute_requests(const std::string& _content, Pistache::Http::ResponseWriter& _response) override;
    void resetNbCcomputeRequest(const std::string& _content, Pistache::Http::ResponseWriter& _response) override;
};


#endif // NBCOMPUTENODE_H
