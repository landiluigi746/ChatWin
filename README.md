# ChatWin

[![Build status](https://github.com/landiluigi746/ChatWin/workflows/Windows/badge.svg)](https://github.com/landiluigi746/ChatWin/actions/workflows/windows.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://github.com/landiluigi746/llib/LICENSE)
[![Language: C](https://img.shields.io/badge/Language-C-blue)](https://github.com/topics/c)

ChatWin is a simple client-server chat application for Windows, written in C using WinSock2 for the network part and PDCurses for the user interface.
Clients can connect to a server, enter their username and exchange messages with other clients connected to the same server.

## Usage

#### 1. Clone the repository
First, clone the repository: `git clone --recursive https://github.com/landiluigi746/ChatWin.git .\`.

#### 2. Build
Run `build.bat` to run premake5 to build solution and project files for Visual Studio 2022, or change action if you need to.
Open `ChatWin.sln` in Visual Studio 2022 and build the projects `Client` and `Server`.

#### 3. Run
Navigate to the folder where you compiled the projects, open a command prompt and run:
- `.\Server.exe [address] [port]` to start the server
- `.\Client.exe [address] [port]` to start the client