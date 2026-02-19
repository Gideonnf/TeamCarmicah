# TeamCarmicah
![Watch the trailer of Sugar Strike!](https://img.youtube.com/vi/i1VNaxWlPes/maxresdefault.jpg)
(https://www.youtube.com/watch?v=i1VNaxWlPes)


# Carmicah Engine                                                                         
Team: Carmicah
Engine Name: Carmicah Engine

------------------------------------------------------------------------------------------------------

Welcome to our Game Engine ReadMe, Sugar Strike is 2.5D game where you play as Princess Strawberry and must defened you and your kingdom from the calamity that is the invasion of the hungry animals! Defend the last stronghold of the kingdom and hold out till reinforcements arrive, strike down the wily mice, the dive-bombing pigeons and the tanky bears to surive!

------------------------------------------------------------------------------------------------------

## Team				
### Programmers
Gideon Nicholas FRANCIS (RTIS)
Role: Technical Co-Lead, Engine Architecture

Rainne WON (RTIS)
Role: Graphics Lead

LEE Yong Yee (RTIS)
Role: Audio Lead, Scripting

LAI Nicholas (IMGD)
Role: ImGui Lead

LIM Sheng Yao Micah (IMGD)
Role: Input Lead

YANG Yujie (IMGD)
Role: Project Manager

### Artists
SUNG Jernin (BFA)
Role: Art Lead (Characters)

Jia Xuan
Role: Artist (Environment)		
		
------------------------------------------------------------------------------------------------------

This file will contain the guide to the Carmicah Engine. Enjoy your time here(or not).
This document provides an overview of the setup and controls for efficient utilization of our game engine.

## Setup
The game engine solution was developed in Visual Studio 2022.
our engine consists of 3 projects:
- SliceEditor (Editor)
- SliceEngine (Engine)
- SliceScript (Scripting)

When first setting up the project, the external dependencies must be downloaded using a batch file. 

### Prerequisites: 
Ensure Premake is installed on your device, and that your Visual Studio has the latest C++ module installed. If not, you can download the Premake command-line-executable from Github (https://premake.github.io/).

Run the BAT file "PremakeProj.bat" by double-clicking it in "./SliceEngine/Slice/" to download the dependencies into the "./?/" folder. 
if the Visual Studio solution does not appear after running the BAT file the first time, run "PremakeProj.bat" again.

Please build the engine in Release.

When running the engine for the first time, the engine will take a while to start up as all the assets need to be compiled.

------------------------------------------------------------------------------------------------------
