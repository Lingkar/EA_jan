
package geneticalgorithm;

/**
 *
 * @author Marco Virgolin, with the collaboration of Anton Bouter and Hoang Ngoc Luong and the supervision of Peter A.N. Bosman
 */
public class Individual {

    int[] genotype;
    double fitness;

    public Individual(int genotype_length) {
        this.genotype = new int[genotype_length];
        this.fitness = 0;

        // random initialization of the genotype
        for (int i = 0; i < genotype_length; i++) {
            this.genotype[i] = Utilities.rng.nextDouble() < 0.5 ? 0 : 1;
        }
    }

    public Individual Clone() {
        Individual individual_clone = new Individual(genotype.length);
        individual_clone.genotype = genotype.clone();   // explicit call to clone because otherwise Java copies the reference
        individual_clone.fitness = fitness;
        return individual_clone;
    }

    public String toString() { // overrides the toString() method to beautifully print the genotype
        String genotype_string = "";
        for(int i = 0; i < genotype.length; i++) {
            genotype_string += genotype[i];
        }
        return genotype_string;
    }

}
