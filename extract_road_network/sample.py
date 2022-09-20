import random

fname = "Datasets_Yun/London_cut"

for sample_ratio in [0.25, 0.5, 0.75]:

    f = open(fname+"_clean.csv")
    lines = f.read()
    f.close()

    lines = lines.strip().split('\n')
    print(len(lines))

    sample_lines = random.sample(lines, int(sample_ratio*len(lines)))

    print(len(sample_lines))

    f2 = open(fname+"_clean_s"+str(sample_ratio)+".csv", "w")
    for line in sample_lines:
        f2.write(line+"\n")
    f2.close()




