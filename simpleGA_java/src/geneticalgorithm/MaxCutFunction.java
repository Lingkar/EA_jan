
package geneticalgorithm;

import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
/**
 *
 * @author Anton Bouter, with the collaboration of Marco Virgolin and Hoang Ngoc Luong and the supervision of Peter A.N. Bosman
 */
public class MaxCutFunction extends FitnessFunction{

	ArrayList<Edge> edgeList;
	int number_of_vertices;
	int number_of_edges;

    MaxCutFunction(String filename) throws FileNotFoundException {
    	readInstanceFromFile(filename);
		readOptimumFromFile("vtr.txt");
	}
    
	MaxCutFunction(String filename, String optimum_filename) throws FileNotFoundException {
    	readInstanceFromFile(filename);
		readOptimumFromFile(optimum_filename);
	}

	/* 
	* Reads a MaxCut instance with the following format:
	* line 1: <n; number of vertices> <m; number of edges>
	* m lines: <endpoint 1 (1-based index)> <endpoint 2 (1-based index)> <weight>
	*/
	private void readInstanceFromFile(String filename) throws FileNotFoundException
	{
		Scanner sc = new Scanner(new File(filename));
		number_of_vertices = sc.nextInt();
		number_of_edges = sc.nextInt();
		edgeList = new ArrayList<Edge>();
		for( int i = 0; i < number_of_edges; i++ )
		{
			int a = sc.nextInt() - 1;
			int b = sc.nextInt() - 1;
			double w = sc.nextDouble();
			assert( a >= 0 ); assert( a < number_of_vertices );
			assert( b >= 0 ); assert( b < number_of_vertices );
			edgeList.add(new Edge(a,b,w));
		}
		assert( edgeList.size() == number_of_edges );

		this.number_of_variables = number_of_vertices;
	}

	public void readOptimumFromFile(String optimum_filename) throws FileNotFoundException
	{
		Scanner sc = new Scanner(new File(optimum_filename));
		optimum = sc.nextDouble();
		optimum_known = true;
	}

	protected double calculateFitness(Individual individual) throws OptimumFoundCustomException {
		double cut = 0;
		for( Edge e : edgeList )
		{
			if( individual.genotype[e.vertex_a] != individual.genotype[e.vertex_b] )
				cut += e.weight;
		}
		return( cut );
	}    

}
