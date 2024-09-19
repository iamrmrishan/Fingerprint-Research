import json
import numpy as np

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

def analyze_minutiae_data(data):
    x_values = [minutia['x'] for minutia in data['minutiae']]
    y_values = [minutia['y'] for minutia in data['minutiae']]
    angle_values = [minutia['angle'] for minutia in data['minutiae']]

    attributes = {
        'x': x_values,
        'y': y_values,
        'angle': angle_values
    }

    variances = {}
    for attribute, values in attributes.items():
        mean_value = np.mean(values)
        variance_value = np.var(values)
        variances[attribute] = variance_value
        print(f"Attribute: {attribute}")
        print(f"  Mean: {mean_value}")
        print(f"  Variance: {variance_value}")
        print()

    # Determine most and least weighted attributes
    most_weighted = min(variances, key=variances.get)
    least_weighted = max(variances, key=variances.get)

    print(f"Most Weighted Attribute: {most_weighted}")
    print(f"Least Weighted Attribute: {least_weighted}")

# Load JSON data
data = load_minutiae_data('fmd.json')

# Print minutiae attributes
print_minutiae_attributes(data)

# Analyze minutiae data
analyze_minutiae_data(data)


