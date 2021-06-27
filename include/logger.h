//
//  logger.h
//  OpenGL
//
//  Created by Apple on 27/06/21.
//

#ifndef logger_h
#define logger_h

#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

static const std::string logFileName = "logger.txt";
static std::ofstream logFile;

static void LOGGER(std::string message){
    logFile.open(logFileName, std::ios::out | std::ios::app);
    if(logFile.is_open()){
        auto end = std::chrono::system_clock::now();
        std::time_t endTime = std::chrono::system_clock::to_time_t(end);
        std::string dateTime = std::string(std::ctime(&endTime));
        logFile << message << ": " <<dateTime;
        logFile.close();
    }else{
        std::cout << "Unable to open a file " << logFileName << std::endl;
    }
}

#endif /* logger_h */
