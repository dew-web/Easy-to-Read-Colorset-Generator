#pragma comment(lib, "Comdlg32.lib")

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <windows.h>
#include <commdlg.h>
#include <tchar.h>

using namespace std;

struct Note {
    double row, column;
    int ms;
};

std::string TCHARToString(const TCHAR* tcharStr) {
    #ifdef UNICODE
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, tcharStr, -1, NULL, 0, NULL, NULL);
    std::string strTo(size_needed - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, tcharStr, -1, &strTo[0], size_needed, NULL, NULL);
    return strTo;
    #else
    return std::string(tcharStr);
    #endif
}

std::string openFileDialog(const TCHAR* filter = _T("Text Files\0*.txt\0All Files\0*.*\0")) {
    OPENFILENAME ofn;
    TCHAR szFile[MAX_PATH] = {0};
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        return TCHARToString(szFile);
    } else {
        return "";
    }
}

void showErrorMessage(const std::string& message) {
    #ifdef UNICODE
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, message.c_str(), -1, NULL, 0);
    std::wstring wMessage(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, message.c_str(), -1, &wMessage[0], size_needed);

    MessageBox(NULL, wMessage.c_str(), L"Error", MB_ICONERROR | MB_OK);
    #else
    MessageBox(NULL, message.c_str(), "Error", MB_ICONERROR | MB_OK);
    #endif
}

int main() {
    string colorFilePath = openFileDialog(_T("Text Files\0*.txt\0All Files\0*.*\0"));
    if (colorFilePath.empty()) {
        showErrorMessage("Color set file not selected or failed to open.");
        return 1;
    }

    vector<string> wheel;
    ifstream colorFile(colorFilePath);

    if (!colorFile.is_open()) {
        showErrorMessage("Failed to open the color set file.");
        return 1;
    }

    string color;
    while (getline(colorFile, color)) {
        if (!color.empty()) {
            wheel.push_back(color);
        }
    }
    colorFile.close();

    if (wheel.empty()) {
        showErrorMessage("Color set file is empty.");
        return 1;
    }

    string inputFilePath = openFileDialog(_T("Text Files\0*.txt\0All Files\0*.*\0"));
    if (inputFilePath.empty()) {
        showErrorMessage("Input file not selected or failed to open.");
        return 1;
    }

    ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        showErrorMessage("Failed to open the input file.");
        return 1;
    }

    string content;
    if (!getline(inputFile, content)) {
        showErrorMessage("Failed to read from the input file.");
        inputFile.close();
        return 1;
    }
    inputFile.close();

    if (content.empty()) {
        showErrorMessage("Input file is empty.");
        return 1;
    }

    istringstream iss(content);
    string part;
    if (!getline(iss, part, ',')) {
        showErrorMessage("Invalid format in input file.");
        return 1;
    }
    string song_id = part;

    vector<Note> notes;
    while (getline(iss, part, ',')) {
        istringstream noteStream(part);
        Note n;
        string value;

        if (!getline(noteStream, value, '|')) {
            showErrorMessage("Invalid note format in input file.");
            return 1;
        }
        try {
            n.row = stod(value);
        } catch (...) {
            showErrorMessage("Invalid row value in note.");
            return 1;
        }

        if (!getline(noteStream, value, '|')) {
            showErrorMessage("Invalid note format in input file.");
            return 1;
        }
        try {
            n.column = stod(value);
        } catch (...) {
            showErrorMessage("Invalid column value in note.");
            return 1;
        }

        if (!getline(noteStream, value)) {
            showErrorMessage("Invalid note format in input file.");
            return 1;
        }
        try {
            n.ms = stoi(value);
        } catch (...) {
            showErrorMessage("Invalid ms value in note.");
            return 1;
        }

        notes.push_back(n);
    }

    if (notes.empty()) {
        showErrorMessage("No notes found in the input file.");
        return 1;
    }

    string outputFileName = song_id + "_colorset.txt";
    ofstream outputFile(outputFileName);

    if (!outputFile.is_open()) {
        showErrorMessage("Failed to create the output file.");
        return 1;
    }

    int colorIndex = 0;
    outputFile << wheel[colorIndex] << endl;

    for (size_t i = 1; i < notes.size(); i++) {
        int timeDiff = notes[i].ms - notes[i - 1].ms;
        double distance = sqrt(pow(notes[i].row - notes[i - 1].row, 2) + pow(notes[i].column - notes[i - 1].column, 2));

        if (timeDiff == 0 || (timeDiff < 60 && distance < 0.2)) {
            outputFile << wheel[colorIndex] << endl;
        } else {
            colorIndex = (colorIndex + 1) % wheel.size();
            outputFile << wheel[colorIndex] << endl;
        }
    }

    outputFile.close();
    return 0;
}