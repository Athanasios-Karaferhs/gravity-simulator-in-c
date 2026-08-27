import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("positions.csv")
for body_id, group in df.groupby("body"):
    plt.plot(group["x"], group["y"], linewidth=0.5)
plt.gca().set_aspect("equal")
plt.savefig("orbits.png")