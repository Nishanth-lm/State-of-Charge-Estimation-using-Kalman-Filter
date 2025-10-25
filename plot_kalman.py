import numpy as np
import matplotlib.pyplot as plt

# Load the estimation data
estimation_file = "estimated_soc.txt"  # Update path if necessary

# Read data from the estimation file
data = np.loadtxt(estimation_file, delimiter=",", skiprows=1)

# Extract columns
time_steps = data[:, 0]
ekf_soc = data[:, 1]
ukf_soc = data[:, 2]

# Plot the EKF and UKF estimated SoC values
plt.figure(figsize=(8, 5))
plt.plot(time_steps, ekf_soc, marker='o', linestyle='-', color='r', label="EKF Estimated SoC")
plt.plot(time_steps, ukf_soc, marker='s', linestyle='--', color='b', label="UKF Estimated SoC")

# Labels and title
plt.xlabel("Time (s)")
plt.ylabel("Estimated State of Charge (SoC)")
plt.title("EKF vs. UKF SoC Estimation")
plt.legend()
plt.grid(True)

# Show the plot
plt.show()
