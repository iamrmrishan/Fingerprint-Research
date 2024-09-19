#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <string>

using namespace std;

// Function to add Gaussian noise to the angle (8-bit value)
unsigned char add_noise_to_angle(unsigned char angle, double noise_percentage)
{
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> noise(0, noise_percentage * angle);
    return static_cast<unsigned char>(angle + static_cast<int>(noise(gen)));
}

void addNoiseToAngleInDat(const string &inputDatFile, const string &outputDatFile, double noise_percentage)
{
    ifstream inFile(inputDatFile, ios::binary);
    ofstream outFile(outputDatFile, ios::binary);

    if (!inFile || !outFile)
    {
        cerr << "Error opening file." << endl;
        return;
    }

    // Copy header (assuming header size of 16 bytes, adjust as necessary)
    char header[16];
    inFile.read(header, sizeof(header));
    outFile.write(header, sizeof(header));

    // Read and modify minutiae data
    while (inFile)
    {
        unsigned char buffer[6]; // Each minutia is 6 bytes (X: 2 bytes, Y: 2 bytes, Angle: 1 byte, Type/Quality: 1 byte)
        inFile.read(reinterpret_cast<char *>(buffer), sizeof(buffer));

        if (!inFile)
            break; // End of file reached

        // Add noise to angle (5th byte in the buffer)
        buffer[4] = add_noise_to_angle(buffer[4], noise_percentage);

        // Write the modified buffer (with noise in angle only) to the output file
        outFile.write(reinterpret_cast<char *>(buffer), sizeof(buffer));
    }

    inFile.close();
    outFile.close();
    cout << "Noise added to Angle at " << noise_percentage * 100 << "% and saved to " << outputDatFile << endl;
}

int main()
{
    string inputDatFile = "fmd.dat";  // Input .dat file

    // Apply noise percentages: 1%, 2%, 5%, 10%, 20%, 30%, 40%, 50%, 60%, 70%, 80%, 90%, 100%
    double noise_percentages[] = {0.01, 0.02, 0.05, 0.10, 0.20, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80, 0.90, 1.00};

    for (double noise_percentage : noise_percentages)
    {
        // Create output filename dynamically based on the noise percentage
        string outputDatFile = "fmd_with_angle_noise_" + to_string(static_cast<int>(noise_percentage * 100)) + ".dat";
        
        // Apply noise and save to file
        addNoiseToAngleInDat(inputDatFile, outputDatFile, noise_percentage);
    }

    return 0;
}
