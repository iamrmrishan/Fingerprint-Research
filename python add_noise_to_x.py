import json
import numpy as np

# Load the minutiae data from the JSON file
with open('fmd.json', 'r') as file:
    data = json.load(file)

# Function to add 1% Gaussian noise to the X-coordinate
def add_noise_to_x(x_value, noise_percentage=0.01):
    noise = np.random.normal(0, noise_percentage * x_value)
    return x_value + noise

# Add noise to the X-coordinate of each minutia
for minutia in data["minutiae"]:
    minutia["x"] = add_noise_to_x(minutia["x"])

# Save the updated data back to a new JSON file
with open('fmd_with_noise.json', 'w') as output_file:
    json.dump(data, output_file, indent=4)

print("1% noise added to X-coordinates. Updated data saved to 'fmd_with_noise.json'.")
