#ifndef COMPUTATIONNODE_H
#define COMPUTATIONNODE_H

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/http_headers.h>
#include <pistache/router.h>


class ComputationApi
{
public:
    ComputationApi(
            std::shared_ptr<Pistache::Http::Endpoint> _pEndpoint, std::shared_ptr<Pistache::Rest::Router> _pRouter);
    virtual ~ComputationApi() {}

    void setupRoutes();

private:
    void computeRequestsHandler(const Pistache::Rest::Request& _request, Pistache::Http::ResponseWriter _response);
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

public:
    const std::string m_base = "/";

private:
    std::shared_ptr<Pistache::Http::Endpoint> m_httpEndpoint;
    std::shared_ptr<Pistache::Rest::Router> m_router;
};

class ComputationApiImpl : public ComputationApi
{
public:
    ComputationApiImpl(
            std::shared_ptr<Pistache::Http::Endpoint> _pEndpoint, std::shared_ptr<Pistache::Rest::Router> _pRouter);
    ~ComputationApiImpl() override {}

    void compute_requests(const std::string& _content, Pistache::Http::ResponseWriter& _response) override;
};


#endif // COMPUTATIONNODE_H
