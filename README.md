# Sales Research Agent

A production-style AI Sales Research Agent built with modern C++.

The system is designed to discover, research, qualify, and generate
evidence-based sales intelligence for potential customers.

## Current Status

Phase 1 - C++ project foundation.

## Technology

- C++20
- CMake
- PostgreSQL
- Redis
- Drogon
- React + TypeScript
- OpenAI-compatible LLM API
- Docker

## Project Goals

The final system will support:

- Company discovery
- Company research
- Technology and business signals
- Lead qualification
- ICP scoring
- Sales recommendations
- Source attribution
- Asynchronous research jobs
- Concurrent research workers
- RAG
- Observability
- Authentication and authorization
- Automated testing

## Build

```bash
cmake -S . -B build
cmake --build build