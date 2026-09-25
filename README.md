# Sales Research Agent (SRA)

A modern, high-performance C++20 HTTP microservice designed for automated B2B sales research, web signal extraction, and Ideal Customer Profile (ICP) scoring powered by LLMs (Groq Cloud / OpenAI) and the Drogon Web Framework.

---

## Key Features

- **Asynchronous Architecture:** Built on top of non-blocking I/O using the **Drogon C++ Web Framework**.
- **LLM-Powered Research Engine:** Integrates directly with Groq Cloud (Llama 3.1) and OpenAI models for real-time extraction.
- **Web Search & Evidence Attribution:** Real-time web signal gathering with claim-level source citation.
- **Configurable ICP Scoring:** Modular scoring strategy for ranking prospective accounts.
- **Modern C++20 Standards:** Built using RAII, smart pointers, thread pools, and strong type safety.

---

## Project Structure

\\\
sales-research-agent/
+-- include/sra/             # Public C++ headers
¦   +-- app/                # Application bootstrap
¦   +-- config/             # Environment & configuration loader
¦   +-- controllers/        # Drogon HTTP REST controllers
¦   +-- domain/             # Core domain models (Company, Signal, Evidence)
¦   +-- research/           # Research services and LLM providers
+-- src/                    # Source implementation files
+-- tests/                  # Unit and integration tests (GTest/GMock)
+-- .env.example            # Environment configuration template
+-- CMakeLists.txt          # Root CMake build configuration
+-- main.cpp                # Application entrypoint
\\\

---

## Prerequisites

- **C++ Compiler:** MSVC 2022 / GCC 11+ / Clang 13+ with C++20 support
- **Build System:** CMake v3.20+
- **Framework:** Drogon C++ Framework & Trantor
- **Dependencies:** JsonCpp, OpenSSL

---

## Building and Running

### 1. Clone & Configure
\\\powershell
git clone https://github.com/maan034/sales-research-agent.git
cd sales-research-agent
cmake -B build -S .
\\\

### 2. Build Debug Binary
\\\powershell
cmake --build build --config Debug
\\\

### 3. Set Environment Variables
Create a \.env\ file in the project root:
\\\ini
APP_ENV=development
APP_NAME=sales-research-agent
APP_VERSION=0.1.0
GROQ_API_KEY=gsk_your_groq_api_key_here
\\\

### 4. Execute the Application
\\\powershell
.\build\Debug\sales_research_agent.exe
\\\

---

## API Reference

### Trigger Company Research
**Endpoint:** \POST /api/research\  
**Content-Type:** \pplication/json\

#### Request Body
\\\json
{
  "company": "Samsung"
}
\\\

#### Response Example
\\\json
{
  "company": "Samsung",
  "icpScore": 85,
  "status": "completed"
}
\\\

---

## Running Unit Tests

To run the complete test suite:

\\\powershell
ctest --test-dir build -C Debug --output-on-failure
\\\

---

## License

Distributed under the MIT License. See \LICENSE\ for details.
