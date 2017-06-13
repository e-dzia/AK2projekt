#pragma once

#include <string>
#include <fstream>

void generateNumbers(double **&arr, const int rows, const int columns);
bool saveToFile(const std::string &filename, const double *const* arr, const int rows, const int columns);
bool readFromFile(const std::string &filename, double **&arr, int rows, int columns);