import os
import numpy as np
from scipy.stats import sem


def confidence(outcomes):
    if len(outcomes):
        error_range = sem(outcomes) * 1.96
        avg_value = np.average(outcomes)
        confidence = f"{avg_value} +- {error_range}"
        print(confidence)

shallow_vl = []
deep_vl = []
with open("E:\\Projects_chess\\probData.txt","r",encoding="utf-8") as f:
    lines = f.readlines()
    for line in lines:
        line = line.strip()
        split_res = line.split(",")
        #print(split_res[0],split_res[1])
        shallow_vl.append(float(split_res[0]))
        deep_vl.append(float(split_res[1]))

shallow_vl = np.array(shallow_vl)
deep_vl = np.array(deep_vl)

delta_vl = deep_vl - shallow_vl

confidence(delta_vl)

