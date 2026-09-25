# Contributing to Sales Research Agent

Thank you for considering contributing to the Sales Research Agent!

## How to Contribute

1. Fork the repository.
2. Create a new topic branch (\git checkout -b feature/new-feature\).
3. Ensure the project builds using \cmake --build build --config Debug\.
4. Commit your changes with descriptive messages.
5. Push to your branch and open a Pull Request.

## Coding Style

- Follow modern C++20 conventions.
- Use explicit types, smart pointers (\std::unique_ptr\, \std::shared_ptr\), and RAII.
- Keep network and business logic modular and separate from HTTP controllers.
