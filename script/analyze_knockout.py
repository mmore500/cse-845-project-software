import pandas as pd
import seaborn as sns
from matplotlib import pyplot as plt
from scipy.stats import ttest_ind

# get data
dat = pd.read_csv('knockoutResults.csv')

# prepare data
dat = dat.dropna()

dat['Knockout Relative Fitness'] = dat.apply(
        lambda x: x['(koFitness-agentFitness)/agentFitness'] + 1,
        axis=1
    )

transl = {0:1000,1:500,2:100,3:50,4:10,5:5,6:1}
dat['Stigmergy Evaporation Delay (Updates)'] = dat.apply(
        lambda x: transl[x['cond']],
        axis=1
    )

# plot data
sns.barplot(
        x='Stigmergy Evaporation Delay (Updates)',
        y='Knockout Relative Fitness',
        data=dat,
        color='C0'
    )
plt.savefig('knockout_rel_fit_stig_delay.pdf')


# stats
cat1 = dat[dat['Stigmergy Evaporation Delay (Updates)']==1]
cat5 = dat[dat['Stigmergy Evaporation Delay (Updates)']==5]
cat10 = dat[dat['Stigmergy Evaporation Delay (Updates)']==10]
cat50 = dat[dat['Stigmergy Evaporation Delay (Updates)']==50]
cat100 = dat[dat['Stigmergy Evaporation Delay (Updates)']==100]
cat500 = dat[dat['Stigmergy Evaporation Delay (Updates)']==500]
cat1000 = dat[dat['Stigmergy Evaporation Delay (Updates)']==1000]

print("cat1 vs cat5")
print(ttest_ind(
        cat1['Knockout Relative Fitness'],
        cat5['Knockout Relative Fitness']
    ))

print("cat1 vs cat10")
print(ttest_ind(
        cat1['Knockout Relative Fitness'],
        cat10['Knockout Relative Fitness']
    ))
print("cat1 vs cat50")
print(ttest_ind(
        cat1['Knockout Relative Fitness'],
        cat50['Knockout Relative Fitness']
    ))
print("cat1 vs cat100")
print(ttest_ind(
        cat1['Knockout Relative Fitness'],
        cat100['Knockout Relative Fitness']
    ))

print("cat500 vs cat5")
print(ttest_ind(
        cat500['Knockout Relative Fitness'],
        cat5['Knockout Relative Fitness']
    ))
print("cat500 vs cat10")
print(ttest_ind(
        cat500['Knockout Relative Fitness'],
        cat10['Knockout Relative Fitness']
    ))
print("cat500 vs cat50")
print(ttest_ind(
        cat500['Knockout Relative Fitness'],
        cat50['Knockout Relative Fitness']
    ))
print("cat500 vs cat100")
print(ttest_ind(
        cat500['Knockout Relative Fitness'],
        cat100['Knockout Relative Fitness']
    ))

print("cat1000 vs cat5")
print(ttest_ind(
        cat1000['Knockout Relative Fitness'],
        cat5['Knockout Relative Fitness']
    ))
print("cat1000 vs cat10")
print(ttest_ind(
        cat1000['Knockout Relative Fitness'],
        cat10['Knockout Relative Fitness']
    ))
print("cat1000 vs cat50")
print(ttest_ind(
        cat1000['Knockout Relative Fitness'],
        cat50['Knockout Relative Fitness']
    ))
print("cat1000 vs cat100")
print(ttest_ind(
        cat1000['Knockout Relative Fitness'],
        cat100['Knockout Relative Fitness']
    ))
