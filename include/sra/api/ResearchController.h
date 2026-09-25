#pragma once

#include <drogon/HttpController.h>

namespace sra::api {

class ResearchController : public drogon::HttpController<ResearchController, false> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(ResearchController::researchCompany, "/api/research", drogon::Post);
    METHOD_LIST_END

    void researchCompany(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
    );
};

} // namespace sra::api