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
    std::wstring wStr(tcharStr);
    std::string str(wStr.begin(), wStr.end());
    return str;
    #else
    return std::string(tcharStr);
    #endif
}

std::string openFileDialog() {
    OPENFILENAME ofn;
    TCHAR szFile[260];
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile) / sizeof(TCHAR);
    ofn.lpstrFilter = _T("All Files\0*.*\0Text Files\0*.TXT\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        return TCHARToString(szFile);
    } else {
        return "";
    }
}

int main() {

    vector<string> wheel;

    ifstream colorFile("colorset.txt");
    if (!colorFile.is_open()) {
        cerr << "Error: Unable to open colorset.txt" << endl;
        return 1;
    }

    string color;
    while (getline(colorFile, color)) {
        wheel.push_back(color);
    }
    colorFile.close();

    string inputFilePath = openFileDialog();
    if (inputFilePath.empty()) {
        cerr << "Error: No file selected" << endl;
        return 1;
    }

    ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open " << inputFilePath << endl;
        return 1;
    }

    string content;
    getline(inputFile, content);
    inputFile.close();

    istringstream iss(content);
    string part;

    getline(iss, part, ',');
    string song_id = part;

    vector<Note> notes;

    while (getline(iss, part, ',')) {
        istringstream noteStream(part);
        Note n;

        getline(noteStream, part, '|');
        n.row = stod(part);

        getline(noteStream, part, '|');
        n.column = stod(part);

        getline(noteStream, part);
        n.ms = stoi(part);

        notes.push_back(n);
    }

    if (notes.size() < 2) {
        cerr << "Error: Not enough notes to process" << endl;
        return 1;
    }

    string outputFileName = song_id + ".txt";
    ofstream outputFile(outputFileName);

    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open output file" << endl;
        return 1;
    }

    int colorIndex = 0;

    outputFile << wheel[colorIndex] << endl;

    for (size_t i = 1; i < notes.size(); i++) {
        int timeDiff = notes[i].ms - notes[i - 1].ms;
        double distance = sqrt(pow(notes[i].row - notes[i - 1].row, 2) + pow(notes[i].column - notes[i - 1].column, 2));

        if (notes[i].ms == notes[i - 1].ms) {
            outputFile << wheel[colorIndex] << endl;
        }

        else if (timeDiff < 60 && distance < 0.25) {
            outputFile << wheel[colorIndex] << endl;
        }

        else {
            colorIndex = (colorIndex + 1) % wheel.size();
            outputFile << wheel[colorIndex] << endl;
        }
    }

    outputFile.close();
    return 0;
}