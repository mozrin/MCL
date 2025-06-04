# MLC - A Simple Interpreted Language

## Introduction

MCL is a small, custom-built interpreted language implemented in C++. This project serves as a foundational exploration into the core components of an interpreted language, from raw source code to executed output. The current release development effort is called nirvana.

## Motivation

The primary motivation behind this project is to provide a hands-on, advanced learning exercise to keep C++ skills sharp and deepen understanding of the construction of programming languages. This is not intended as a production-ready language, but rather a deliberate and focused educational endeavor. All contributions, ideas, and questions are welcome.

## AI Collaboration

This project extensively leverages advanced AI models in its development process. I use, almost exclusively, pre-release versions of Google's Gemini LLMs via the [Google AI Studio](https://aistudio.google.com/).

The `prep_ai` folder within the project structure contains prompt and problem files. These are used to define the core objectives of the project (in `prompt` files) and the current work focus (in `problem` files) respectively. I engage in a continuous feedback loop with the AI, reviewing, massaging, and cooperatively defining this project one step at a time. This approach allows me to dive into complex topics like interpreted computer languages and rapidly bring myself up to speed with a tool like C++ (which I haven't used commercially in over a decade) without the initial struggle. This process works uniquely for my brain, and while it may or may not work for yours, you are absolutely welcome to try it!

A significant advantage of using Google Gemini LLMs for this type of development is their ability to process many megabytes of data. This allows for comprehensive context sharing with the AI. Furthermore, the capability to adjust the "thinking" cost of the models enables fine-tuning for accuracy and depth in results – just look for one of their "thinking" LLMs!

## Development Environment

This project, along with all my other development endeavors, is proudly built and maintained within the streamlined workflow provided by the **DevLite Devcontainer**.

DevLite offers a lightweight, standardized, and portable development environment using Docker containers, ensuring consistent toolchains and dependencies across different machines. This significantly simplifies setup and collaboration.

You can learn more about DevLite Devcontainers and how to integrate them into your own projects here:

[http://github.com/moztopia/devlite](http://github.com/moztopia/devlite)

## Current Features

I have decided to delete this section for now ... it is evolving too rapidly to keep up with the changes here.

## How to Run

To compile and run MCL, navigate to the project's root directory (e.g., `/nirvana/code/`) in your terminal and use a C++17 compatible compiler (like g++):

```bash
g++ -std=c++17 -g src/*.cpp -Iinclude -o bin/nirvana
```

Then, execute the interpreter with the example script:

```bash
./bin/nirvana bin/main.nv
```

## Contributing

You are always welcome to ask anything about this project. If I can tell you the answer or help you, I will. Feel free to explore, modify, and extend this project. I would appreciate at the very least a fork if you plan to build upon it!
