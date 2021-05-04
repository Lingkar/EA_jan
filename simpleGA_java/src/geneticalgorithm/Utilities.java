
package geneticalgorithm;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.util.Random;

/**
 *
 * @author Marco Virgolin, with the collaboration of Anton Bouter and Hoang Ngoc Luong and the supervision of Peter A.N. Bosman
 */
public class Utilities {
    
    public static Random rng = new Random();
    
    public static BufferedWriter logger;

    public static int [] CreateRandomPermutation(int n) {
        int [] result = new int[n];
        for(int i = 0; i < n; i++)
            result[i] = i;
        
        for (int i=0; i<n; i++) {
            int randomPosition = rng.nextInt(n);
            int temp = result[i];
            result[i] = result[randomPosition];
            result[randomPosition] = temp;
        }
        
        return result;
    }
    
}
