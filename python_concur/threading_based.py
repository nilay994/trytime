import threading
import requests
import datetime

def fetch_data(url, result_container):
    response = requests.get(url)
    result_container.append(response.text)

a = datetime.datetime.now()

# Container to store results
results = []

# Create threads to perform I/O operations concurrently
thread1 = threading.Thread(target=fetch_data, args=("https://example.com", results))
thread2 = threading.Thread(target=fetch_data, args=("https://example.org", results))

# Start the threads
thread1.start()
thread2.start()

# Wait for both threads to finish
thread1.join()
thread2.join()

# Access the results
result1, result2 = results
print(result1, result2)

b = datetime.datetime.now()
delta = b - a
print(delta)
