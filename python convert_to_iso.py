import json

# Load the minutiae data from the JSON file
with open('fmd_with_noise.json', 'r') as file:
    data = json.load(file)

# Open a new file to write the ISO formatted data
with open('fmd_iso_output.txt', 'w') as output_file:
    # Write initial information based on the fmd.txt format
    output_file.write(f"0\n{data['height']}\n{len(data['minutiae'])}\n")

    # Iterate through minutiae and write them in the required format
    for minutia in data['minutiae']:
        x = minutia['x']
        y = minutia['y']
        angle = minutia['angle']
        output_file.write(f"{int(x)} {int(y)} {angle:.6f}\n")

print("ISO formatted data saved to 'fmd_iso_output.txt'.")