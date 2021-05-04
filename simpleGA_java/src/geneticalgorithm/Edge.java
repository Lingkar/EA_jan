
package geneticalgorithm;

/**
 *
 * @author Anton Bouter, with the collaboration of Marco Virgolin and Hoang Ngoc Luong and the supervision of Peter A.N. Bosman
 */
public class Edge {

    int vertex_a, vertex_b;
    double weight;

    public Edge(int a, int b, double w) {
    	this.vertex_a = a;
		this.vertex_b = b;
		this.weight = w;
	}
}
