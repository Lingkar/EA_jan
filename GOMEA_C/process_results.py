import json
import os
import statistics as stat

directory = "./results-a/"
results = {}
maxPop = 0
for filename in os.listdir(directory):
    file = open(directory + filename, 'r')
    test = json.load(file)
    for i in test:
        if not i[0][3]:
            no_variables = i[0][0]
            population_size = i[1][0]
            if(population_size > maxPop):
                maxPop = population_size
            evaluations = i[1][1]
            if no_variables in results:
                results[no_variables]["population_size"].append(population_size)
                results[no_variables]["evaluations"].extend(evaluations)
            else:
                results[no_variables] = {"population_size": [population_size], "evaluations": evaluations}

print("problem size:", 6)
print("Population stats")
print(stat.mean(results[6]['population_size']))
print(stat.quantiles(results[6]['population_size']))
print("Evaluation stats")
print(stat.mean(results[6]['evaluations']))
print(stat.quantiles(results[6]['evaluations']))


print("problem size:", 12)
print("Population stats")
print(stat.mean(results[12]['population_size']))
print(stat.quantiles(results[12]['population_size']))
print("Evaluation stats")
print(stat.mean(results[12]['evaluations']))
print(stat.quantiles(results[12]['evaluations']))

print("problem size:", 25)
print("Population stats")
print(stat.mean(results[25]['population_size']))
print(stat.quantiles(results[25]['population_size']))
print("Evaluation stats")
print(stat.mean(results[25]['evaluations']))
print(stat.quantiles(results[25]['evaluations']))

print("problem size:", 50)
print("Population stats")
print(stat.mean(results[50]['population_size']))
print(stat.quantiles(results[50]['population_size']))
print("Evaluation stats")
print(stat.mean(results[50]['evaluations']))
print(stat.quantiles(results[50]['evaluations']))


