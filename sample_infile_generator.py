import random
import sys

def generate_customer_file(filename, num_customers):
    with open(filename, 'w') as file:
        for customer_number in range(1, num_customers + 1):
            service_time = random.randint(1, 10)
            file.write(f"{customer_number} {service_time}\n")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <num_customers>")
        sys.exit(1)

    num_customers = int(sys.argv[1])
    generate_customer_file("c_file", num_customers)
    print(f"{num_customers} customers have been generated in 'c_file'.")