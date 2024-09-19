#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

const double PI  = 3.141592653589793238463;
const double PI2 = 2. * PI;

// Function to pack X-coordinate (14 bits) and minutiae type (2 bits)
int packX(int x, int type) {
    return (type << 14) | (x & 0x3fff);
}

// Function to pack Y-coordinate (14 bits)
int packY(int y) {
    return y & 0x3fff;
}

// Function to pack angle (8 bits)
unsigned char packAngle(double angle) {
    return static_cast<unsigned char>(round(angle * 256.0 / PI2));
}

void convertTxtToDat(const string& txtFile, const string& datFile) {
    ifstream inFile(txtFile);
    ofstream outFile(datFile, ios::binary);

    if (!inFile || !outFile) {
        cerr << "Error opening file." << endl;
        return;
    }

    int width, height, numMinutiae;
    inFile >> width >> height >> numMinutiae;

    // Write header (adapt according to Neurotechnology's format)
    outFile.put(0); // Placeholder for the first value (might be reserved)
    outFile.put(width >> 8); outFile.put(width & 0xFF);   // Write width (2 bytes)
    outFile.put(height >> 8); outFile.put(height & 0xFF); // Write height (2 bytes)
    outFile.put(numMinutiae);                             // Number of minutiae (1 byte)

    for (int i = 0; i < numMinutiae; ++i) {
        int x, y;
        double angle;
        inFile >> x >> y >> angle;

        // Pack minutiae data in binary format
        int packedX = packX(x, 0); // Assuming type = 0 for now
        int packedY = packY(y);
        unsigned char packedAngle = packAngle(angle);

        // Write X-coordinate (2 bytes)
        outFile.put(packedX >> 8); outFile.put(packedX & 0xFF);

        // Write Y-coordinate (2 bytes)
        outFile.put(packedY >> 8); outFile.put(packedY & 0xFF);

        // Write Angle (1 byte)
        outFile.put(packedAngle);

        // Write additional fields (e.g., quality, type - placeholders for now)
        outFile.put(0); // Placeholder for minutiae quality
    }

    inFile.close();
    outFile.close();
    cout << "Conversion complete. Output written to " << datFile << endl;
}

int main() {
    string txtFile = "fmd_iso_output.txt";  // Input file (txt)
    string datFile = "output_fmd.dat";      // Output file (dat)

    convertTxtToDat(txtFile, datFile);

    return 0;
}
