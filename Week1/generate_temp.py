import random

def generate_temperature_data(filename="temperature_log.txt"):
    with open(filename, "w") as file:
        for hour in range(24):
            for _ in range(random.randint(4, 10)):  # Random number of readings per hour
                minute = random.randint(0, 59)
                temperature = round(random.uniform(-5, 5), 1)  # Random temp between -5 and 5°C
                file.write(f"{hour:02d}:{minute:02d} {temperature}\n")

generate_temperature_data()
print("Temperature log generated: temperature_log.txt")
