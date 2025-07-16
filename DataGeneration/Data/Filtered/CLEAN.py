import pandas as pd

# Load CSV files
a = pd.read_csv('gsFilt.csv')
b = pd.read_csv('karaFilt.csv')
c = pd.read_csv('toomFilt.csv')

# Concatenate the data
all_data = pd.concat([a, b, c], ignore_index=True)

# Drop rows where 'n' or 'time' are missing
all_data.dropna(subset=['n', 'time'], inplace=True)

# Group by 'n' and select the minimum 'time' for each group
result = all_data.groupby('n', as_index=False).agg({'time': 'min'})

# Save the result to a new CSV file
result.to_csv('d.csv', index=False)

print("CSV file 'd.csv' has been created with the minimum time values for each n.")
