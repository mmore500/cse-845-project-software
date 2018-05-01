import matplotlib.pyplot as plt

b = (0.0,0.0,0.0)
w = (1.0,1.0,1.0)
gy = (0.5,0.5,0.5)
r =  (0.15,0.15,0.55)
p = (0.5,0.5,0.75)

bl = (0.37,0.78,1.0)
gr = (72.5,1.0,0.18)
bg = (0.9,0.2,0.6)
y = (1.0,0.8,0.2)
pp = (0.6,0.2,0.9)


vision = [
    [w]*7,
    [w]+[gy]*5+[w],
    [w]*2+[gy]*3+[w]*2,
    [w]*7,
    [w]*7,
    [w]*7,
    [w]*7
]

stigexist = [
    [w]*7,
    [w]*7,
    [w]*2+[gy]*3+[w]*2,
    [w]*2+[gy]*3+[w]*2,
    [w]*2+[gy]*3+[w]*2,
    [w]*7,
    [w]*7
]

stigrw = [
    [w]*7,
    [w]*7,
    [w]*7,
    [w]*3+[gy]+[w]*3,
    [w]*7,
    [w]*7,
    [w]*7
]

def draw(im, ax):
    ax.imshow(im,extent=(0,7,7,0))

    major_ticks=[0,1,2,3,4,5,6,7]

    ax.set_xticks(major_ticks)
    ax.set_yticks(major_ticks)

    # hide tickmarks
    for tic in ax.xaxis.get_major_ticks() + ax.yaxis.get_major_ticks():
        tic.tick1On = tic.tick2On = False
        tic.label1On = tic.label2On = False

    # Gridlines based on minor ticks
    ax.grid(which='Major',color='black', linestyle='-', linewidth=1)

    ax.add_patch(plt.Polygon([[3.75,3.75],[3.25,3.75],[3.5,3.25]], color='black'))

    return ax

f, (r1) = plt.subplots(1,5);

f.suptitle("Agent Sensor Design")

draw(vision, r1[0]).set_xlabel('Obstacle Vision')

draw(vision, r1[1]).set_xlabel('Home/Food Vision')

draw(stigexist, r1[2]).set_xlabel('Stigmergy Presence')

draw(stigrw, r1[3]).set_xlabel('Stigmery Read/Write')

draw(stigrw, r1[4]).set_xlabel('Compass Orientation')

plt.gcf().set_size_inches(12, 2)

plt.savefig('sense_explanatory.pdf')
