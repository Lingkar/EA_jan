import subprocess
import os


# TU Delft CS4205 - Evolutionary Algorithms - Assignment 1
# Author: Anton Bouter

# This is an example script to run external code of sGA, EDA, and GOMEA
# This script requires code for sGA, EDA, and GOMEA in subdirectories 'simpleGA_java', 'EDA_C', and 'GOMEA_C', and maxcut instances in the subdirectory 'maxcut_instances', as supplied on brightspace
# Make sure the code has been compiled before running 

def runExperiment(instance_info, pop, U):
    # Copy a maxcut instance to the current directory
    subprocess.run(['cp', directory + instance_info[2] + '.txt', 'maxcut_instance.txt'])
    # Copy the optimal value for this maxcut instance to the current directory
    subprocess.run(['cp', directory + instance_info[2] + '.bkv', 'vtr.txt'])

    f = open("vtr.txt", "r")
    opt = float(f.readline().strip())

    # Parameters for GOMEA and EDA
    pro = 5  # Problem ID (5 for maxcut)
    dim = instance_info[0]  # Make sure this is the same as the problem instance.
    # TODO:: Does the evaluation has to scale with the problem instances?
    eva = 1000000000000000000000  # Evaluation budget
    tol = 0

    # Run GOMEA
    # Results can be found in
    # statistics.dat -- generational statistics
    # best_ever.dat -- best solution found
    # best_ever_hitting_time.dat -- hitting time of best solution found
    print('# Running GOMEA...')

    experiment_result = []

    for i in range(10):
        if U:
            gomea_process = subprocess.Popen(['./GOMEA', '-U', '-s', str(pro), str(dim), str(pop), str(eva), str(tol)],
                                             stdout=subprocess.PIPE)
        else:
            gomea_process = subprocess.Popen(['./GOMEA', '-s', str(pro), str(dim), str(pop), str(eva), str(tol)],
                                             stdout=subprocess.PIPE)
        gomea_process.wait()
        if (i == 0):
            print('# best_solution best_fitness constraint_value tot_time_ms num_eval')
        with open('best_ever.dat') as f_elitist:
            with open('best_ever_hitting_time.dat') as f_time:
                l_elitist = f_elitist.readlines()
                l_time = f_time.readlines()

                info_experiment = l_elitist[-1].strip().split()
                info_time = l_time[-1].strip().split()
                fit = float(info_experiment[1])
                no_evals = int(info_time[1])
                experiment_result.append(no_evals)
                if fit != opt:
                    return -1
                print(l_elitist[-1].strip(), l_time[-1].strip())  # Last line of the results
    return experiment_result

# TODO:: Make these programm arguments
# Variables to set
directory = "../maxcut-instances/set0a/"
max_population = 10000


problem_instances = []
# List all the instances in directory for which experiments have to be ran
for filename in os.listdir(directory):
    if filename.endswith(".txt"):
        instance_info = filename[:-4][1:].lstrip('0').split('i')
        instance_info.append(filename[:-4])
        problem_instances.append(instance_info)
        continue

# Sort the instances based on the number of binary variables
problem_instances.sort(key=lambda x: x[0], reverse=True)


# TODO:: Do binary search over population size, and collect results
test = runExperiment(problem_instances[0], 10, True)
print(test)
