import json

def load_minutiae_data(file_path):
    with open(file_path, 'r') as file:
        data = json.load(file)
    return data

def print_minutiae_attributes(data):
    print(f"Width: {data['width']}")
    print(f"Height: {data['height']}")
    print(f"Number of Minutiae: {data['numMinutiae']}")
    for minutia in data['minutiae']:
        print(f"x: {minutia['x']}, y: {minutia['y']}, angle: {minutia['angle']}")

# Load JSON data
data = load_minutiae_data('fmd.json')

# Print minutiae attributes
print_minutiae_attributes(data)
