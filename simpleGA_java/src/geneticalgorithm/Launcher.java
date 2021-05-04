package geneticalgorithm;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.util.Arrays;

/**
 *
 * @author Marco Virgolin, with the collaboration of Anton Bouter and Hoang Ngoc Luong and the supervision of Peter A.N. Bosman
 */
public class Launcher {

    public static void main(String[] args) throws IOException {
       
		if( args.length != 2 )
		{
			System.out.println("Invalid number of parameters.");
			System.out.println("Required parameters: <maxcut-instance> <population-size> <evaluation-limit>");
			return;
		}

		 // GA parameters
		String maxcut_file = "maxcut_instance.txt";
        int population_size = Integer.parseInt(args[0]);
        CrossoverType ct = CrossoverType.Uniform;
		FitnessFunction fitness_function = new MaxCutFunction(maxcut_file);

        // Termination condition parameters ( 0 or negatives are ignored )
        long time_limit = -1; // in milliseconds
        int generations_limit = -1;
        //long evaluations_limit = 100000;
		long evaluations_limit = Long.parseLong(args[1]);
        
		// Set up logging
        String output_file_name = "statistics.dat";
        Files.deleteIfExists(new File(output_file_name).toPath());
        Utilities.logger = new BufferedWriter(new FileWriter(output_file_name, true));
        Utilities.logger.write("gen evals time best_fitness\n");

        // Run GA
        System.out.println("Starting run with pop_size=" + population_size);
        SimpleGeneticAlgorithm ga = new SimpleGeneticAlgorithm(population_size, ct, fitness_function);
        try {
            ga.run(generations_limit, evaluations_limit, time_limit);
            
            System.out.println("Best fitness " + ga.fitness_function.elite.fitness + " found at\n"
                    + "generation\t" + ga.generation + "\nevaluations\t" + ga.fitness_function.evaluations + "\ntime (ms)\t" + (System.currentTimeMillis() - ga.start_time + "\n")
                    + "elite\t\t" + ga.fitness_function.elite.toString());
            
        } catch (FitnessFunction.OptimumFoundCustomException ex) {
            System.out.println("Optimum " + ga.fitness_function.elite.fitness + " found at\n"
                    + "generation\t" + ga.generation + "\nevaluations\t" + ga.fitness_function.evaluations + "\ntime (ms)\t" + (System.currentTimeMillis() - ga.start_time + "\n")
                    + "elite\t\t" + ga.fitness_function.elite.toString());
            Utilities.logger.write(ga.generation + " " + ga.fitness_function.evaluations + " " + (System.currentTimeMillis() - ga.start_time) + " " + ga.fitness_function.elite.fitness + "\n");
        }
        Utilities.logger.close();

    }
}
