# Asteroid [![GitHub license](https://img.shields.io/github/license/OriolCS2/Asteroid)](https://github.com/OriolCS2/Asteroid/blob/main/LICENSE)


Asteroid is a profiler tool that allows you to profile C++ applications using its API inside the source code.

## Main Goal
The main goal is to visualizate the performance of a C++ application in order to detect code issues.

## Author
This project is made by [Oriol Capdevila Salinas](https://www.linkedin.com/in/oriol-capdevila-0a6b3914b/) ([GitHub](https://github.com/OriolCS2)) and belongs to my final thesis.

GitHub Repository: https://github.com/OriolCS2/Asteroid

## Usage

1. Copy 'Asteroid.h' and 'Asteroid.cpp' files from the repository or [latest release](https://github.com/OriolCS2/Asteroid/releases/tag/1.0) to your game project
2. Add ```ASTEROID_INIT();``` macro at the init of the application
  ```cpp
  void main() 
  {
      ASTEROID_INIT();
      ...
  } 
  ```
3. Add ```ASTEROID_FRAME();``` macro to the main loop of your application
  ```cpp
  while(true) 
  {
      ASTEROID_FRAME();
      application.Update();
  } 
  ```
4. In order to instrument a function, add one of the following macros:
    * Use ```ASTEROID_FUNCTION();``` at the beginning of a function to intrument it with default values
      ```cpp
      void SomeFunction() 
      {
          ASTEROID_FUNCTION();
          ...
      } 
      ```
    * Use ```ASTEROID_FUNCTION_EX("My Function");``` at the beginning of a function to intrument it with a given name
      ```cpp
      void SomeFunction() 
      {
          ASTEROID_FUNCTION_EX("My Function");
          ...
      } 
      ```
    * Use ```ASTEROID_CATEGORY("My Category", AsteroidColor::ORANGE);``` at the beginning of a function to intrument it with a given name and color
      ```cpp
      void SomeFunction() 
      {
          ASTEROID_CATEGORY("My Category", AsteroidColor::ORANGE);
          ...
      } 
      ```
5. Add ```ASTEROID_CLEANUP();``` at the end of the application
  ```cpp
  void main() 
  {
      ...
      ASTEROID_CLEANUP();
  } 
  ```
6. Open Asteroid application
7. Open the application you want to test
8. Click the play button on the Asteroid interface
![image](https://user-images.githubusercontent.com/36154523/116930797-839eea80-ac60-11eb-9573-e79b05f1c1d1.png)
9. Once you think you have enough frames collected, click the pause button
![image](https://user-images.githubusercontent.com/36154523/116930926-adf0a800-ac60-11eb-9b12-e3982a49d1a4.png)
10. Examine the frames
![image](https://user-images.githubusercontent.com/36154523/116931019-ca8ce000-ac60-11eb-85af-6781d8c95ac4.png)

## MIT License

Copyright (c) 2021 Oriol Capdevila

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
