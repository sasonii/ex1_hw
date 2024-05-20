import csv

data = [
    [100, -16.666667, -41.666667],
    [200, 212.040000, 280.040000],
    [400, 28.000000, -40.000000],
    [800, 5.000000, -24.010000],
    [1600, 11.500000, -18.500000],
    [3200, 13.750000, -24.500000],
    [6400, 7.126250, -23.750000],
    [12800, 8.063125, -24.125000],
    [25600, 5.625312, 6.781562],
    [51200, 44.720469, 1.578125],
    [102400, 5.414297, 4.664219],
    [204800, 2.578203, 46.853320],
    [409600, 5.754102, 21.383613],
    [819200, 6.604746, 0.098623]
]

# Define the output CSV file name
output_file = 'output.csv'

# Write the data to the CSV file
with open(output_file, 'w', newline='') as csvfile:
    csvwriter = csv.writer(csvfile)
    csvwriter.writerows(data)

print(f"Data successfully written to {output_file}")

