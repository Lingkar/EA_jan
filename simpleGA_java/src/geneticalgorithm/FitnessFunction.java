
package geneticalgorithm;

/**
 *
 * @author Marco Virgolin, with the collaboration of Anton Bouter and Hoang Ngoc Luong and the supervision of Peter A.N. Bosman
 */
abstract class FitnessFunction {

	int number_of_variables;
    long evaluations = 0;
    double optimum;
	boolean optimum_known = false;

    Individual elite = null;

    FitnessFunction() {}
    
    // Halt the GA as soon as the optimum is found
    class OptimumFoundCustomException extends Exception {
        public OptimumFoundCustomException(String message) {
            super(message);
        }
    }

    public void Evaluate(Individual individual) throws OptimumFoundCustomException {

        evaluations++;

        // set the fitness of the individual
        individual.fitness = calculateFitness(individual);

        // update elite
        if (elite == null || elite.fitness < individual.fitness) {
            elite = individual.Clone();
        }

        // check if optimum has been found
        if ( optimum_known && individual.fitness == optimum) {
            // throw our custom exception
            throw new OptimumFoundCustomException("Optimum found");
        }
    }

	protected abstract double calculateFitness(Individual individual) throws OptimumFoundCustomException;

}
