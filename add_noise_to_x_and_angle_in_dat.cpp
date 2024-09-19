#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <string>

using namespace std;

// Function to add Gaussian noise to a value (either x-coordinate or angle)
int add_noise(int value, double noise_percentage)
{
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> noise(0, noise_percentage * value);
    return value + static_cast<int>(noise(gen));
}

// Function to unpack 16-bit x or y value (14 bits for value, 2 bits for metadata)
int unpackXY(unsigned char high_byte, unsigned char low_byte)
{
    return ((high_byte << 8) | low_byte) & 0x3FFF; // 14 bits for the value
}

// Function to repack 16-bit x or y value (14 bits for value, 2 bits unchanged)
void packXY(int value, unsigned char &high_byte, unsigned char &low_byte, unsigned char original_high_byte)
{
    value &= 0x3FFF;                                          // Keep only 14 bits
    high_byte = ((original_high_byte & 0xC0) | (value >> 8)); // Preserve the top 2 bits
    low_byte = value & 0xFF;
}

// Function to add noise to the angle (8-bit value)
unsigned char add_noise_to_angle(unsigned char angle, double noise_percentage)
{
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> noise(0, noise_percentage * angle);
    return static_cast<unsigned char>(angle + static_cast<int>(noise(gen)));
}

void addNoiseToXAndAngleInDat(const string &inputDatFile, const string &outputDatFile, double noise_percentage)
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

        // Extract x-coordinate (first 2 bytes)
        int x = unpackXY(buffer[0], buffer[1]);

        // Add noise to x-coordinate
        x = add_noise(x, noise_percentage);

        // Repack the x-coordinate, preserving the top 2 bits (minutiae type/flags)
        packXY(x, buffer[0], buffer[1], buffer[0]);

        // Add noise to angle (5th byte in the buffer)
        buffer[4] = add_noise_to_angle(buffer[4], noise_percentage);

        // Write the modified buffer (with noise in x and angle) to the output file
        outFile.write(reinterpret_cast<char *>(buffer), sizeof(buffer));
    }

    inFile.close();
    outFile.close();
    cout << "Noise added to X-coordinate and Angle at " << noise_percentage * 100 << "% and saved to " << outputDatFile << endl;
}

int main()
{
    string inputDatFile = "fmd.dat"; // Input .dat file

    // Apply noise percentages: 1%, 2%, 5%, 10%, 20%, 30%, 40%, 50%, 60%, 70%, 80%, 90%, 100%
    double noise_percentages[] = {0.01, 0.02, 0.05, 0.10, 0.20, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80, 0.90, 1.00};

    for (double noise_percentage : noise_percentages)
    {
        // Create output filename dynamically based on the noise percentage
        string outputDatFile = "fmd_with_x_and_angle_noise_" + to_string(static_cast<int>(noise_percentage * 100)) + ".dat";

        // Apply noise and save to file
        addNoiseToXAndAngleInDat(inputDatFile, outputDatFile, noise_percentage);
    }

    return 0;
}
