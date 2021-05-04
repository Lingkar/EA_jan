
package geneticalgorithm;

import java.util.ArrayList;

/**
 *
 * @author Marco Virgolin, with the collaboration of Anton Bouter and Hoang Ngoc Luong and the supervision of Peter A.N. Bosman
 */
public class Selection {

    public ArrayList<Individual> TournamentSelect(ArrayList<Individual> individuals) {

        // L3.6 onwards
        ArrayList<Individual> result = new ArrayList<>();
        
        int n = individuals.size() / 2; // auxiliary variable to follow the pseudocode

        for (int j = 0; j < 2; j++) {
            
            int[] perm = Utilities.CreateRandomPermutation( 2*n );
            
            for (int i = 0; i < n / 2; i++) {
                ArrayList<Individual> candidates = new ArrayList<>();
                for (int k = 0; k < 4; k++) {
                    candidates.add(individuals.get(perm[4 * i + k]));
                }
                result.add(GetBest(candidates));
            }
            
        }

        return result;
    }

    private Individual GetBest(ArrayList<Individual> candidates) {

        Individual best = candidates.get(0);
        for (int i = 1; i < candidates.size(); i++) {
            
            if (candidates.get(i).fitness >= best.fitness) {
                best = candidates.get(i);
            }
            
        }
        return best;

    }

}
