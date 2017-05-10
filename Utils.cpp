#include "Utils.h"

void generateNumbers(double **&arr, const int rows, const int columns) {
    if (arr != nullptr){
        for (int i = 0; i < rows; i++){
            delete[] arr[i];
        }
        delete[] arr;
    }
    arr = new double * [rows];
    for(int i = 0; i < rows; i++)
        arr[i] = new double[columns];

    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < columns; ++j){
            arr[i][j] =  double(rand()%100);
        }
    }

}

bool saveToFile(const std::string &filename, const double* const* arr, const int rows, const int columns) {
    std::ofstream fout;
    fout.open(filename, std::ios::out);
    if (fout.is_open()) {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < columns; ++j){
                fout << arr[i][j] << std::endl;
            }
        fout.close();
        return true;
    }
    
    return false;
}

bool readFromFile(const std::string &filename, double **&arr, int rows, int columns) {
    // clear vars
    if (arr != nullptr){
        for (int i = 0; i < rows; i++){
            delete[] arr[i];
        }
        delete[] arr;
    }
    arr = new double * [rows];
    for(int i = 0; i < rows; i++)
        arr[i] = new double[columns];

    // try to load file
    std::ifstream fin;
    fin.open(filename, std::ios::in);
    if (fin.is_open()) {
        int tmp_val;
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < columns; j++){
                if (fin >> tmp_val)
                    arr[i][j] = tmp_val;
                else
                    arr[i][j] = 0;
            }
        }
        fin.close();
        return true;
    }

    return false;
}
