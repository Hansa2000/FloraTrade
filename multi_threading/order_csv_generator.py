import csv
import random

# CSV file header
header = ["Cl.Ord.ID", "Instrument", "Side", "Quantity", "Price"]

print("Number of entries : ")
num_trades = int(input().strip())

# Generate data for entries
data = []
for i in range(num_trades):
    order_id = "aa" + str(i)
    instrument = "Rose"
    side = random.choice([1, 2])
    quantity = random.randint(1, 100) * 10
    price = random.randint(1, 1000)
    
    data.append((order_id, instrument, side, quantity, price))

# Outfile name 
name = "order_"+str(num_trades)+".csv"

# Write data to CSV file
with open(name, "w", newline="") as csvfile:
    csvwriter = csv.writer(csvfile)
    csvwriter.writerow(header)
    csvwriter.writerows(data)

print("CSV file generated successfully.")