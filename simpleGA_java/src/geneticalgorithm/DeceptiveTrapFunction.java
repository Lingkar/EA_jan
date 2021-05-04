
package geneticalgorithm;

/**
 *
 * @author Anton Bouter, with the collaboration of Marco Virgolin and Hoang Ngoc Luong and the supervision of Peter A.N. Bosman
 */
public class DeceptiveTrapFunction extends FitnessFunction{

    int m, k;

    DeceptiveTrapFunction(int m, int k) {
		this.optimum_known = true;
        this.optimum = m;
		this.number_of_variables = m*k;
    }

	protected double calculateFitness(Individual individual) throws OptimumFoundCustomException {
		double result = 0;
		for ( int i = 0; i < m; i++ ) {
			int unitation = 0;
			for( int j = 0; j < k; j++ )
				unitation += individual.genotype[i*k+j];
			if( unitation == k )
				result++;
			else
				result += (1.0-(1.0/k)) * (k-1.0-unitation) / (k-1.0);
		}
		return( result );
	}    

}
