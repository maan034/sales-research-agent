#include <drogon/drogon.h>
#include "sra/api/ResearchController.h"
#include <iostream>

int main() {
    std::cout << "Sales Research Agent starting HTTP Server..." << std::endl;

    auto researchController = std::make_shared<sra::api::ResearchController>();

    drogon::app().registerHandler(
        "/api/research",
        [researchController](const drogon::HttpRequestPtr& req,
                             std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
            researchController->researchCompany(req, std::move(callback));
        },
        {drogon::Post}
    );

    std::cout << "Server listening on http://127.0.0.1:8080" << std::endl;

    drogon::app()
        .addListener("127.0.0.1", 8080)
        .setThreadNum(16)
        .run();

    return 0;
}