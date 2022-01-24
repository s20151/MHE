import os;
import re;
import numpy as np;

statistics = {
    "tabu_search":[],
    "hill_climb_rnd":[]
}

for method_name in statistics:
    for problem_size in range(5, 25):
        for repeat in range(1,15):
            cmndName = "./sript.py " + str(problem_size) +  " " + method_name + " --iterations 1000 --tabu_size 200"
            print(cmndName)
            result = os.popen(cmndName)
            output = result.read()
            calcTime = re.findall("dt.*", output)
            if (len(calcTime) > 0):
                calcTime = re.findall("[0-9.]+",calcTime[0])
                result_val = re.findall("[0-9.]+",re.findall("result.*", output)[0])
                statistics[method_name].append([problem_size,float(result_val[0]), float(calcTime[0])])


#print(statistics)
with open("result.plt", "a") as gnuplotfile:
    gnuplotfile.write("set term png\n")
    gnuplotfile.write("set output \"result.png\"\n")
    gnuplotfile.write("plot ")
    for method_name in statistics:
        print(method_name)
        summary = statistics[method_name]
        # print(summary)
        per_size = {}
        for values in summary:
            if (per_size.get(values[0]) is None):
                per_size[values[0]] = [[values[1], values[2]]]
            else:
                per_size[values[0]].append([values[1], values[2]])
        #print(per_size)
        for s in per_size:
            combined = np.mean(per_size[s], axis=0)
            with open("result_" + method_name + ".txt", "a") as myfile:
                myfile.write(str(s) + " "  + str(combined[0]) + " "+ str(combined[1]) + "\n")
        gnuplotfile.write("'result_" + method_name + ".txt' u 1:2 w lines, ")

    gnuplotfile.write("\n")


result = os.popen("gnuplot result.plt")
output = result.read()