import pandas as pd
import os
import seaborn as sns
from matplotlib import pyplot as plt
import re
from tqdm import tqdm, tqdm_pandas
from scipy.stats import ttest_ind

data = {
    e : {
    t : {
    r : {
    'max' :
    pd.read_csv(os.path.join(os.path.join(os.path.join(os.path.join("data", e), t), r), 'max.csv')),
    'pop' :
    pd.read_csv(os.path.join(os.path.join(os.path.join(os.path.join("data", e), t), r), 'pop.csv'))
    }
    for r in os.listdir(os.path.join(os.path.join(os.path.join("data", e), t)))
    if os.path.exists(os.path.join(os.path.join(os.path.join(os.path.join('data', e), t), r), 'pop.csv'))
    and os.path.exists(os.path.join(os.path.join(os.path.join(os.path.join("data", e), t), r), 'max.csv'))
    }
    for t in os.listdir(os.path.join(os.path.join("data", e)))
    }
    for e in os.listdir("data") if os.path.isdir(os.path.join("data", e))
}

# IMPORT DATA

print('loaded!')

res = []

for e, d1 in data.items():
    for t, d2 in d1.items():
        for r, d3 in d2.items():
            for f, df in d3.items():
                df['exper'] = e
                df['treat'] = t
                df['rep'] = r
                df['file'] = f
                if len(df) >= 2000:
                    res.append(df)

complete = pd.concat(res)

# FILTER TO 20k UPDATES FOR EACH EXPERIMENT
# REMOVE REPS THAT DIDN'T REACH 20k UPDATES

fil = complete[complete['update'] <= 20000]
fil = fil[(fil['update'] == 20000) & (fil['file'] == 'pop')]

# MAKE COLUMNS THAT ARE NICE TO PLOT

# Population Average Fitness Score

fil['Population Average Fitness Score'] = fil['score_AVE']

# Hidden Node Count

transl = {
        'C0__STIG_0__MBHN_0' : 0,
        'C1__STIG_0__MBHN_1' : 1,
        'C2__STIG_0__MBHN_2' : 2,
        'C3__STIG_0__MBHN_4' : 4,
        'C4__STIG_0__MBHN_8' : 8,
        'C5__STIG_0__MBHN_16' : 16,
        'C6__STIG_0__MBHN_32' : 32,
        'C7__STIG_0__MBHN_64' : 64,
        'C8__STIG_0__MBHN_128' : 128
    }

fil['Hidden Node Count'] = fil.apply(lambda x: transl[x['treat']] if x['treat'] in transl else 'NA', axis=1)

# Pheromone Evaporation Delay (Updates)

transl = {
        'C6__STIG_1__TIME_1__MBHN_8__TDR8_1' : 1,
        'C5__STIG_1__TIME_1__MBHN_8__TDR8_5' : 5,
        'C4__STIG_1__TIME_1__MBHN_8__TDR8_10' : 10,
        'C3__STIG_1__TIME_1__MBHN_8__TDR8_50' : 50,
        'C2__STIG_1__TIME_1__MBHN_8__TDR8_100' : 100,
        'C1__STIG_1__TIME_1__MBHN_8__TDR8_500' : 500,
        'C0__STIG_1__TIME_1__MBHN_8__TDR8_1000' : 1000
    }
fil['Pheromone Evaporation Delay (Updates)'] = fil.apply(lambda x: transl[x['treat']] if x['treat'] in transl else 'NA', axis=1)

# Obstacle Density

transl = {
        'C0__WALL_1.0' : 0.0,
        'C1__WALL_0.8' : 0.2,
        'C2__WALL_0.6' : 0.4,
        'C3__WALL_0.4' : 0.6,
        'C4__WALL_0.2' : 0.8,
        'C5__WALL_0.0' : 1.0
    }
fil['Obstacle Density'] = fil.apply(lambda x: transl[x['treat']] if x['treat'] in transl else 'NA', axis=1)

# MAKE BARPLOTS

plt.clf()
sns.barplot(
        x='Hidden Node Count',
        y='Population Average Fitness Score',
        data=fil[fil['exper'] == 'stig_hidden'],
        color='C0'
    )
plt.savefig('hidden_node_count_average_fitness.pdf')

plt.clf()
sns.barplot(
        x='Pheromone Evaporation Delay (Updates)',
        y='Population Average Fitness Score',
        data=fil[fil['exper'] == 'stig_timeDecay'],
        color='C0'
    )
plt.savefig('pheromone_evaporation_delay_average_fitness.pdf')

plt.clf()
sns.barplot(
        x='Obstacle Density',
        y='Population Average Fitness Score',
        data=fil[fil['exper'] == 'obsDensity'],
        color='C0'
    )
plt.savefig('obstacle_density_average_fitness.pdf')

# STATISTICS

cat1 = fil[fil['Pheromone Evaporation Delay (Updates)']==1]
cat5 = fil[fil['Pheromone Evaporation Delay (Updates)']==5]
cat10 = fil[fil['Pheromone Evaporation Delay (Updates)']==10]
cat50 = fil[fil['Pheromone Evaporation Delay (Updates)']==50]
cat100 = fil[fil['Pheromone Evaporation Delay (Updates)']==100]
cat500 = fil[fil['Pheromone Evaporation Delay (Updates)']==500]
cat1000 = fil[fil['Pheromone Evaporation Delay (Updates)']==1000]

print("cat1 vs cat5")
print(ttest_ind(
        cat1['Population Average Fitness Score'],
        cat5['Population Average Fitness Score']
    ))

print("cat1 vs cat10")
print(ttest_ind(
        cat1['Population Average Fitness Score'],
        cat10['Population Average Fitness Score']
    ))
print("cat1 vs cat50")
print(ttest_ind(
        cat1['Population Average Fitness Score'],
        cat50['Population Average Fitness Score']
    ))
print("cat1 vs cat100")
print(ttest_ind(
        cat1['Population Average Fitness Score'],
        cat100['Population Average Fitness Score']
    ))

print("cat500 vs cat5")
print(ttest_ind(
        cat500['Population Average Fitness Score'],
        cat5['Population Average Fitness Score']
    ))
print("cat500 vs cat10")
print(ttest_ind(
        cat500['Population Average Fitness Score'],
        cat10['Population Average Fitness Score']
    ))
print("cat500 vs cat50")
print(ttest_ind(
        cat500['Population Average Fitness Score'],
        cat50['Population Average Fitness Score']
    ))
print("cat500 vs cat100")
print(ttest_ind(
        cat500['Population Average Fitness Score'],
        cat100['Population Average Fitness Score']
    ))

print("cat1000 vs cat5")
print(ttest_ind(
        cat1000['Population Average Fitness Score'],
        cat5['Population Average Fitness Score']
    ))
print("cat1000 vs cat10")
print(ttest_ind(
        cat1000['Population Average Fitness Score'],
        cat10['Population Average Fitness Score']
    ))
print("cat1000 vs cat50")
print(ttest_ind(
        cat1000['Population Average Fitness Score'],
        cat50['Population Average Fitness Score']
    ))
print("cat1000 vs cat100")
print(ttest_ind(
        cat1000['Population Average Fitness Score'],
        cat100['Population Average Fitness Score']
    ))
