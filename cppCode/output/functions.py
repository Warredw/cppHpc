def moving_average(data): 
    
    cma = []
    
    cumulative_sum = 0
    
    
    for i, value in enumerate(data, start = 1): 
        
        cumulative_sum += value
        cma.append(cumulative_sum / i)
        
        
    return cma 




import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Load the dataset
file_path = "new_df.csv"  # Adjust the path if needed
df = pd.read_csv(file_path)

# Melt the dataframe for visualization
df_melted = df.melt(id_vars=["run"], var_name="Category", value_name="Matches")

# Extract round numbers and relevance type
df_melted["Round"] = df_melted["Category"].str.extract(r'(\d+)$').astype(int)
df_melted["Relevance"] = df_melted["Category"].str.extract(r'(\w+)_matches')[0]

# ---- Heatmap ----
heatmap_data = df_melted.pivot_table(index="Relevance", columns="Round", values="Matches", aggfunc="mean")
plt.figure(figsize=(10, 6))
sns.heatmap(heatmap_data, annot=True, cmap="coolwarm", fmt=".1f", linewidths=0.5)
plt.title("Average Number of Matches by Round and Relevance Type")
plt.xlabel("Round")
plt.ylabel("Match Relevance")
plt.show()

# ---- Boxplots ----
plt.figure(figsize=(12, 6))
sns.boxplot(x="Round", y="Matches", hue="Relevance", data=df_melted)
plt.title("Distribution of Matches by Round and Relevance Type")
plt.xlabel("Round")
plt.ylabel("Number of Matches")
plt.legend(title="Relevance Type")
plt.show()

# ---- Stacked Area Chart ----
stacked_data = df_melted.groupby(["Round", "Relevance"])['Matches'].mean().unstack()
plt.figure(figsize=(12, 6))
stacked_data.plot(kind="area", stacked=True, alpha=0.7)
plt.title("Evolution of Match Relevance Across Rounds")
plt.xlabel("Round")
plt.ylabel("Average Number of Matches")
plt.legend(title="Relevance Type")
plt.show()

# ---- Histograms ----
plt.figure(figsize=(12, 6))
relevance_categories = df_melted["Relevance"].unique()
for relevance in relevance_categories:
    subset = df_melted[df_melted["Relevance"] == relevance]["Matches"]
    sns.histplot(subset, bins=20, kde=True, label=relevance, alpha=0.6)

plt.title("Distribution of Matches Across Competitions")
plt.xlabel("Number of Matches")
plt.ylabel("Frequency")
plt.legend(title="Relevance Type")
plt.show()