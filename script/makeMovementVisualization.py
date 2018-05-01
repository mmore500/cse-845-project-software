import matplotlib.pyplot as plt

b = (0.0,0.0,0.0)
w = (1.0,1.0,1.0)
gy = (0.5,0.5,0.5)
r =  (0.15,0.15,0.55)
p = (0.5,0.5,0.75)

bl = (0.37,0.78,1.0)
gr = (71.5,1.0,0.18)
bg = (0.9,0.2,0.6)
y = (1.0,0.8,0.2)
pp = (0.6,0.2,0.9)


blank = [
    [w]*3,
    [w]*3,
    [w]*3,
]

def draw(im, ax):
    ax.imshow(im,extent=(0,3,3,0))

    major_ticks=[0,1,2,3]

    ax.set_xticks(major_ticks)
    ax.set_yticks(major_ticks)

    # hide tickmarks
    for tic in ax.xaxis.get_major_ticks() + ax.yaxis.get_major_ticks():
        tic.tick1On = tic.tick2On = False
        tic.label1On = tic.label2On = False

    # Gridlines based on minor ticks
    ax.grid(which='Major',color='black', linestyle='-', linewidth=1)


    return ax

f, (r1, r2) = plt.subplots(2,4);

f.suptitle("Agent Locomotion Design")

ax = draw(blank, r1[0])

ax.add_patch(plt.Polygon([[1.25,0.75],[1.75,0.75],[1.5,0.25]], color='black'))

ax.set_ylabel('$t+1$         ', rotation=0)

ax = draw(blank, r1[1])

ax.add_patch(plt.Polygon([[1.75,1.75],[1.75,1.25],[1.25,1.5]], color='black'))

ax = draw(blank, r1[2])

ax.add_patch(plt.Polygon([[1.25,1.25],[1.25,1.75],[1.75,1.5]], color='black'))

ax = draw(blank, r1[3])

ax.add_patch(plt.Polygon([[1.75,1.75],[1.25,1.75],[1.5,1.25]], color='black'))

ax = draw(blank, r2[0])

ax.set_ylabel('$t$      ', rotation=0)
ax.set_xlabel('Output: 11')

ax.add_patch(plt.Polygon([[1.75,1.75],[1.25,1.75],[1.5,1.25]], color='black'))

ax = draw(blank, r2[1])

ax.set_xlabel('Output: 10')

ax.add_patch(plt.Polygon([[1.75,1.75],[1.25,1.75],[1.5,1.25]], color='black'))

ax = draw(blank, r2[2])

ax.set_xlabel('Output: 01')

ax.add_patch(plt.Polygon([[1.75,1.75],[1.25,1.75],[1.5,1.25]], color='black'))

ax = draw(blank, r2[3])

ax.set_xlabel('Output: 00')

ax.add_patch(plt.Polygon([[1.75,1.75],[1.25,1.75],[1.5,1.25]], color='black'))

plt.gcf().set_size_inches(12, 5)
plt.show()

plt.savefig('movement_explanatory.pdf')
