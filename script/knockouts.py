import subprocess


def bash_command(cmd):
    subprocess.Popen(cmd, shell=True, executable='/bin/bash').wait()

	
condCount = 7
repCount = 142

with open("knockoutResults.csv",'w') as output:
    
    output.write("{},{},{},{},{},{}\n".format('cond','rep','agentFitness','koFitness','koFitness-agentFitness','(koFitness-agentFitness)/agentFitness)')

    for cond in range(7):
        cond2list = [1000,500,100,50,10,5,1]
        cond2 = cond2list[cond]
        bash_command("mkdir knockout{}".format(cond))
        for repn in range(repCount):
            rep = repn + 101
            bash_command("mkdir knockout{}/{}".format(cond,rep))
            path_to_LOD_org = "timeDecay/C{}__STIG_1__TIME_1__MBHN_8__TDR8_{}/{}/LOD_organisms.csv".format(cond,cond2,rep)
            path_to_LOD_dat = "timeDecay/C{}__STIG_1__TIME_1__MBHN_8__TDR8_{}/{}/LOD_data.csv".format(cond,cond2,rep)
            run_mabe = "./mabe -f settings* -p GLOBAL-outputDirectory ./knockout{}/{}/ GLOBAL-initPop \"MASTER = greatest 1 by ID from '{}'\"".format(cond,rep,path_to_LOD_org)
            bash_command(run_mabe)
            agentID = ""
            with open("{}".format(path_to_LOD_org), 'r') as org:
                for line in org:
                    qsplit = line.split('"')
                    if len(qsplit) > 1 and int(qsplit[2].split(',')[2]) == 20000:
                        agentID = qsplit[2].split(',')[1]
            agentFitness = -1
            with open("{}".format(path_to_LOD_dat), 'r') as dat:
                for line in dat:
                    if agentID == line.split(',')[2]:
                        agentFitness = float(line.split('"')[4].split(',')[1])
                        break
            koFitness = 0
            koSum = 0
            koCount = 0
            with open("./knockout{}/{}/knockoutFitness.txt".format(cond, rep), 'r') as ko:
                for line in ko:
                      koSum += float(line)
                      koCount += 1.0
            koFitness = koSum/koCount
            if agentID != "" and agentFitness != -1:
                output.write("{},{},{},{},{},{}\n".format(cond,rep,agentFitness,koFitness,koFitness-agentFitness,(koFitness-agentFitness)/agentFitness))
            else:
                output.write("{},{},{},{},{},{}\n".format(cond,rep,"NA","NA","NA","NA"))
